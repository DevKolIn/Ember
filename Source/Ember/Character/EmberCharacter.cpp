// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberCharacter.h"

#include "AI/MonsterAIController.h"
#include "Item/BaseItem.h"
#include "Item/RuneItem.h"
#include "assert.h"
#include "Utility/CHelpers.h"
#include "Utility/CLog.h"
#include "AbilitySystemComponent.h"
#include "EmberPlayerController.h"
#include "EmberPlayerState.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/CustomCameraComponent.h"
#include "Component/CustomMoveComponent.h"
#include "Component/MontageComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Component/WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Attribute/EmberAS_Player.h"
#include "Item/Drop/PickupItemActor.h"
#include "Utility/EmberGameplayTags.h"

// Sets default values
AEmberCharacter::AEmberCharacter()
{

	ASC == nullptr;
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &SpringArm, "SpringArm", RootComponent);
	SpringArm->TargetArmLength = 300.f; // ī�޶� �Ÿ�
	SpringArm->bUsePawnControlRotation = true; // ���콺�� ȸ��
	SpringArm->bDoCollisionTest = false;

	CHelpers::CreateComponent(this, &Camera, "Camera", SpringArm);
	Camera->bUsePawnControlRotation = false; // ī�޶�� �������Ͽ� ���� (���� ȸ�� X)

	CHelpers::CreateActorComponent(this, &MoveComponent, "Movement Component");
	CHelpers::CreateActorComponent(this, &CameraComponent, "Camera Component");
	CHelpers::CreateActorComponent(this, &WeaponComponent, "Weapon Component");
	CHelpers::CreateActorComponent<UMontageComponent>(this, &MontageComponent, "Montage Component");

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;// �̵� �������� ĳ���� ȸ��

	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	PickupSphere->SetupAttachment(RootComponent);
	PickupSphere->SetSphereRadius(200.f); // 원하는 감지 반경 설정
	PickupSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	PickupSphere->SetGenerateOverlapEvents(true);
	PickupSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupSphere->SetCollisionObjectType(ECC_WorldDynamic);
	PickupSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap); // 추가
	PickupSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // 필요 시 무시

	QuickSlotComponent = CreateDefaultSubobject<UQuickSlotComponent>(TEXT("QuickSlotComponent"));

	RuneSystem = CreateDefaultSubobject<URuneSystemComponent>(TEXT("RuneSystem"));
}
void AEmberCharacter::BeginPlay()
{
	TemperatureLeve = 1.0f;
	Super::BeginPlay();
	MoveComponent->OnWalk();
}

void AEmberCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// (디버그 콘솔/로그 등)
	if (AEmberPlayerState* PS = GetPlayerState<AEmberPlayerState>())
	{
		ASC = PS->GetAbilitySystemComponent();
		if (!ASC)
		{
			DebugLogE("ASC is null in PossessedBy");
			return;
		}

		ASC->InitAbilityActorInfo(PS, this);

		// 입력 어빌리티/게임 어빌리티 부여
		for (const TSubclassOf<UGameplayAbility>& InputAbility : InputAbilities)
		{
			FGameplayAbilitySpec Spec(InputAbility);
			ASC->GiveAbility(Spec);
		}
		for (const auto& GA : GameAbilities)
		{
			FGameplayAbilitySpec Spec(GA.Value);
			Spec.InputID = GA.Key;
			ASC->GiveAbility(Spec);
		}

		SetupGASInputComponent();

		if (UEmberAS_Player* Attr = Cast<UEmberAS_Player>(PS->GetAttributeSet()))
		{
			Attr->OnOutOfHealth.AddUObject(this, &AEmberCharacter::Dead);
		}
	}
}
void AEmberCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AEmberPlayerState* PS = GetPlayerState<AEmberPlayerState>())
	{
		ASC = PS->GetAbilitySystemComponent();
		if (!ASC)
		{
			DebugLogE("ASC is null in OnRep_PlayerState");
			return;
		}

		ASC->InitAbilityActorInfo(PS, this);

		// 클라에서도 입력 바인딩/어빌리티 핸드오프가 필요하면(프로젝트 정책에 따라):
		SetupGASInputComponent();

		if (UEmberAS_Player* Attr = Cast<UEmberAS_Player>(PS->GetAttributeSet()))
		{
			Attr->OnOutOfHealth.AddUObject(this, &AEmberCharacter::Dead);
		}
	}
}

FGenericTeamId AEmberCharacter::GetGenericTeamId() const
{
	return FGenericTeamId((uint8)EGameTeamID::Team1);
}

void AEmberCharacter::Dead(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec,
	float DamageMagnitude, float OldValue, float NewValue)
{
	FGameplayEventData Payload;
	Payload.EventTag = EmberGameplayTags::GameplayEvent_Death;
	Payload.Instigator = DamageInstigator;
	Payload.Target = ASC->GetAvatarActor();
	Payload.EventMagnitude = DamageMagnitude;

	ASC->HandleGameplayEvent(Payload.EventTag, &Payload);
}

void AEmberCharacter::SetIgnoreCollision(bool bIgnore)
{

}

void AEmberCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEmberCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput)
	{
		DebugLogE("Enhanced Input is null");
		return;
	}

	PlayerController = Cast<AEmberPlayerController>(GetController());
	if (!PlayerController)
	{
		DebugLogE("PlayerController is null");
		return;
	}

	// 일반 액션 바인딩
	EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, MoveComponent.Get(), &UCustomMoveComponent::Move);
	EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, CameraComponent.Get(), &UCustomCameraComponent::Look);
	EnhancedInput->BindAction(PlayerController->Quick1Action, ETriggerEvent::Started, this, &AEmberCharacter::UseQuickSlot1);
	EnhancedInput->BindAction(PlayerController->Quick2Action, ETriggerEvent::Started, this, &AEmberCharacter::UseQuickSlot2);
	EnhancedInput->BindAction(PlayerController->Quick3Action, ETriggerEvent::Started, this, &AEmberCharacter::UseQuickSlot3);
	EnhancedInput->BindAction(PlayerController->Quick4Action, ETriggerEvent::Started, this, &AEmberCharacter::UseQuickSlot4);
	EnhancedInput->BindAction(PlayerController->Quick5Action, ETriggerEvent::Started, this, &AEmberCharacter::UseQuickSlot5);
	//  여기에서 F키(PickupItem) 바인딩
	EnhancedInput->BindAction(PlayerController->InteractAction, ETriggerEvent::Started, this, &AEmberCharacter::PickupItem);

	SetupGASInputComponent();
}
void AEmberCharacter::UseQuickSlot1() { UseQuickSlot(0); }
void AEmberCharacter::UseQuickSlot2() { UseQuickSlot(1); }
void AEmberCharacter::UseQuickSlot3() { UseQuickSlot(2); }
void AEmberCharacter::UseQuickSlot4() { UseQuickSlot(3); }
void AEmberCharacter::UseQuickSlot5() { UseQuickSlot(4); }

void AEmberCharacter::UseQuickSlot(int32 Index)
{
	if (!QuickSlotComponent) return;

	if (HasAuthority())
	{
		QuickSlotComponent->UseQuickSlot(Index);          // ✅ 네가 만든 함수 재사용
	}
	else
	{
		Server_UseQuickSlot(Index);                       // ✅ 서버로 권한 위임
	}
}

void AEmberCharacter::Server_UseQuickSlot_Implementation(int32 Index)
{
	if (QuickSlotComponent)
		QuickSlotComponent->UseQuickSlot(Index);          // ✅ 서버에서 실행
}

void AEmberCharacter::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(InputComponent);
		if (input == nullptr)
		{
			DebugLogE("input is null");
			return;
		}
		input->BindAction(PlayerController.Get()->JumpAction, ETriggerEvent::Triggered, this, &AEmberCharacter::GASInputPressed, 0);
		input->BindAction(PlayerController.Get()->JumpAction, ETriggerEvent::Completed, this, &AEmberCharacter::GASInputReleased, 0);
		input->BindAction(PlayerController.Get()->SprintAction, ETriggerEvent::Triggered, this, &AEmberCharacter::GASInputPressed, 1);
		input->BindAction(PlayerController.Get()->SprintAction, ETriggerEvent::Completed, this, &AEmberCharacter::GASInputReleased, 1);
		input->BindAction(PlayerController.Get()->AttackAction, ETriggerEvent::Triggered, this, &AEmberCharacter::GASInputPressed, 2);
		input->BindAction(PlayerController.Get()->Avoid, ETriggerEvent::Triggered, this, &AEmberCharacter::GASInputPressed, 3);
		input->BindAction(PlayerController.Get()->JumpAction, ETriggerEvent::Completed, this, &AEmberCharacter::GASInputReleased, 3);
	}
}

void AEmberCharacter::GASInputPressed(int32 Input)
{
	UE_LOG(LogTemp, Warning, TEXT("=== GASInputPressed Called, Input: %d ==="), Input);

	FGameplayAbilitySpec* spec = ASC->FindAbilitySpecFromInputID(Input);
	if (spec != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spec Found, IsActive: %s"), spec->IsActive() ? TEXT("True") : TEXT("False"));
		UE_LOG(LogTemp, Warning, TEXT("InputPressed was: %s"), spec->InputPressed ? TEXT("True") : TEXT("False"));

		spec->InputPressed = true;
		if (spec->IsActive() == true)
		{
			UE_LOG(LogTemp, Warning, TEXT("Calling AbilitySpecInputPressed"));
			ASC->AbilitySpecInputPressed(*spec);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Calling TryActivateAbility"));
			ASC->TryActivateAbility(spec->Handle);
		}
	}
}

void AEmberCharacter::GASInputReleased(int32 Input)
{
	FGameplayAbilitySpec* spec = ASC->FindAbilitySpecFromInputID(Input);
	if (spec == nullptr)
	{
		DebugLogE("spec is null");
		return;
	}

	spec->InputPressed = false;
	if (spec->IsActive() == true)
		ASC->AbilityLocalInputReleased(Input);
}

void AEmberCharacter::DamageTemperature()
{
	Count++;
	if (MaxCount == Count)
	{
		TemperatureLeve++;
		FMath::Clamp(TemperatureLeve, 1, 3);
		Count = 0;
	}

	FGameplayEffectContextHandle contextHandle = ASC->MakeEffectContext();
	if (contextHandle.IsValid() == false)
	{
		DebugLogE("contexHandle is not found");
		return;
	}
	contextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle specHandle = ASC->MakeOutgoingSpec(GETemperature, TemperatureLeve, contextHandle);
	if (specHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(specHandle);
	}
}

UAbilitySystemComponent* AEmberCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void AEmberCharacter::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack triggered!"));
}

//void AEmberCharacter::PickupItem()
//{
//	FVector Start = Camera->GetComponentLocation() + Camera->GetForwardVector() * 30.f;
//	FRotator ControlRot = GetControlRotation();
//	FVector Direction = ControlRot.Vector();
//	float Distance = InteractDistance;
//	FVector End = Start + Direction * Distance;
//	float ActualDist = FVector::Distance(Start, End);
//
//	// 디버그 로그
//	UE_LOG(LogTemp, Warning, TEXT("==== PickupItem Debug ===="));
//	UE_LOG(LogTemp, Warning, TEXT("Start         : %s"), *Start.ToString());
//	UE_LOG(LogTemp, Warning, TEXT("End           : %s"), *End.ToString());
//	UE_LOG(LogTemp, Warning, TEXT("Direction     : %s"), *Direction.ToString());
//	UE_LOG(LogTemp, Warning, TEXT("Control Rot   : %s"), *ControlRot.ToString());
//	UE_LOG(LogTemp, Warning, TEXT("Actual Length : %.2f"), ActualDist);
//
//	// 시각적 디버그
//	DrawDebugDirectionalArrow(GetWorld(), Start, End, 150.0f, FColor::Red, false, 5.0f, 0, 3.0f);
//	DrawDebugSphere(GetWorld(), Start, 10.f, 12, FColor::Green, false, 5.0f);
//	DrawDebugSphere(GetWorld(), End, 10.f, 12, FColor::Blue, false, 5.0f);
//
//	// 라인트레이스
//	FHitResult HitResult;
//	FCollisionQueryParams Params;
//	Params.AddIgnoredActor(this);
//	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
//	{
//		AActor* HitActor = HitResult.GetActor();
//		UE_LOG(LogTemp, Warning, TEXT("LineTrace HIT!"));
//		if (HitActor)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("Hit Actor     : %s"), *HitActor->GetName());
//		}
//		else
//		{
//			UE_LOG(LogTemp, Warning, TEXT("Hit Actor     : nullptr"));
//		}
//		UE_LOG(LogTemp, Warning, TEXT("Impact Point  : %s"), *HitResult.ImpactPoint.ToString());
//		UE_LOG(LogTemp, Warning, TEXT("Impact Normal : %s"), *HitResult.ImpactNormal.ToString());
//		UE_LOG(LogTemp, Warning, TEXT("Hit Bone Name : %s"), *HitResult.BoneName.ToString());
//
//		// 룬인지 먼저 체크
//		if (ARuneItem* Rune = Cast<ARuneItem>(HitActor))
//		{
//			UE_LOG(LogTemp, Warning, TEXT("==> Rune Interacted: %s"), *Rune->GetName());
//			Rune->Use(this); // 내부에서 TryEquipRune 호출
//		}
//		// 일반 아이템
//		else if (ABaseItem* Item = Cast<ABaseItem>(HitActor))
//		{
//			UE_LOG(LogTemp, Warning, TEXT("==> Base Item Interacted: %s"), *Item->GetName());
//			Item->Use(this);
//		}
//		else if (APickupItemActor* Pickup = Cast<APickupItemActor>(HitActor))
//		{
//			UE_LOG(LogTemp, Warning, TEXT("==> PickupItemActor Interacted: %s"), *Pickup->GetName());
//			Pickup->OnPickedUp(this); // 캐릭터 넘겨서 처리
//		}
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("LineTrace MISS — nothing hit."));
//	}
//}
void AEmberCharacter::Server_PickupItem_Implementation(APickupItemActor* TargetItem)
{
	if (TargetItem && HasAuthority())
	{
		TargetItem->OnPickedUp(this); // 서버에서만 실행
	}
}
void AEmberCharacter::Server_RequestInteraction_Implementation(UInteractionComponent* TargetInteraction)
{
	if (TargetInteraction)
	{
		TargetInteraction->Interact(this); // 서버에서 다시 실행
	}
}

void AEmberCharacter::PickupItem()
{
	DrawDebugSphere(GetWorld(), PickupSphere->GetComponentLocation(), PickupSphere->GetScaledSphereRadius(), 32, FColor::Green, false, 5.f);

	if (OverlappingItems.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("PickupItem - No overlapping items."));
		return;
	}

	APickupItemActor* FocusedItem = nullptr;
	float BestDot = -1.f;

	const FVector ViewLocation = Camera->GetComponentLocation();
	const FVector ViewDirection = Camera->GetForwardVector();

	for (APickupItemActor* Item : OverlappingItems)
	{
		if (!IsValid(Item)) continue;

		const FVector ToItem = (Item->GetActorLocation() - ViewLocation).GetSafeNormal();
		const float Dot = FVector::DotProduct(ViewDirection, ToItem);

		if (Dot > BestDot)
		{
			BestDot = Dot;
			FocusedItem = Item;
		}
	}

	if (FocusedItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Requesting pickup of item: %s"), *FocusedItem->GetName());

		// ✅ 서버에 요청
		Server_PickupItem(FocusedItem);

		// ✅ 로컬에서 UI, 사운드 등 처리
		OverlappingItems.Remove(FocusedItem);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PickupItem - No item in focus."));
	}
}


bool AEmberCharacter::TryEquipRune(const URuneItemTemplate* Template, int32 PreferredSlotIndex)
{
	UE_LOG(LogTemp, Log, TEXT("[TryEquipRune] HasAuth=%d Template=%s PrefSlot=%d"),
		HasAuthority(), Template ? *Template->GetName() : TEXT("NULL"), PreferredSlotIndex);

	if (!Template) return false;

	if (!HasAuthority())
	{
		Server_TryEquipRune(Template, PreferredSlotIndex);
		return false; // 서버 확정 전에는 성공 처리 금지
	}

	URuneSystemComponent* RuneComp = FindComponentByClass<URuneSystemComponent>();
	if (!RuneComp) return false;

	// 서버 직접 호출 경로(싱글/리스닝 등)
	UAbilitySystemComponent* ASC_Local = GetAbilitySystemComponent();
	if (!ASC_Local) return false;

	const float Old_AD = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetAttackDamageAttribute());
	const float Old_MaxAD = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetMaxAttackDamageAttribute());
	const float Old_Range = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetAttackRangeAttribute());
	const float Old_Radius = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetAttackRadiusAttribute());
	const float Old_Meta = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetMetaDamageAttribute());

	int32 ResolvedSlot = INDEX_NONE;
	const bool bEquipped = RuneComp->EquipRuneFromTemplate(Template, PreferredSlotIndex, /*out*/ResolvedSlot);
	UE_LOG(LogTemp, Warning, TEXT("[Server] EquipRuneFromTemplate result=%d (Slot=%d)"), bEquipped, ResolvedSlot);
	if (!bEquipped) return false;

	const float New_AD = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetAttackDamageAttribute());
	const float New_MaxAD = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetMaxAttackDamageAttribute());
	const float New_Range = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetAttackRangeAttribute());
	const float New_Radius = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetAttackRadiusAttribute());
	const float New_Meta = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetMetaDamageAttribute());

	const float dAD = New_AD - Old_AD;
	const float dMaxAD = New_MaxAD - Old_MaxAD;
	const float dRange = New_Range - Old_Range;
	const float dRadius = New_Radius - Old_Radius;
	const float dMeta = New_Meta - Old_Meta;

	Multicast_OnRuneEquippedDetailed(
		Template->RuneName, ResolvedSlot,
		New_AD, dAD,
		New_MaxAD, dMaxAD,
		New_Range, dRange,
		New_Radius, dRadius,
		New_Meta, dMeta
	);
	return true;
}

void AEmberCharacter::Server_TryEquipRune_Implementation(const URuneItemTemplate* Template, int32 PreferredSlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("[RPC] Server_TryEquipRune_Implementation Template=%s PrefSlot=%d"),
		Template ? *Template->GetName() : TEXT("NULL"), PreferredSlotIndex);

	if (!Template) return;

	URuneSystemComponent* RuneComp = FindComponentByClass<URuneSystemComponent>();
	UAbilitySystemComponent* ASC_Local = GetAbilitySystemComponent();
	if (!RuneComp || !ASC_Local) return;

	const float Old_AD = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetAttackDamageAttribute());
	const float Old_MaxAD = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetMaxAttackDamageAttribute());
	const float Old_Range = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetAttackRangeAttribute());
	const float Old_Radius = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetAttackRadiusAttribute());
	const float Old_Meta = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetMetaDamageAttribute());

	int32 ResolvedSlot = INDEX_NONE;
	const bool bEquipped = RuneComp->EquipRuneFromTemplate(Template, PreferredSlotIndex, /*out*/ResolvedSlot);
	UE_LOG(LogTemp, Warning, TEXT("[Server] EquipRuneFromTemplate result=%d (Slot=%d)"), bEquipped, ResolvedSlot);
	if (!bEquipped) return;

	const float New_AD = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetAttackDamageAttribute());
	const float New_MaxAD = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetMaxAttackDamageAttribute());
	const float New_Range = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetAttackRangeAttribute());
	const float New_Radius = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetAttackRadiusAttribute());
	const float New_Meta = ASC_Local->GetNumericAttribute(UEmberAS_Player::GetMetaDamageAttribute());

	const float dAD = New_AD - Old_AD;
	const float dMaxAD = New_MaxAD - Old_MaxAD;
	const float dRange = New_Range - Old_Range;
	const float dRadius = New_Radius - Old_Radius;
	const float dMeta = New_Meta - Old_Meta;

	Multicast_OnRuneEquippedDetailed(
		Template->RuneName, ResolvedSlot,
		New_AD, dAD,
		New_MaxAD, dMaxAD,
		New_Range, dRange,
		New_Radius, dRadius,
		New_Meta, dMeta
	);
}
void AEmberCharacter::Multicast_OnRuneEquippedDetailed_Implementation(
	const FText& RuneName, int32 SlotIndex,
	float New_AD, float dAD,
	float New_MaxAD, float dMaxAD,
	float New_Range, float dRange,
	float New_Radius, float dRadius,
	float New_Meta, float dMeta)
{
	UE_LOG(LogTemp, Warning, TEXT("[Rune Equipped] %s | Slot=%d"), *RuneName.ToString(), SlotIndex);
	UE_LOG(LogTemp, Warning, TEXT("  - AttackDamage     : %+0.1f (Now: %.1f)"), dAD, New_AD);
	UE_LOG(LogTemp, Warning, TEXT("  - MaxAttackDamage  : %+0.1f (Now: %.1f)"), dMaxAD, New_MaxAD);
	UE_LOG(LogTemp, Warning, TEXT("  - AttackRange      : %+0.1f (Now: %.1f)"), dRange, New_Range);
	UE_LOG(LogTemp, Warning, TEXT("  - AttackRadius     : %+0.1f (Now: %.1f)"), dRadius, New_Radius);
	UE_LOG(LogTemp, Warning, TEXT("  - MetaDamage       : %+0.1f (Now: %.1f)"), dMeta, New_Meta);
}
void AEmberCharacter::Multicast_OnRuneEquipped_Implementation(const URuneItemTemplate* Template, int32 SlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("[Multicast] Equipped confirmed: %s (Slot %d)"),
		Template ? *Template->GetName() : TEXT("NULL"), SlotIndex);

	// 여기에서 UI 갱신, 소모/파괴 처리(픽업 액터가 있다면) 등 확정 후 처리를 하세요.
	// ex) OnRuneEquipped 델리게이트 브로드캐스트, 위젯 업데이트 등
}
bool AEmberCharacter::TryEquipRuneAuto(const URuneItemTemplate* Template)
{
	return TryEquipRune(Template, -1);
}
void AEmberCharacter::ShowRuneComparisonUI(const URuneItemTemplate* NewRuneTemplate)
{
	if (!NewRuneTemplate) return;

	// 컴포넌트 핸들 확보
	URuneSystemComponent* RS = (RuneSystem.Get() != nullptr)
		? RuneSystem.Get()
		: FindComponentByClass<URuneSystemComponent>();

	if (!RS) return;

	// 템플릿 원시값 -> FRuneStat 조립
	FRuneStat NewStat;
	NewStat.Power = NewRuneTemplate->BasePower;
	NewStat.CooldownReduction = NewRuneTemplate->BaseCooldownReduction;
	NewStat.Element = NewRuneTemplate->BaseElement;

	// 비교할 슬롯(필요하면 바꾸세요)
	const int32 SlotIndex = 0;
	const FRuneStat CurrentStat = RS->GetRuneStatAtSlot(SlotIndex);

	// 로그 출력
	UE_LOG(LogTemp, Log, TEXT("현재 룬[%d] - Power: %.1f, CDR: %.1f, Element: %s"),
		SlotIndex, CurrentStat.Power, CurrentStat.CooldownReduction, *CurrentStat.Element);

	UE_LOG(LogTemp, Log, TEXT("새 룬     - Power: %.1f, CDR: %.1f, Element: %s"),
		NewStat.Power, NewStat.CooldownReduction, *NewStat.Element);

	// 차이도 보여주고 싶으면:
	const float dP = NewStat.Power - CurrentStat.Power;
	const float dCDR = NewStat.CooldownReduction - CurrentStat.CooldownReduction;
	UE_LOG(LogTemp, Log, TEXT("변화량    - ΔPower: %+0.1f, ΔCDR: %+0.1f"), dP, dCDR);

	// TODO: 위 값들을 위젯에 전달하여 UI 갱신
}


UQuickSlotComponent* AEmberCharacter::GetQuickSlotComponent() const
{
	return QuickSlotComponent;
}

void AEmberCharacter::AddOverlappingItem(APickupItemActor* Item)
{
	if (!OverlappingItems.Contains(Item))
	{
		OverlappingItems.Add(Item);
	}
}

void AEmberCharacter::RemoveOverlappingItem(APickupItemActor* Item)
{
	OverlappingItems.Remove(Item);
}
APickupItemActor* AEmberCharacter::GetFocusedPickupItem() const
{
	if (OverlappingItems.Num() == 0) return nullptr;

	const FVector ViewLocation = Camera->GetComponentLocation();
	const FVector ViewDirection = Camera->GetForwardVector();

	APickupItemActor* ClosestItem = nullptr;
	float BestDot = -1.f;

	for (APickupItemActor* Item : OverlappingItems)
	{
		if (!IsValid(Item)) continue;

		const FVector ToItem = (Item->GetActorLocation() - ViewLocation).GetSafeNormal();
		const float Dot = FVector::DotProduct(ViewDirection, ToItem); // 카메라 정면과 얼마나 일치하는지

		if (Dot > BestDot)
		{
			BestDot = Dot;
			ClosestItem = Item;
		}
	}

	return ClosestItem;
}