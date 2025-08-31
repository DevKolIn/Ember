#include "MonsterAIBase.h"

#include "ALootManagerActor.h"
#include "AI/MonsterAIController.h"
#include "GAS/EmberAbilitySystemComponent.h"
#include "AI/AbilitySystem/MonsterGameplayEffectComponent.h"
#include "AI/AbilitySystem/Abilities/MonsterGameplayAbility.h"
#include "AI/Data/MonsterAIData.h"
#include "AI/Data/MonsterAITemplate.h"
#include "AI/Weapon/MonsterAIWeapon.h"
#include "MonsterAIAttributeSet.h"
#include "Component/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "GAS/Attribute/EmberAS_Player.h"
#include "Perception/AISense_Damage.h"

AMonsterAIBase::AMonsterAIBase()
{
	AIControllerClass = AMonsterAIController::StaticClass();
	
	ASC = CreateDefaultSubobject<UEmberAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UEmberAS_Player>(TEXT("AttributeSet"));

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 180.f);
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AMonsterAIBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMonsterAIBase::BeginPlay()
{
	Super::BeginPlay();
	
	ASC->InitAbilityActorInfo(this, this);
	
	if (HasAuthority())
	{
		InitializeMonsterAI();
	}
}

void AMonsterAIBase::InitializeMonsterAI()
{
	const UMonsterAITemplate& MonsterAITemplate = UMonsterAIData::Get().FindMonsterAITemplateByClass(GetClass());

	/* Create Weapon */
	if (MonsterAITemplate.WeaponClass)
	{
		UWorld* World = GetWorld();
		WeaponActor = World->SpawnActorDeferred<AMonsterAIWeapon>(MonsterAITemplate.WeaponClass, FTransform::Identity, this);
		WeaponActor->SetActorRelativeTransform(MonsterAITemplate.WeaponAttachTransform);
		WeaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, MonsterAITemplate.WeaponAttachSocket);
		WeaponActor->FinishSpawning(FTransform::Identity, true);
	}

	/* Grant Ability*/
	if (ASC)
	{
		for (const FEmberAbilitySet_GameplayAbility& AbilityToGrant : MonsterAITemplate.GrantedGameplayAbilities)
		{
			if (!IsValid(AbilityToGrant.Ability))
				continue;	
			
			UMonsterGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UMonsterGameplayAbility>();

			FGameplayAbilitySpec AbilitySpec(AbilityCDO, 1);
			AbilitySpec.SourceObject = this;
			AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.GameplayTag);

			ASC->GiveAbility(AbilitySpec);
		}
	}

	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &ThisClass::HandleHealthChanged);
	}
}

void AMonsterAIBase::HandleHealthChanged(UHealthComponent* InHealthComponent, float OldValue, float NewValue, AActor* DamageInstigator)
{
	if (DamageInstigator == nullptr)
		return;

	APlayerState* PS = Cast<APlayerState>(DamageInstigator);
	if (PS == nullptr)
		return;

	APawn* InstigatorPawn = PS->GetPawn();
	if (InstigatorPawn == nullptr)
		return;

	float DamageAmount = OldValue - NewValue;
	UAISense_Damage::ReportDamageEvent(
		GetWorld(),
		this,
		InstigatorPawn,
		DamageAmount,
		InstigatorPawn->GetActorLocation(),
		GetActorLocation()
	);
}


void AMonsterAIBase::OnDeath()
{
	if (HasAuthority())
	{
		if (WeaponActor)
		{
			WeaponActor->Destroy();
		}

		///* 몬스터 아이템 드랍 */
		if (ALootManagerActor* LootManager = ALootManagerActor::GetLootManager(this))
		{
			FMonsterDiedMessage Msg;
			Msg.MonsterID = FName("Test");
			Msg.DeathLocation = GetActorLocation();
			
			ILootableInterface::Execute_NotifyMonsterDied(LootManager, Msg);
		
		}
	}
	
	DisableMovementAndCollision();
}

void AMonsterAIBase::DisableMovementAndCollision()
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->UnPossess();
	}

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
		Capsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		Capsule->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);
	}

	if (USkeletalMeshComponent* SkeletalMesh = GetMesh())
	{
		SkeletalMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	if (UCharacterMovementComponent* MonsterAIMovement = GetCharacterMovement())
	{
		MonsterAIMovement->Velocity = FVector(0.f, 0.f, MonsterAIMovement->Velocity.Z);
		MonsterAIMovement->UpdateComponentVelocity();
	}
}
