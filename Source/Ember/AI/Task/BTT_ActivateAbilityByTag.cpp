// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ActivateAbilityByTag.h"

#include "AbilitySystemComponent.h"
#include "GAS/EmberAbilitySystemComponent.h"
#include "AI/Monster/MonsterAIBase.h"

EBTNodeResult::Type UBTT_ActivateAbilityByTag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (ActorOwner == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	CachedOwnerComp = OwnerComp;

	CachedASC = Cast<UEmberAbilitySystemComponent>(ActorOwner->GetAbilitySystemComponent());
	if (UEmberAbilitySystemComponent* ASC = CachedASC.Get())
	{
		if (ASC->TryActivateAbilityByTag(GameplayTag))
		{
			ASC->OnAbilityEnded.AddUObject(this, &ThisClass::OnAbilityEnd);
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::InProgress;
}

void UBTT_ActivateAbilityByTag::OnAbilityEnd(const FAbilityEndedData& AbilityEndedData)
{
	const UGameplayAbility* Ability = AbilityEndedData.AbilityThatEnded;
	
	if (Ability->AbilityTags.HasTag(GameplayTag))
	{
		if (UEmberAbilitySystemComponent* ASC = CachedASC.Get())
		{
			ASC->OnAbilityEnded.RemoveAll(this);
		}
		
		FinishLatentTask(*CachedOwnerComp ,EBTNodeResult::Succeeded);
	}
}

void UBTT_ActivateAbilityByTag::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	if (UAbilitySystemComponent* ASC = CachedASC.Get())
	{
		if (AbilityEndHandle.IsValid())
		{
			ASC->RegisterGameplayTagEvent(GameplayTag, EGameplayTagEventType::NewOrRemoved).Remove(AbilityEndHandle);
		}
	}
}
