// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/AnimNotifyState_Invincible.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AI/AbilitySystem/Abilities/Monster/GameplayAbility_Death.h"
#include "Utility/CLog.h"

FString UAnimNotifyState_Invincible::GetNotifyName_Implementation() const
{
	return "Invincible Frame";
}
void UAnimNotifyState_Invincible::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp == nullptr)
	{
		DebugLogE("character mesh is null");
		return;
	}
	AActor* owner = MeshComp->GetOwner();
	if (owner == nullptr)
	{
		DebugLogE("owner is null");
		return;
	}

	FGameplayEventData data;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(owner,EnableInvincibleTag,data);
}

void UAnimNotifyState_Invincible::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr)
	{
		DebugLogE("character mesh is null");
		return;
	}
	AActor* owner = MeshComp->GetOwner();
	if (owner == nullptr)
	{
		DebugLogE("owner is null");
		return;
	}

	FGameplayEventData data;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(owner, DisableInvincibleTag, data);
}


