// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbility/EmberGA_PickupItem.h"
#include "Character/EmberCharacter.h"

UEmberGA_PickupItem::UEmberGA_PickupItem()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEmberGA_PickupItem::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (AEmberCharacter* EmberCharacter = Cast<AEmberCharacter>(ActorInfo->AvatarActor.Get()))
    {
        EmberCharacter->PickupItem();
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}