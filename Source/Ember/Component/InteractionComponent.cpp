#include "InteractionComponent.h"
#include "Item/BaseItem.h"
#include "Character/EmberCharacter.h"
#include "GameFramework/Actor.h"


UInteractionComponent::UInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UInteractionComponent::Interact(AActor* Interactor)
{
	if (!bCanInteract || !Interactor) return;

	// 클라이언트일 경우 → 서버로 요청
	if (!GetOwner()->HasAuthority())
	{
		if (AEmberCharacter* Character = Cast<AEmberCharacter>(Interactor))
		{
			Character->Server_RequestInteraction(this);
		}
		return;
	}

	// 서버에서만 파괴 처리
	GetOwner()->Destroy();
}

