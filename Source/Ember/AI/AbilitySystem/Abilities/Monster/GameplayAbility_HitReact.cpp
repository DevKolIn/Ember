// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_HitReact.h"

#include "Abilities/Tasks/AbilityTask_NetworkSyncPoint.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AI/Data/MonsterAIData.h"
#include "AI/Data/MonsterAITemplate.h"
#include "Utility/EmberGameplayTags.h"

UGameplayAbility_HitReact::UGameplayAbility_HitReact(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bServerRespectsRemoteAbilityCancellation = true;
	bRetriggerInstancedAbility = true;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	AbilityTags.AddTag(EmberGameplayTags::AI_Ability_HitReact);
	ActivationOwnedTags.AddTag(EmberGameplayTags::Status_HitReact);

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = EmberGameplayTags::GameplayEvent_HitReact;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UGameplayAbility_HitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerEventData == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	if (UAbilityTask_NetworkSyncPoint* NetSyncTask = UAbilityTask_NetworkSyncPoint::WaitNetSync(this, EAbilityTaskNetSyncType::OnlyServerWait))
	{
		NetSyncTask->OnSync.AddDynamic(this, &ThisClass::OnNetSync);
		NetSyncTask->ReadyForActivation();
	}
}

void UGameplayAbility_HitReact::OnNetSync()
{
	AActor* TargetActor = GetAvatarActorFromActorInfo();
	if (TargetActor == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	const FHitResult* HitResultPtr = CurrentEventData.ContextHandle.GetHitResult();
	if (HitResultPtr == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}
	
	const UMonsterAITemplate& MonsterAITemplate = UMonsterAIData::Get().FindMonsterAITemplateByClass(TargetActor->GetClass());
	UAnimMontage* HitMontage = MonsterAITemplate.GetHitMontage(TargetActor, HitResultPtr->ImpactPoint);

	if (UAbilityTask_PlayMontageAndWait* HitReactMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("HitReactMontage"), HitMontage, 1.f, NAME_None, true))
	{
		HitReactMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageFinished);
		HitReactMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontageFinished);
		HitReactMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageFinished);
		HitReactMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageFinished);
		HitReactMontageTask->ReadyForActivation();
	}
}

void UGameplayAbility_HitReact::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
