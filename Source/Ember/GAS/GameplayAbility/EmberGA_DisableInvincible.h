// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EmberGA_DisableInvincible.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UEmberGA_DisableInvincible : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UEmberGA_DisableInvincible();
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
