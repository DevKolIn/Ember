#include "GAS/AbilityTask/EmberAT_Trace.h"

#include "AbilitySystemComponent.h"
#include "GAS/TargetActor/EmberTA_Trace.h"
#include "Utility/CLog.h"

UEmberAT_Trace::UEmberAT_Trace()
{
}

UEmberAT_Trace* UEmberAT_Trace::CreateTask(UGameplayAbility* OwningAbility,
	TSubclassOf<AEmberTA_Trace> InTargetActorClass)
{
	UEmberAT_Trace* task = NewAbilityTask<UEmberAT_Trace>(OwningAbility);
	task->TargetActorClass = InTargetActorClass;
	return task;
}

void UEmberAT_Trace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<AEmberTA_Trace>(Ability->GetWorld()->
		SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass,FTransform::Identity,
			nullptr,nullptr,ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor == nullptr)
	{
		DebugLogE("SpawnedTargetActor is null");
		return;
	}

	SpawnedTargetActor->SetShowDebug(true);
	SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this,&UEmberAT_Trace::OnTargetDataReadyCallback);
}

void UEmberAT_Trace::FinallzeTargetActor()
{
	UAbilitySystemComponent* asc = AbilitySystemComponent.Get();
	if (asc == nullptr)
	{
		DebugLogE("asc is null");
		return;
	}

	const FTransform transform = asc->GetAvatarActor()->GetTransform();
	SpawnedTargetActor->FinishSpawning(transform);
	asc->SpawnedTargetActors.Push(SpawnedTargetActor);
	SpawnedTargetActor->StartTargeting(Ability);
	SpawnedTargetActor->ConfirmTargeting();
}

void UEmberAT_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinallzeTargetActor();

	SetWaitingOnAvatar();
}

void UEmberAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates() == true)
		OnComplete.Broadcast(TargetDataHandle);

	EndTask();
}

void UEmberAT_Trace::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
	if (SpawnedTargetActor == nullptr)
	{
		DebugLogE("Spawned Target Actor is null");
		return;
	}

	SpawnedTargetActor->Destroy();
}
