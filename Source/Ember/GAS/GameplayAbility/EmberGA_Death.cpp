// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbility/EmberGA_Death.h"

#include "Weapon/EmberWeaponBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Component/CustomMoveComponent.h"
#include "Component/MontageComponent.h"
#include "Component/WeaponComponent.h"
#include "GameFramework/Character.h"
#include "Utility/CLog.h"

class UAbilityTask_PlayMontageAndWait;

UEmberGA_Death::UEmberGA_Death()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEmberGA_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	DebugLogW("Death");
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
	FMontageData* data = MontageComponent->GetDeadData();
	if (data == nullptr)
		return;
	UAbilityTask_PlayMontageAndWait* death = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Death"), data->Montage, data->PlayRate);
	death->OnCompleted.AddDynamic(this, &UEmberGA_Death::OnCompleteCallback);
	death->ReadyForActivation();
}

void UEmberGA_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	DebugLogW("Death");
	ACharacter* character = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (character == nullptr)
		return;
	UAbilitySystemComponent* asc = ActorInfo->AbilitySystemComponent.Get();
	if (asc == nullptr)
		return;
	UWeaponComponent* weapon = Cast<UWeaponComponent>(character->GetComponentByClass(UWeaponComponent::StaticClass()));
	if (weapon != nullptr)
		if(weapon->GetWeapon() != nullptr)
			weapon->GetWeapon()->Destroy();
	
	character->Destroy();
}

void UEmberGA_Death::OnCompleteCallback()
{
	DebugLogE("complete");
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
