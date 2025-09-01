// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_Death.h"

#include "AbilitySystemComponent.h"
#include "Utility/EmberGameplayTags.h"

UGameplayAbility_Death::UGameplayAbility_Death(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

	AbilityTags.AddTag(EmberGameplayTags::AI_Ability_Death);
	ActivationOwnedTags.AddTag(EmberGameplayTags::Status_Death);
	
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = EmberGameplayTags::GameplayEvent_Death;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	check(ActorInfo);

	/* ASC의 모든 Ability를 비활성화 */
	if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	{
		SetCanBeCanceled(false);
		ASC->CancelAbilities(nullptr, nullptr, this);
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}, EndDelay, false);
}
