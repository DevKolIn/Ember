// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIEliteBase.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GAS/Attribute/EmberAS_Player.h"

AMonsterAIEliteBase::AMonsterAIEliteBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMonsterAIEliteBase::HandleHealthChanged(UHealthComponent* InHealthComponent, float OldValue, float NewValue, AActor* DamageInstigator)
{
	Super::HandleHealthChanged(InHealthComponent, OldValue, NewValue, DamageInstigator);

	if (ShouldNextPhase())
	{
		NextPhase();
		bIsNextPhase = true;
	}
}

bool AMonsterAIEliteBase::ShouldNextPhase()
{
	// TODO : will fix the magic number
	return AttributeSet->GetHealthRatio() <= 50.f && bIsNextPhase == false;
}

void AMonsterAIEliteBase::NextPhase()
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsBool(FName("bIsNextPhase"), true);
		}
	}
}
