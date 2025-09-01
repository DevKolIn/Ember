// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "EmberAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EMBER_API UEmberAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UEmberAbilitySystemComponent();

protected:
	virtual void BeginPlay() override;

public:
	bool TryActivateAbilityByTag(const FGameplayTag& GameplayTag);
	FGameplayAbilitySpec* FindActivateAbilityByTag(const FGameplayTag& GameplayTag);
};
