#include "Item/Drop/PickupItemActor.h"
#include "Character/EmberCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

APickupItemActor::APickupItemActor()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true; //  멀티플레이용
	SetReplicateMovement(true);

	// 컴포넌트 초기화
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComponent->InitSphereRadius(100.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetIsReplicated(true); //  충돌 컴포넌트도 복제

	RootComponent = CollisionComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetEnableGravity(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeshComponent->SetIsReplicated(true);
	MeshComponent->SetGenerateOverlapEvents(true);


}
void APickupItemActor::OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEmberCharacter* Character = Cast<AEmberCharacter>(OtherActor);
	if (Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("PickupItem: %s overlapped!"), *GetName());
		Character->AddOverlappingItem(this); // 이 함수는 캐릭터에 직접 구현해야 함
	}
}

void APickupItemActor::OnPickupEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AEmberCharacter* Character = Cast<AEmberCharacter>(OtherActor);
	if (Character)
	{
		Character->RemoveOverlappingItem(this);
	}
}
void APickupItemActor::BeginPlay()
{
	Super::BeginPlay();
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("[CLIENT] Dropped item appeared: %s"), *GetName());
	}
	
	if (MeshComponent)
	{
		MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupItemActor::OnPickupBeginOverlap);
		MeshComponent->OnComponentEndOverlap.AddDynamic(this, &APickupItemActor::OnPickupEndOverlap);
	}

}


void APickupItemActor::InitializeLootDrop(const FLootResultData& InLootData)
{
	LootData = InLootData;

	UStaticMesh* TargetMesh = nullptr;

	if (LootData.ItemTemplateClass)
	{
		const UItemTemplate* TemplateCDO = LootData.ItemTemplateClass->GetDefaultObject<UItemTemplate>();
		if (TemplateCDO)
		{
			TargetMesh = TemplateCDO->ItemMesh;
		}
	}
	else if (LootData.RuneTemplateClass)
	{
		const URuneItemTemplate* RuneCDO = LootData.RuneTemplateClass->GetDefaultObject<URuneItemTemplate>();
		if (RuneCDO)
		{
			TargetMesh = RuneCDO->RuneMesh;
		}
	}

	if (TargetMesh)
	{
		MeshComponent->SetStaticMesh(TargetMesh);
	}
}

void APickupItemActor::OnPickedUp(AActor* Picker)
{
	if (!HasAuthority()) return;

	AEmberCharacter* Player = Cast<AEmberCharacter>(Picker);
	if (!Player) return;

	FString ItemName = TEXT("Unnamed Loot");
	bool bConsumed = false;

	// 일반 아이템
	if (LootData.ItemTemplateClass)
	{
		const UItemTemplate* T = LootData.ItemTemplateClass->GetDefaultObject<UItemTemplate>();
		ItemName = T ? T->DisplayName.ToString() : TEXT("Unknown Item");

		if (UQuickSlotComponent* QS = Player->GetQuickSlotComponent())
		{
			QS->AddItemToQuickSlot(LootData.ItemTemplateClass, LootData.Quantity);
			bConsumed = true;
		}
	}
	// 룬
	else if (LootData.RuneTemplateClass)
	{
		const URuneItemTemplate* T = LootData.RuneTemplateClass->GetDefaultObject<URuneItemTemplate>();
		ItemName = T ? T->RuneName.ToString() : TEXT("Unknown Rune");

		if (T)
		{
			//  자동 장착 시도(서버 확정 bool)
			const bool bEquipped = Player->TryEquipRune(T, /*PreferredSlotIndex=*/-1);
			UE_LOG(LogTemp, Warning, TEXT("[Pickup] TryEquipRune returned=%d (server)"), bEquipped);

			if (bEquipped)
			{
				UE_LOG(LogTemp, Warning, TEXT("[Pickup] Rune auto-equipped: %s"), *ItemName);
				bConsumed = true; // 장착 성공일 때만 consume
			}
			else
			{
				// 실패 시에는 '보관만' 하고 파괴 여부는 정책에 따라
				if (UQuickSlotComponent* QS = Player->GetQuickSlotComponent())
				{
					QS->AddRuneToQuickSlot(LootData.RuneTemplateClass, /*Qty=*/1);
					// 권장: 여기서는 파괴하지 말고 남겨 두거나, UI에서 확인 후 정리
					// bConsumed = true;  // ← 지금은 지양 (장착 실패 디버그 용이)
					UE_LOG(LogTemp, Warning, TEXT("[Pickup] Rune stored (not destroyed): %s"), *ItemName);
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Picked up: %s x%d (consumed=%s)"),
		*ItemName, LootData.Quantity, bConsumed ? TEXT("true") : TEXT("false"));

	if (bConsumed)
	{
		Destroy(); // 서버에서 파괴 → 클라 동기화
	}
}


void APickupItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickupItemActor, LootData); //  구조체 복제
}
