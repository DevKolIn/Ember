// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbility/EmberGA_Sprint.h"

#include "Component/CustomMoveComponent.h"
#include "GameFramework/Character.h"
#include "Utility/CHelpers.h"
#include "Utility/CLog.h"

UEmberGA_Sprint::UEmberGA_Sprint()
{
}

void UEmberGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	const TObjectPtr<ACharacter> character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	const TObjectPtr<UCustomMoveComponent> move = CHelpers::GetComponentCheck<UCustomMoveComponent>(character);
	if (character == nullptr)
		DebugLogE("character is null");
	if (move == nullptr)
		DebugLogE("move component is null");
	move->OnSprint();
}

bool UEmberGA_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
		return bResult;

	const TObjectPtr<ACharacter> character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	const TObjectPtr<UCustomMoveComponent> move = CHelpers::GetComponentCheck<UCustomMoveComponent>(character);
	if (character == nullptr)
		DebugLogE("character is null");
	if (move == nullptr)
		DebugLogE("move component is null");
	return (character != nullptr && move != nullptr);
}

void UEmberGA_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	const TObjectPtr<ACharacter> character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	const TObjectPtr<UCustomMoveComponent> move = CHelpers::GetComponent<UCustomMoveComponent>(character);
	if (character == nullptr)
	{
		DebugLogE("character is null");
		return;
	}
	if (move == nullptr)
	{
		DebugLogE("movement Component is null");
		return;
	}
	switch (PrevSpeedType)
	{
	case ESpeedType::Walk:
		move->OnWalk();
		break;
	case ESpeedType::Run:
		move->OnRun();
		break;
	}
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
