// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_Melee.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AI/MonsterAIController.h"
#include "AI/Monster/MonsterAIBase.h"
#include "AI/Weapon/MonsterAIWeapon.h"

UGameplayAbility_Melee::UGameplayAbility_Melee(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGameplayAbility_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CachedHitActors.Reset();
}

void UGameplayAbility_Melee::ParseTargetData(const FGameplayAbilityTargetDataHandle& InTargetDataHandle,
	TArray<int32>& OutCharacterHitIndexes)
{
	for (int32 i = 0; i < InTargetDataHandle.Data.Num(); i++)
	{
		const TSharedPtr<FGameplayAbilityTargetData>& TargetData = InTargetDataHandle.Data[i];

		if (FHitResult* HitResult = const_cast<FHitResult*>(TargetData->GetHitResult()))
		{
			if (AActor* HitActor = HitResult->GetActor())
			{
				if (CachedHitActors.Contains(HitActor))
					continue;
				
				if (GetMonsterAIControllerFromActorInfo()->GetTeamAttitudeTowards(*HitActor))
				{
					CachedHitActors.Add(HitActor);
				}

				OutCharacterHitIndexes.Add(i);
			}
		}
	}
}

void UGameplayAbility_Melee::ProcessHitResult(FHitResult HitResult, float Damage, bool bBlockingHit,
	UAnimMontage* BackwardMontage, AMonsterAIWeapon* WeaponActor)
{
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (SourceASC == nullptr)
		return;
	
	if (HasAuthority(&CurrentActivationInfo))
	{
		FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(HitResult.GetActor());
		FGameplayEffectContextHandle EffectContextHandle = MakeEffectContext(CurrentSpecHandle, CurrentActorInfo);
		EffectContextHandle.AddHitResult(HitResult);
		EffectContextHandle.AddInstigator(SourceASC->AbilityActorInfo->OwnerActor.Get(), WeaponActor);

		FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(WeaponActor->GameplayEffectClass, 1.f, EffectContextHandle);
		ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
	}
}