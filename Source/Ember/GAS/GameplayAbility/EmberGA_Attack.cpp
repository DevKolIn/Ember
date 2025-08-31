#include "GAS/GameplayAbility/EmberGA_Attack.h"

#include "Weapon/EmberWeaponDataAsset.h"
#include "Character/EmberCharacter.h"
#include "Component/CustomMoveComponent.h"
#include "Component/WeaponComponent.h"
#include "Utility/CHelpers.h"
#include "Utility/CLog.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UEmberGA_Attack::UEmberGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEmberGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	DebugLogE("activate");
	ACharacter* character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (character == nullptr)
	{
		DebugLogE("character is null");
		return;
	}
	UWeaponComponent* weapon = CHelpers::GetComponent<UWeaponComponent>(character);
	CurrentComboData = weapon->GetWeaponAsset();
	if (CurrentComboData.Get()->GetCanMove()== false)
	{
		UCustomMoveComponent* move = CHelpers::GetComponent<UCustomMoveComponent>(character);
		if (move == nullptr)
		{
			DebugLogE("move component is null");
			return;
		}

		move->ShouldMove(CurrentComboData.Get()->GetCanMove());
	}
	UAbilityTask_PlayMontageAndWait* attackMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, *CurrentComboData.Get()->GetTaskInstanceName(), CurrentComboData.Get()->GetAnimMontage(), 1.0f, GetNextSection());

	attackMontage->OnCompleted.AddDynamic(this,&UEmberGA_Attack::OnCompleteCallback);
	attackMontage->OnInterrupted.AddDynamic(this,&UEmberGA_Attack::OnInterruptedCallback);

	attackMontage->ReadyForActivation();

	StartComboTimer();
}

void UEmberGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	DebugLogE("end");
	CurrentComboData = nullptr;
	CurrentCombo = 0;
	bHasNextComboInput = false;
	ACharacter* character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (character == nullptr)
	{
		DebugLogE("character is null");
		return;
	}
	UWeaponComponent* weapon = CHelpers::GetComponent<UWeaponComponent>(character);
	CurrentComboData = weapon->GetWeaponAsset();
	
	UCustomMoveComponent* move = CHelpers::GetComponent<UCustomMoveComponent>(character);
	if (move == nullptr)
	{
		DebugLogE("move component is null");
		return;
	}
	move->ShouldMove(!CurrentComboData.Get()->GetCanMove());
}

void UEmberGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UEmberGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	DebugLogE("input");
	DebugLogW("Pressed");
	if (ComboTimerHandle.IsValid() == false)
		bHasNextComboInput = false;
	else
		bHasNextComboInput = true;
}

void UEmberGA_Attack::OnCompleteCallback()
{
	DebugLogE("complete");
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UEmberGA_Attack::OnInterruptedCallback()
{
	DebugLogE("interrupt");
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);

}

FName UEmberGA_Attack::GetNextSection()
{
	DebugLogE("next");
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboData.Get()->GetMaxCount());
	FName next = *FString::Printf(TEXT("%s%d"),*CurrentComboData->GetMontageSectionNamePrefix(), CurrentCombo);
	return next;
}

void UEmberGA_Attack::StartComboTimer()
{
	DebugLogE("start combo");
	int32 comboIndex = CurrentCombo - 1;
	if (CurrentComboData->GetEffectiveFrameCount(comboIndex) <= 0)
		return;
	float comboEffectiveTime = CurrentComboData->GetEffectiveFrameCount(comboIndex) / CurrentComboData->GetFrameRate();
	if (comboEffectiveTime > 0.0f)
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle,this,&UEmberGA_Attack::CheckComboInput, comboEffectiveTime,false);
}

void UEmberGA_Attack::CheckComboInput()
{
	DebugLogE("check");
	ComboTimerHandle.Invalidate();
	if (bHasNextComboInput == true)
	{
		DebugLogE("true");
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		bHasNextComboInput = false;
	}
}
