// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/AbilitySystem/Abilities/MonsterGameplayAbility.h"
#include "GameplayAbility_HitReact.generated.h"

UCLASS()
class EMBER_API UGameplayAbility_HitReact : public UMonsterGameplayAbility
{
	GENERATED_BODY()

	UGameplayAbility_HitReact(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
private:
	UFUNCTION()
	void OnNetSync();

	UFUNCTION()
	void OnMontageFinished();
};
