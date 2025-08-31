// Fill out your copyright notice in the Description page of Project Settings.

#include "EmberBaseCharacter.h"

#include "Component/HealthComponent.h"
#include "GAS/EmberAbilitySystemComponent.h"

// Sets default values
AEmberBaseCharacter::AEmberBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

UEmberAbilitySystemComponent* AEmberBaseCharacter::GetEmberAbilitySystemComponent() const
{
	return ASC;
}

UAbilitySystemComponent* AEmberBaseCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

// Called when the game starts or when spawned
void AEmberBaseCharacter::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("EmberBaseCharacter BeginPlay START - ASC: %s"),
		ASC ? TEXT("Valid") : TEXT("Null"));
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("EmberBaseCharacter BeginPlay END - ASC: %s"),
		ASC ? TEXT("Valid") : TEXT("Null"));
	if (HealthComponent)
	{
		HealthComponent->OnDeath.BindUObject(this, &ThisClass::OnDeath);
	}
}

void AEmberBaseCharacter::OnDeath()
{
}

void AEmberBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	HealthComponent->InitializeWithAbilitySystem(ASC);
}
