// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AbilitySystem/Abilities/MonsterGameplayAbility.h"
#include "GameplayAbility_Death.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UGameplayAbility_Death : public UMonsterGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_Death(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Death")
	float EndDelay = 1.5f;
};
