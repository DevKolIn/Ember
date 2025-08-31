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
	// 1. 메시지 구성
	FMonsterDiedMessage Message;
	Message.MonsterID = FName("Test");           // MasterLootTable에 "TestMonster" RowName 필요
	Message.DeathLocation = GetActorLocation();

	// 2. 싱글톤 LootManagerActor 가져오기
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
