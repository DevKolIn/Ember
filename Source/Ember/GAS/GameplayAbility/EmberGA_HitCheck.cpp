// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbility/EmberGA_HitCheck.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/AbilityTask/EmberAT_Trace.h"
#include "GAS/Attribute/EmberAS_Player.h"
#include "GAS/TargetActor/EmberTA_Trace.h"
#include "Tag/EmberGameplayTag.h"
#include "Utility/CLog.h"

UEmberGA_HitCheck::UEmberGA_HitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentLevel = 1.0f;
}

void UEmberGA_HitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UEmberAT_Trace* task = UEmberAT_Trace::CreateTask(this,AEmberTA_Trace::StaticClass());
	task->OnComplete.AddDynamic(this,&UEmberGA_HitCheck::OnTraceResultCallback);
	task->ReadyForActivation();
}

void UEmberGA_HitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	for (int32 i = 0; i < TargetDataHandle.Num(); i++)
	{
		if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, i) == true)
		{
			FHitResult result = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle,i);
			DebugLogW(result.GetActor()->GetName());

			UAbilitySystemComponent* sourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
			if (sourceASC == nullptr)
			{
				DebugLogE("source ASC is null");
				return;
			}
			const UEmberAS_Player* sourceAttribute = sourceASC->GetSet<UEmberAS_Player>();
			FGameplayEffectSpecHandle effectHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect,CurrentLevel);
			if (effectHandle == nullptr)
			{
				DebugLogE("effect handle is null");
				return;
			}

			effectHandle.Data->SetSetByCallerMagnitude(ABTAG_DATA_DAMAGE, - sourceAttribute->GetAttackDamage());
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,effectHandle,TargetDataHandle);
		}
	}
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
