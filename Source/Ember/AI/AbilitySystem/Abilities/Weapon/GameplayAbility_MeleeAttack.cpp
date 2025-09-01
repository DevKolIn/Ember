// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_MeleeAttack.h"

#include "GameplayAbilitySpec.h"
#include "GameplayAbilitySpecHandle.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AI/Weapon/MonsterAIWeapon.h"
#include "Utility/EmberGameplayTags.h"

UGameplayAbility_MeleeAttack::UGameplayAbility_MeleeAttack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bServerRespectsRemoteAbilityCancellation = false;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnlyTermination;
	
	ActivationOwnedTags.AddTag(EmberGameplayTags::Status_Attack);
}

void UGameplayAbility_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAbilityTask_WaitGameplayEvent* GameplayEvent_Trace = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EmberGameplayTags::GameplayEvent_Trace, nullptr, true))
	{
		GameplayEvent_Trace->EventReceived.AddDynamic(this, &ThisClass::OnTargetDataReady);
		GameplayEvent_Trace->ReadyForActivation();
	}

	if (UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("MeleeAttack"), AttackMontage, AttackRate, NAME_None, false, 1.f, 0.f, false))
	{
		PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageFinished);
		PlayMontageTask->ReadyForActivation();
	}

	if (UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EmberGameplayTags::GameplayEvent_Montage_End, nullptr, true, true))
	{
		GameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::OnMontageEventTriggered);
		GameplayEventTask->ReadyForActivation();
	}
}

void UGameplayAbility_MeleeAttack::OnTargetDataReady(FGameplayEventData Payload)
{
	FGameplayAbilityTargetDataHandle LocalTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(Payload.TargetData)));

	AMonsterAIWeapon* WeaponActor = const_cast<AMonsterAIWeapon*>(Cast<AMonsterAIWeapon>(Payload.Instigator));
	if (WeaponActor == nullptr)
		return;
	
	TArray<int32> CharacterHitIndexes;
	ParseTargetData(LocalTargetDataHandle, CharacterHitIndexes);

	float Damage = 10.f;
	
	for (int32 CharqacterHitIndex : CharacterHitIndexes)
	{
		FHitResult HitResult = *LocalTargetDataHandle.Data[CharqacterHitIndex]->GetHitResult();
		ProcessHitResult(HitResult, Damage, false, nullptr, WeaponActor);
	}
}

void UGameplayAbility_MeleeAttack::OnMontageEventTriggered(FGameplayEventData Payload)
{
	OnMontageFinished();
}

void UGameplayAbility_MeleeAttack::OnMontageFinished()
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}