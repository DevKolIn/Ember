// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/AbilitySystem/Abilities/MonsterGameplayAbility.h"
#include "GameplayAbility_Phase.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UGameplayAbility_Phase : public UMonsterGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_Phase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnPhaseTriggered(bool bMatchesComparison, float CurrentValue);	
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Threshold")
	FGameplayAttribute TargetAttribute;

	UPROPERTY(EditDefaultsOnly, Category="Threshold")
	FGameplayAttribute BaseAttribute;
	
	UPROPERTY(EditDefaultsOnly, Category="Threshold")
	float ThresholdRatio;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TSubclassOf<UGameplayEffect> PhaseEffectClass;
};
