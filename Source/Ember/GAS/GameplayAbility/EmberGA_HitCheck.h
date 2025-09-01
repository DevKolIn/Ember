// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EmberGA_HitCheck.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UEmberGA_HitCheck : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UEmberGA_HitCheck();
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

protected:
	UPROPERTY(EditAnywhere, Category="GAS")
	TSubclassOf<UGameplayEffect> AttackDamageEffect;

	float CurrentLevel;
};
