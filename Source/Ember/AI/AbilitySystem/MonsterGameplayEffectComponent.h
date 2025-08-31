// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectComponents/AbilitiesGameplayEffectComponent.h"
#include "MonsterGameplayEffectComponent.generated.h"

UENUM()
enum class EGameplayEffectActivatedAbilityCancelPolicy : uint8
{
	DoNothing,
	CancelOnEffectEnd,
};

USTRUCT()
struct FGameplayAbilityActivateConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AbilityEventTag;

	UPROPERTY(EditDefaultsOnly)
	EGameplayEffectActivatedAbilityCancelPolicy CancelPolicy = EGameplayEffectActivatedAbilityCancelPolicy::DoNothing;
};

UCLASS(DisplayName="Activate Gameplay Abilities by Event")
class EMBER_API UMonsterGameplayEffectComponent : public UAbilitiesGameplayEffectComponent
{
	GENERATED_BODY()

protected:
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="ActiveAbilities")
	TArray<FGameplayAbilityActivateConfig> ActivateAbilityConfigs;
};
