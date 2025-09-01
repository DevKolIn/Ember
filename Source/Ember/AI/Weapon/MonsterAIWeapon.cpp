// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIWeapon.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

AMonsterAIWeapon::AMonsterAIWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bReplicates = true;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->PrimaryComponentTick.bCanEverTick = false;
	SetRootComponent(ArrowComponent);
	
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionProfileName("Weapon");
	MeshComponent->SetGenerateOverlapEvents(false);
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	MeshComponent->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;

	TraceDebugCollision = CreateDefaultSubobject<UBoxComponent>("TraceDebugCollision");
	TraceDebugCollision->SetCollisionProfileName("NoCollision");
	TraceDebugCollision->SetGenerateOverlapEvents(false);
	TraceDebugCollision->SetupAttachment(GetRootComponent());
	TraceDebugCollision->PrimaryComponentTick.bStartWithTickEnabled = false;
}

void AMonsterAIWeapon::BeginPlay()
{
	Super::BeginPlay();
}

