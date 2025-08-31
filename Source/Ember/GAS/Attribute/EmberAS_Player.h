#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "EmberAS_Player.generated.h"

struct FGameplayEffectSpec;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_MULTICAST_DELEGATE_SixParams(FEmberAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec* /*EffectSpec*/, float /*EffectMagnitude*/, float /*OldValue*/, float /*NewValue*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FEmberAttributeEventTwoParams, float /*OldValue*/, float /*NewValue*/);

UCLASS()
class EMBER_API UEmberAS_Player : public UAttributeSet
{
	GENERATED_BODY()
public:
	UEmberAS_Player();

public:
	float GetHealthRatio() const;

public:
	ATTRIBUTE_ACCESSORS(UEmberAS_Player,MaxHealth);
	ATTRIBUTE_ACCESSORS(UEmberAS_Player,Health);
	ATTRIBUTE_ACCESSORS(UEmberAS_Player, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(UEmberAS_Player, AttackRange);
	ATTRIBUTE_ACCESSORS(UEmberAS_Player, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(UEmberAS_Player, AttackRadius);
	ATTRIBUTE_ACCESSORS(UEmberAS_Player, MaxAttackDamage);
	ATTRIBUTE_ACCESSORS(UEmberAS_Player, AttackDamage)
	ATTRIBUTE_ACCESSORS(UEmberAS_Player, MetaDamage)

	ATTRIBUTE_ACCESSORS(UEmberAS_Player, MoveSpeed);
	
	ATTRIBUTE_ACCESSORS(UEmberAS_Player, PlayerTemperature)
	ATTRIBUTE_ACCESSORS(UEmberAS_Player, MaxPlayerTemperature)
	ATTRIBUTE_ACCESSORS(UEmberAS_Player, MaxDamageTemperature)
	ATTRIBUTE_ACCESSORS(UEmberAS_Player, DamageTemperature)
	ATTRIBUTE_ACCESSORS(UEmberAS_Player, MetaTemperature)
	ATTRIBUTE_ACCESSORS(UEmberAS_Player, Invincible);
	

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;;

public:
	mutable FEmberAttributeEvent OnHealthChanged;
	mutable FEmberAttributeEvent OnOutOfHealth;
	mutable FEmberAttributeEventTwoParams OnMoveSpeedChanged;
	
protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

private:
	FORCEINLINE bool HasHealthChanged() const;
	FORCEINLINE bool IsDead() const;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="HP", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="HP", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, Category="Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MetaDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MoveSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Temperature", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PlayerTemperature;
	UPROPERTY(BlueprintReadOnly, Category = "Temperature", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxPlayerTemperature;
	UPROPERTY(BlueprintReadOnly, Category = "Temperature", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxDamageTemperature;
	UPROPERTY(BlueprintReadOnly, Category = "Temperature", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DamageTemperature;
	UPROPERTY(BlueprintReadOnly, Category = "Temperature", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MetaTemperature;

	UPROPERTY(BlueprintReadOnly, Category = "Temperature", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Invincible;
private:
	bool bOutOfHealth;
	float PreviousHealth;
};
