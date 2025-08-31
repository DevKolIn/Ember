// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIMovementComponent.h"

#include "AbilitySystemInterface.h"
#include "GAS/Attribute/EmberAS_Player.h"


// Sets default values for this component's properties
UMonsterAIMovementComponent::UMonsterAIMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMonsterAIMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	AActor* Owner = GetOwner();
	check(Owner);

	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Owner);
	if (ASI == nullptr)
	{
		return;
	}

	UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
	if (ASC == nullptr)
	{
		return;
	}

	const UEmberAS_Player* AttributeSet = ASC->GetSet<UEmberAS_Player>();
	if (AttributeSet == nullptr)
	{
		return;
	}

	//AttributeSet->OnMoveSpeedChanged.AddUObject(this, &ThisClass::);
}

void UMonsterAIMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}
