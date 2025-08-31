// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GAS/EmberAbilitySystemComponent.h"
#include "EmberPlayerState.generated.h"

class UEmberAS_Player;

UCLASS()
class EMBER_API AEmberPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	FORCEINLINE TObjectPtr<UEmberAS_Player> GetAttributeSet() { return AttributeSet; }
public:
	AEmberPlayerState();
	virtual UEmberAbilitySystemComponent* GetAbilitySystemComponent() const;

protected:
	UPROPERTY(EditAnywhere, Category="GAS")
	TObjectPtr<UEmberAbilitySystemComponent> ASC;
	UPROPERTY(EditAnywhere, Category="GAS")
	TObjectPtr<UEmberAS_Player> AttributeSet;
};
