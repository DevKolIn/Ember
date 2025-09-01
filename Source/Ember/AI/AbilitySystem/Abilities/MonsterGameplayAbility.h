// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MonsterGameplayAbility.generated.h"

class AMonsterAIBase;
class AAIController;

UENUM(BlueprintType)
enum class ELyraAbilityActivationPolicy : uint8
{
	Manual,
	
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};

UCLASS()
class EMBER_API UMonsterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMonsterGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
public:
	UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
	AMonsterAIBase* GetMonsterAIFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
	AAIController* GetMonsterAIControllerFromActorInfo() const;

protected:
	//~UGameplayAbility interface
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	//~End of UGameplayAbility interface

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ELyraAbilityActivationPolicy ActivationPolicy;
};
