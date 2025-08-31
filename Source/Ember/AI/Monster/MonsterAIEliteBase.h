// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MonsterAIBase.h"
#include "MonsterAIEliteBase.generated.h"

UCLASS(Abstract, Blueprintable)
class EMBER_API AMonsterAIEliteBase : public AMonsterAIBase
{
	GENERATED_BODY()

public:
	AMonsterAIEliteBase();

protected:
	virtual void HandleHealthChanged(UHealthComponent* InHealthComponent, float OldValue, float NewValue, AActor* DamageInstigator) override;

protected:
	virtual bool ShouldNextPhase();
	virtual void NextPhase();

protected:
	bool bIsNextPhase = false;
};
