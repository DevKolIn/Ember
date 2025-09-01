// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AbilityTask/EmberAT_Jump.h"

#include "GameFramework/Character.h"

UEmberAT_Jump::UEmberAT_Jump()
{
}

UEmberAT_Jump* UEmberAT_Jump::CreateTask(UGameplayAbility* OwningAbility)
{
	UEmberAT_Jump* task = NewAbilityTask<UEmberAT_Jump>(OwningAbility);

	return task;
}

void UEmberAT_Jump::Activate()
{
	Super::Activate();

	ACharacter* character = CastChecked<ACharacter>(GetAvatarActor());
	character->LandedDelegate.AddDynamic(this, &UEmberAT_Jump::OnLandedCallback);
	character->Jump();

	SetWaitingOnAvatar();
}

void UEmberAT_Jump::OnDestroy(bool bInOwnerFinished)
{
	ACharacter* character = CastChecked<ACharacter>(GetAvatarActor());
	character->LandedDelegate.RemoveDynamic(this, &UEmberAT_Jump::OnLandedCallback);

	Super::OnDestroy(bInOwnerFinished);
}

void UEmberAT_Jump::OnLandedCallback(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates() == true)
		OnComplete.Broadcast();

}