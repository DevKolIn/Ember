// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_HitCheck.h"
#include "Utility/CLog.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"

FString UAnimNotifyState_HitCheck::GetNotifyName_Implementation() const
{
	return "Hit Check";
}

void UAnimNotifyState_HitCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp == nullptr)
	{
		DebugLogE("Mesh Comp is null");
		return;
	}

	AActor* owner = MeshComp->GetOwner();
	if (owner == nullptr)
	{
		DebugLogE("owner is null");
		return;
	}

	FGameplayEventData data;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(owner, EnableHitTag,data);
}

void UAnimNotifyState_HitCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr)
	{
		DebugLogE("Mesh Comp is null");
		return;
	}

	AActor* owner = MeshComp->GetOwner();
	if (owner == nullptr)
	{
		DebugLogE("owner is null");
		return;
	}

	FGameplayEventData data;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(owner, DisableHitTag, data);
}
