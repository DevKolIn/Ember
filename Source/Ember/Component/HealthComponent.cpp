// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "Utility/CLog.h"
#include "Utility/EmberGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "ALootManagerActor.h"
#include "GAS/Attribute/EmberAS_Player.h"

UHealthComponent::UHealthComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	ASC = nullptr;
}

void UHealthComponent::InitializeWithAbilitySystem(UAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (ASC)
	{
		CLog::Print("UHealthComponent : Cannot initialize health component");
		return;
	}

	ASC = InASC;
	if (ASC == nullptr)
	{
		CLog::Print("UHealthComponent : Cannot initialize health component / InASC is NULL");
		return;
	}
	
	HealthSet = ASC->GetSet<UEmberAS_Player>();
	if (HealthSet == nullptr)
	{
		CLog::Print("UHealthComponent : Cannot initialize health component / HealthSet is NULL");
		return;
	}

	if (Owner->HasAuthority())
	{
		HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);
		HealthSet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	}
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHealthComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
#if WITH_SERVER_CODE
	if (ASC && DamageEffectSpec)
	{
		{
			FGameplayEventData Payload;
			Payload.EventTag = EmberGameplayTags::GameplayEvent_Death;
			Payload.Instigator = DamageInstigator;
			Payload.Target = ASC->GetAvatarActor();
			Payload.EventMagnitude = DamageMagnitude;
			
			ASC->HandleGameplayEvent(Payload.EventTag, &Payload);
		}
	}

	StartDeath();
#endif
}

void UHealthComponent::HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser,
	const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void UHealthComponent::StartDeath_Implementation()
{
	OnDeath.ExecuteIfBound();
}
