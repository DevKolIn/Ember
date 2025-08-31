// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbility/EmberGA_HitState.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/EmberCharacter.h"
#include "Component/CustomCameraComponent.h"
#include "Component/CustomMoveComponent.h"
#include "Component/MontageComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/MovementComponent.h"
#include "Utility/CLog.h"

UEmberGA_HitState::UEmberGA_HitState()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEmberGA_HitState::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	DebugLogW("Hit");
	ACharacter* character = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (character == nullptr)
		return;
	UAbilitySystemComponent* asc = ActorInfo->AbilitySystemComponent.Get();
	if (asc == nullptr)
		return;
	UCustomMoveComponent* move = Cast<UCustomMoveComponent>(character->GetComponentByClass(UCustomMoveComponent::StaticClass()));
	if (move != nullptr)
		move->ShouldMove(false);
	UMontageComponent* MontageComponent = Cast<UMontageComponent>(character->GetComponentByClass(UMontageComponent::StaticClass()));
	if (MontageComponent == nullptr)
	{
		return;
	}
	FMontageData* data = MontageComponent->GetHitData();
	if (data == nullptr)
		return;
	UAbilityTask_PlayMontageAndWait* hit = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Hit"), data->Montage, data->PlayRate);
	hit->OnCompleted.AddDynamic(this,&UEmberGA_HitState::OnCompleteCallback);
	hit->ReadyForActivation();
}

void UEmberGA_HitState::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ACharacter* character = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (character == nullptr)
		return;
	UAbilitySystemComponent* asc = ActorInfo->AbilitySystemComponent.Get();
	if (asc == nullptr)
		return;
	UCustomMoveComponent* move = Cast<UCustomMoveComponent>(character->GetComponentByClass(UCustomMoveComponent::StaticClass()));
	if (move != nullptr)
		move->ShouldMove(true);
}


void UEmberGA_HitState::OnCompleteCallback()
{
	DebugLogE("complete");
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}