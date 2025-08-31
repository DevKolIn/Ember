// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/AbilitySystem/Abilities/MonsterGameplayAbility.h"
#include "GameplayAbility_GiantForm.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FMontageStep
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> Montage;
	
	UPROPERTY(EditAnywhere)
	float Rate = 1.0f;
};

UCLASS()
class EMBER_API UGameplayAbility_GiantForm : public UMonsterGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_GiantForm(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	void PlayNextStep();
	
	UFUNCTION()
	void OnMontageFinished();

	UFUNCTION()
	void OnMontageInterrupted();
	
protected:
	UPROPERTY(EditDefaultsOnly, category = "Animation")
	TArray<FMontageStep> MontageSteps;

private:
	int32 StepIndex = -1;
};
