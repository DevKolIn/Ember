// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "AI/Monster/MonsterAIBase.h"

UMonsterGameplayAbility::UMonsterGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActivationPolicy = ELyraAbilityActivationPolicy::OnInputTriggered;
}

AMonsterAIBase* UMonsterGameplayAbility::GetMonsterAIFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<AMonsterAIBase>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

AAIController* UMonsterGameplayAbility::GetMonsterAIControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<AAIController>(GetAvatarActorFromActorInfo()->GetInstigatorController()) : nullptr);
}

void UMonsterGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	TryActivateAbilityOnSpawn(ActorInfo, Spec);
}

void UMonsterGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec) const
{
	const bool bIsPredicting = (Spec.ActivationInfo.ActivationMode == EGameplayAbilityActivationMode::Predicting);

	// Try to activate if activation policy is on spawn.
	if (ActorInfo && !Spec.IsActive() && !bIsPredicting && (ActivationPolicy == ELyraAbilityActivationPolicy::OnSpawn))
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

		// If avatar actor is torn off or about to die, don't try to activate until we get the new one.
		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);

			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}
}
