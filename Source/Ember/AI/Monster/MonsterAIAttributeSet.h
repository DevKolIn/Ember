// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "MonsterAIAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)				\
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)	\
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)				\
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)				\
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class EMBER_API UMonsterAIAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UMonsterAIAttributeSet();
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ATTRIBUTE_ACCESSORS(ThisClass, Health);
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth);
	
protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	
private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData MaxHealth;
};
