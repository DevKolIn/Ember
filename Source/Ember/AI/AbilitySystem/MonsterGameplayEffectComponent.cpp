// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterGameplayEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GAS/Attribute/EmberAS_Player.h"
#include "Utility/CLog.h"

void UMonsterGameplayEffectComponent::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer,
	FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	UAbilitySystemComponent* ASC = ActiveGEContainer.Owner;
	if (ensure(ASC) == false)
		return;
	
	for (const FGameplayAbilityActivateConfig& ActivateAbilityConfig : ActivateAbilityConfigs)
	{
		FGameplayEventData Payload;
		Payload.Instigator = GESpec.GetEffectContext().GetInstigator();
		Payload.ContextHandle = GESpec.GetEffectContext();

		ASC->HandleGameplayEvent(ActivateAbilityConfig.AbilityEventTag, &Payload);
	}
}

