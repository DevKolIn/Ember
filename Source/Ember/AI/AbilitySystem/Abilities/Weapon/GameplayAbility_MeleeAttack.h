// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayAbility_Melee.h"
#include "AI/AbilitySystem/Abilities/MonsterGameplayAbility.h"
#include "GameplayAbility_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UGameplayAbility_MeleeAttack : public UGameplayAbility_Melee
{
	GENERATED_BODY()

public:
	UGameplayAbility_MeleeAttack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnTargetDataReady(FGameplayEventData Payload);

	UFUNCTION()
	void OnMontageEventTriggered(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnMontageFinished();
	
protected:
	UPROPERTY(EditDefaultsOnly, category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, category = "Attack")
	float AttackRate = 1.0f;
};
