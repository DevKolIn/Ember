// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/AbilitySystem/Abilities/MonsterGameplayAbility.h"
#include "GameplayAbility_Melee.generated.h"

class AMonsterAIWeapon;

/**
 * 
 */
UCLASS()
class EMBER_API UGameplayAbility_Melee : public UMonsterGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_Melee(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void ParseTargetData(const FGameplayAbilityTargetDataHandle& InTargetDataHandle, TArray<int32>& OutCharacterHitIndexes);

	UFUNCTION()
	void ProcessHitResult(FHitResult HitResult, float Damage, bool bBlockingHit, UAnimMontage* BackwardMontage, AMonsterAIWeapon* WeaponActor);

	
protected:
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> CachedHitActors;
};
