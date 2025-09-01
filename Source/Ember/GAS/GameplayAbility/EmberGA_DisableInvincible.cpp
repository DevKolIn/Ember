#include "GAS/GameplayAbility/EmberGA_DisableInvincible.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Utility/CLog.h"

UEmberGA_DisableInvincible::UEmberGA_DisableInvincible()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEmberGA_DisableInvincible::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	TArray<UActorComponent*> components;
	character->GetComponents(components);
	for (UActorComponent* comp : components)
	{
		if (UPrimitiveComponent* prim = Cast<UPrimitiveComponent>(comp))
		{
			prim->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
			prim->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
		}
	}

	EndAbility(Handle,ActorInfo,ActivationInfo,true,false);
}
