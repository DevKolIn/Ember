// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TargetActor/EmberTA_Trace.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Weapon/EmberWeaponBase.h"
#include "Abilities/GameplayAbility.h"
#include "Component/WeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GAS/Attribute/EmberAS_Player.h"
#include "Utility/CLog.h"

AEmberTA_Trace::AEmberTA_Trace()
{
}

void AEmberTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AEmberTA_Trace::ConfirmTargetingAndContinue()
{
	if (SourceActor == nullptr)
	{
		DebugLogE("SourceActor is null");
		return;	
	}

	DebugLogE(SourceActor.GetName());
	FGameplayAbilityTargetDataHandle dataHandle = MakeTargetData();
	TargetDataReadyDelegate.Broadcast(dataHandle);
}

FGameplayAbilityTargetDataHandle AEmberTA_Trace::MakeTargetData() const
{
	ACharacter* character = Cast<ACharacter>(SourceActor);
	UAbilitySystemComponent* asc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (asc == nullptr)
	{
		DebugLogE("asc is null");
		return FGameplayAbilityTargetDataHandle();
	}
	const UEmberAS_Player* attribute = asc->GetSet<UEmberAS_Player>();
	if (attribute == nullptr)
	{
		DebugLogE("attribute is null");
		return FGameplayAbilityTargetDataHandle();
	}

	FHitResult result;
	const float attackRange = attribute->GetAttackRange();
	const float attackRadius = attribute->GetAttackRadius();

	FCollisionQueryParams params(SCENE_QUERY_STAT(AEmberTA_Trace),false,character);
	FVector forward = character->GetActorForwardVector();
	FVector start = character->GetActorLocation() + forward * character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector end = start + forward * attackRange;
	bool hit = GetWorld()->SweepSingleByChannel(result,start,end,FQuat::Identity,ECC_Pawn,FCollisionShape::MakeSphere(attackRadius),params);
	FGameplayAbilityTargetDataHandle handle;

	if (hit == true)
	{
		FGameplayAbilityTargetData_SingleTargetHit* targetData = new FGameplayAbilityTargetData_SingleTargetHit(result);
		CLog::Print(result.GetActor()->GetName());
		handle.Add(targetData);
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug == true)
	{
		FVector capsule = start + (end - start) * 0.5f;
		float half = attackRange * 0.5f;
		FColor color = hit ? FColor::Green : FColor::Red;

		DrawDebugCapsule(GetWorld(),capsule,half,attackRadius,FRotationMatrix::MakeFromZ(forward).ToQuat(),color,false,5.0f);
	}
#endif

	return handle;
}
