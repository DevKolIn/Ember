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

	// Ŭ���̾�Ʈ�� ��� �� ������ ��û
	if (!GetOwner()->HasAuthority())
	{
		if (AEmberCharacter* Character = Cast<AEmberCharacter>(Interactor))
		{
			Character->Server_RequestInteraction(this);
		}
		return;
	}

	// ���������� �ı� ó��
	GetOwner()->Destroy();
}

