// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HealthComponent.generated.h"

class UAbilitySystemComponent;
class UEmberAS_Player;
struct FGameplayEffectSpec;


DECLARE_DELEGATE(FDeathEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FLyraHealth_AttributeChanged, UHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EMBER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable)
	void InitializeWithAbilitySystem(UAbilitySystemComponent* InASC);

protected:
	virtual void BeginPlay() override;

	virtual void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	virtual void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

	UFUNCTION(NetMulticast, Reliable)
	void StartDeath();
	
public:
	FLyraHealth_AttributeChanged OnHealthChanged;
	
	FDeathEvent OnDeath;
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<const UEmberAS_Player> HealthSet;
};
