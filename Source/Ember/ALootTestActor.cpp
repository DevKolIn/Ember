#include "ALootTestActor.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/LootableInterface.h"
#include "ALootManagerActor.h"

ALootTestActor::ALootTestActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALootTestActor::BeginPlay()
{
	Super::BeginPlay();

	// 1�� �� �޽��� ����
	GetWorld()->GetTimerManager().SetTimer(
		MessageTimerHandle,
		this,
		&ALootTestActor::SendLootMessage,
		1.0f,
		false
	);
}

void ALootTestActor::SendLootMessage()
{
	if (ALootManagerActor* Manager = ALootManagerActor::GetLootManager(this))
	{
		FMonsterDiedMessage Msg;
		Msg.MonsterID = FName("Test");
		Msg.DeathLocation = GetActorLocation();

		UE_LOG(LogTemp, Warning, TEXT("[ALootTestActor] Sending Loot Message"));

		// �������̽� ������� ȣ��
		ILootableInterface::Execute_NotifyMonsterDied(Manager, Msg);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ALootTestActor] No ALootManagerActor (Singleton) found"));
	}

	// ��� �� �ڱ� �ڽ� ����
	Destroy();
}
