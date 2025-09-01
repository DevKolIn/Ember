// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "EmberTA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API AEmberTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:
	FORCEINLINE void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; }

public:
	AEmberTA_Trace();
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

private:
	bool bShowDebug{};
};
