// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EmberGA_EnableInvincible.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UEmberGA_EnableInvincible : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UEmberGA_EnableInvincible();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditAnywhere, Category="GE")
	TSubclassOf<UGameplayEffect> GE;
};
