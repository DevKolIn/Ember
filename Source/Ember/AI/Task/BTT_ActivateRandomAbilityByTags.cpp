// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ActivateRandomAbilityByTags.h"

#include "GAS/EmberAbilitySystemComponent.h"
#include "AI/Monster/MonsterAIBase.h"

EBTNodeResult::Type UBTT_ActivateRandomAbilityByTags::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (ActorOwner == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	if (UEmberAbilitySystemComponent* ASC = Cast<UEmberAbilitySystemComponent>(ActorOwner->GetAbilitySystemComponent()))
	{
		if (GameplayTags.IsEmpty() == false)
		{
			int32 SelectedIndex = FMath::RandRange(0, GameplayTags.Num() - 1);
			
			if (ASC->TryActivateAbilityByTag(GameplayTags[SelectedIndex]) == false)
			{
				return EBTNodeResult::Failed;
			}
		}
	}
	
	return EBTNodeResult::Succeeded;
}
