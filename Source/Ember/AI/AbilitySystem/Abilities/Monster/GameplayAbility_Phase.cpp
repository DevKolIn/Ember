// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_Phase.h"

#include "AIController.h"
#include "Abilities/Tasks/AbilityTask_WaitAttributeChangeThreshold.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GAS/Attribute/EmberAS_Player.h"
#include "Utility/EmberGameplayTags.h"

UGameplayAbility_Phase::UGameplayAbility_Phase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActivationPolicy = ELyraAbilityActivationPolicy::OnSpawn;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

	AbilityTags.AddTag(EmberGameplayTags::AI_Ability_Phase);
	ActivationOwnedTags.AddTag(EmberGameplayTags::Status_Phase);
}

void UGameplayAbility_Phase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	float BaseAttributeValue = ASC->GetNumericAttribute(BaseAttribute);
	if (UAbilityTask_WaitAttributeChangeThreshold* WaitPhaseEvent = UAbilityTask_WaitAttributeChangeThreshold::WaitForAttributeChangeThreshold(this, TargetAttribute, EWaitAttributeChangeComparison::LessThanOrEqualTo, BaseAttributeValue * ThresholdRatio,  true))
	{
		WaitPhaseEvent->ReadyForActivation();
		WaitPhaseEvent->OnChange.AddDynamic(this, &ThisClass::OnPhaseTriggered);
	}
}

void UGameplayAbility_Phase::OnPhaseTriggered(bool bMatchesComparison, float CurrentValue)
{
	if (AAIController* AIController = GetMonsterAIControllerFromActorInfo())
	{
		if (UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsBool(FName("bIsNextPhase"), true);
		}
	}

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		if (PhaseEffectClass)
		{
			FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(PhaseEffectClass, /*Level*/1.f, Ctx);
			if (Spec.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
			}
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
