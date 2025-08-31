// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbility/EmberGA_Avoid.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Component/MontageComponent.h"
#include "GameFramework/Character.h"
#include "Tag/EmberGameplayTag.h"
#include "Utility/CLog.h"

UEmberGA_Avoid::UEmberGA_Avoid()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEmberGA_Avoid::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
	UMontageComponent* montage = Cast<UMontageComponent>(character->GetComponentByClass(UMontageComponent::StaticClass()));
	if (montage == nullptr)
	{
		DebugLogE("montage component is null");
		return;
	}
	FMontageData* data = montage->GetAvoidData(ABTAG_DATA_Avoid);
	FVector inputDir = character->GetLastMovementInputVector();
	inputDir.Z = 0.0f;
	inputDir.Normalize();

	FVector forward = character->GetActorForwardVector();
	forward.Z = 0.0f;
	forward.Normalize();

	float angle = FMath::RadiansToDegrees(acos(FVector::DotProduct(inputDir, forward)));
	float crossZ = FVector::CrossProduct(forward, inputDir).Z;

	if (crossZ < 0)
		angle = 360.0f - angle;
	
	{
		//FGameplayTag DirectionTag;

		//if (AngleDegrees >= 337.5f || AngleDegrees < 22.5f)
		//	DirectionTag = TAG_Dodge_Forward;       // ¡è
		//else if (AngleDegrees < 67.5f)
		//	DirectionTag = TAG_Dodge_ForwardRight;  // ¢Ö
		//else if (AngleDegrees < 112.5f)
		//	DirectionTag = TAG_Dodge_Right;         // ¡æ
		//else if (AngleDegrees < 157.5f)
		//	DirectionTag = TAG_Dodge_BackwardRight; // ¢Ù
		//else if (AngleDegrees < 202.5f)
		//	DirectionTag = TAG_Dodge_Backward;      // ¡é
		//else if (AngleDegrees < 247.5f)
		//	DirectionTag = TAG_Dodge_BackwardLeft;  // ¢×
		//else if (AngleDegrees < 292.5f)
		//	DirectionTag = TAG_Dodge_Left;          // ¡ç
		//else
		//	DirectionTag = TAG_Dodge_ForwardLeft;   // ¢Ø
	}
	if (data->Montage == nullptr)
		OnCancelledCallback();
	UAbilityTask_PlayMontageAndWait* avoid = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "Avoid", data->Montage, data->PlayRate);
	avoid->OnCompleted.AddDynamic(this,&UEmberGA_Avoid::OnCompleteCallback);
	avoid->OnCancelled.AddDynamic(this,&UEmberGA_Avoid::OnCancelledCallback);
	avoid->ReadyForActivation();
}

void UEmberGA_Avoid::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEmberGA_Avoid::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo,CurrentActivationInfo,true, false);
}

void UEmberGA_Avoid::OnCancelledCallback()
{
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,true);
}

void UEmberGA_Avoid::OnInterruptedCallback()
{
}
