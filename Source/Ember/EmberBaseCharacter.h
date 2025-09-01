// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "EmberBaseCharacter.generated.h"

class UEmberAbilitySystemComponent;

UCLASS(Abstract)
class EMBER_API AEmberBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AEmberBaseCharacter();
	virtual UEmberAbilitySystemComponent* GetEmberAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PostInitializeComponents() override;
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	virtual void OnDeath();

protected:
	UPROPERTY(VisibleAnywhere, Category = GAS)
	TObjectPtr<UEmberAbilitySystemComponent> ASC;
	UPROPERTY()
	TObjectPtr<class UHealthComponent> HealthComponent;
};
