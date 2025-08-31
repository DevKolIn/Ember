// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbility/EmberGA_EnableInvincible.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GAS/Attribute/EmberAS_Player.h"
#include "Utility/CLog.h"

UEmberGA_EnableInvincible::UEmberGA_EnableInvincible()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEmberGA_EnableInvincible::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* character = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (character == nullptr)
	{
		DebugLogE("character is null");
		return;
	}
	TArray<UActorComponent*> components;
	character->GetComponents(components);
	for (UActorComponent* comp: components)
	{
		if (UPrimitiveComponent* prim = Cast<UPrimitiveComponent>(comp))
		{
			prim->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Ignore);
			prim->SetCollisionResponseToChannel(ECC_GameTraceChannel2,ECR_Ignore);
		}
	}

	UAbilitySystemComponent* asc = GetAbilitySystemComponentFromActorInfo_Checked();
	if (asc == nullptr)
	{
		DebugLogE("asc is null");
		return;
	}
	const UEmberAS_Player* attribute = asc->GetSet<UEmberAS_Player>();
	FGameplayEffectSpecHandle handle = MakeOutgoingGameplayEffectSpec(GE);
	if (handle == nullptr)
	{
		DebugLogE("handle is null");
		return;
	}
	ApplyGameplayEffectSpecToOwner(CurrentSpecHandle,ActorInfo,ActivationInfo,handle);

	EndAbility(Handle,ActorInfo,ActivationInfo,true,false);
}
