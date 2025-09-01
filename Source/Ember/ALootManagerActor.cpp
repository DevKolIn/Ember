// Fill out your copyright notice in the Description page of Project Settings.

#include "ALootManagerActor.h"
#include "Component/LootDropManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "EngineUtils.h"

// 싱글톤 전역 포인터 초기화
TWeakObjectPtr<ALootManagerActor> ALootManagerActor::SingletonInstance = nullptr;

ALootManagerActor::ALootManagerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// 1. 루트 컴포넌트 생성
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);

	// 2. LootDropManagerComponent 생성 및 부착
	LootDropComponent = CreateDefaultSubobject<ULootDropManagerComponent>(TEXT("LootDropComponent"));
}

void ALootManagerActor::BeginPlay()
{
	Super::BeginPlay();

	// 싱글톤 등록
	if (!SingletonInstance.IsValid())
	{
		SingletonInstance = this;
		UE_LOG(LogTemp, Log, TEXT("[LootManager] Singleton instance registered."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[LootManager] Multiple instances detected. Only the first one will be used."));
	}

	// 예시: 테스트용 타이머
	/*
	GetWorld()->GetTimerManager().SetTimer(
		SimulateDeathTimerHandle,
		this,
		&ALootManagerActor::SimulateDeath,
		1.0f,
		false
	);
	*/
}

//  전역 접근용 정적 함수
ALootManagerActor* ALootManagerActor::GetLootManager(const UObject* WorldContext)
{
	if (SingletonInstance.IsValid())
	{
		return SingletonInstance.Get();
	}

	// BeginPlay 전에 접근했거나 초기화 안됐을 경우 월드 내에서 수동 탐색
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContext);
	for (TActorIterator<ALootManagerActor> It(World); It; ++It)
	{
		SingletonInstance = *It;
		break;
	}

	return SingletonInstance.Get();
}

void ALootManagerActor::SimulateDeath()
{
	FMonsterDiedMessage Message;
	Message.MonsterID = TEXT("Test");  // DataTable의 RowName과 일치해야 함
	Message.DeathLocation = GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("SimulateDeath() called"));

	if (this->GetClass()->ImplementsInterface(ULootableInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ALootManagerActor] Executing NotifyMonsterDied via Execute_"));
		ILootableInterface::Execute_NotifyMonsterDied(this, Message);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast to ILootableInterface"));
	}
}

void ALootManagerActor::NotifyMonsterDied_Implementation(const FMonsterDiedMessage& Message)
{
	if (LootDropComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ALootManagerActor] Calling LootDropComponent->HandleMonsterDied"));
		LootDropComponent->HandleMonsterDied(Message);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LootDropComponent is null"));
	}
}
