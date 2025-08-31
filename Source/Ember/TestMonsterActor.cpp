#include "TestMonsterActor.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/LootableInterface.h"
#include "ALootManagerActor.h"

ATestMonsterActor::ATestMonsterActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATestMonsterActor::BeginPlay()
{
	Super::BeginPlay();


}

void ATestMonsterActor::SimulateDeath()
{
	// 1. �޽��� ����
	FMonsterDiedMessage Message;
	Message.MonsterID = FName("Test");           // MasterLootTable�� "TestMonster" RowName �ʿ�
	Message.DeathLocation = GetActorLocation();

	// 2. �̱��� LootManagerActor ��������
	if (ALootManagerActor* LootManager = ALootManagerActor::GetLootManager(this))
	{
		ILootableInterface::Execute_NotifyMonsterDied(LootManager, Message);
		UE_LOG(LogTemp, Log, TEXT("[Test] Sent MonsterDiedMessage to %s"), *LootManager->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Test] No ALootManagerActor found (singleton not initialized)"));
	}
}
