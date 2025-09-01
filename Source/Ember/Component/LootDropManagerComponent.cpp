#include "Component/LootDropManagerComponent.h"
#include "GameFramework/Character.h"
#include "Item/Drop/PickupItemActor.h"
#include "Item/LootResultData.h"
#include "DataTable/LootTable.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

ULootDropManagerComponent::ULootDropManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULootDropManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULootDropManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ULootDropManagerComponent::OnLootItemHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!IsValid(HitComp) || !OtherComp) return;

	UE_LOG(LogTemp, Warning, TEXT("HIT! %s -> %s | Type: %d"),
		*HitComp->GetName(), *OtherActor->GetName(), (int32)OtherComp->GetCollisionObjectType());

	if (OtherComp->GetCollisionObjectType() != ECC_WorldStatic)
		return;

	// 임팩트 위치가 밑면보다 아래일 때만 (Z축 기준)
	if (Hit.ImpactPoint.Z > HitComp->GetComponentLocation().Z + 10.0f)
		return;

	UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(HitComp);
	if (MeshComp)
	{
		MeshComp->SetSimulatePhysics(false);
		MeshComp->SetEnableGravity(false);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

		MeshComp->OnComponentHit.RemoveDynamic(this, &ULootDropManagerComponent::OnLootItemHit);
		ActiveLootMap.Remove(MeshComp);
	}
}


void ULootDropManagerComponent::HandleMonsterDied(const FMonsterDiedMessage& Message)
{
	UE_LOG(LogTemp, Warning, TEXT("HandleMonsterDied() called for ID: %s"), *Message.MonsterID.ToString());
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("No Authority"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[LootDropManager] NotifyMonsterDied Called. Monster ID: %s"), *Message.MonsterID.ToString());

	if (!MasterLootTable || !PickupItemActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("LootDropManager: Missing MasterLootTable or PickupItemActorClass."));
		return;
	}

	const FMonsterLootProfile* LootProfile = MasterLootTable->FindRow<FMonsterLootProfile>(
		Message.MonsterID, TEXT("NotifyMonsterDied"));

	if (!LootProfile)
	{
		UE_LOG(LogTemp, Warning, TEXT("LootDropManager: No loot profile found for MonsterID %s"), *Message.MonsterID.ToString());
		return;
	}

	TArray<FLootResultData> FinalLoots;

	for (const FLootDropItem& Drop : LootProfile->LootDrops)
	{
		if (FMath::FRand() < Drop.DropChance)
		{
			int32 Quantity = FMath::RandRange(Drop.QuantityRange.X, Drop.QuantityRange.Y);

			EItemRarity FinalRarity = EItemRarity::Common;
			float TotalWeight = 0.f;

			for (const FRarityDropInfo& RarityInfo : Drop.PossibleRarities)
			{
				TotalWeight += RarityInfo.Weight;
			}

			float RarityRoll = FMath::FRand() * TotalWeight;
			for (const FRarityDropInfo& RarityInfo : Drop.PossibleRarities)
			{
				if (RarityRoll < RarityInfo.Weight)
				{
					FinalRarity = RarityInfo.Rarity;
					break;
				}
				RarityRoll -= RarityInfo.Weight;
			}

			FLootResultData Result;
			Result.Quantity = Quantity;
			Result.Rarity = FinalRarity;

			if (Drop.ItemTemplateClass)  // ⭐ 일반 아이템
			{
				Result.ItemTemplateClass = Drop.ItemTemplateClass;
			}
			else if (Drop.RuneTemplateClass) // ⭐ 룬 아이템
			{
				Result.RuneTemplateClass = Drop.RuneTemplateClass;
			}

			FinalLoots.Add(Result);
		}
	}

	if (FinalLoots.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("LootDropManager: No items dropped for %s"), *Message.MonsterID.ToString());
		return;
	}
	else
	{
		for (const FLootResultData& Loot : FinalLoots)
		{
			FVector RandomOffset = FVector(
				FMath::FRandRange(-50.f, 50.f),
				FMath::FRandRange(-50.f, 50.f),
				0.0f
			);

			FVector SpawnLocation = Message.DeathLocation + RandomOffset;

			// 지면에 맞추기 위한 라인트레이스
			FHitResult HitResult;
			FVector TraceStart = SpawnLocation + FVector(0.f, 0.f, 100.f);
			FVector TraceEnd = SpawnLocation - FVector(0.f, 0.f, 1000.f);
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(Cast<AActor>(GetOwner()));

			if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params))
			{
				SpawnLocation.Z = HitResult.ImpactPoint.Z + 100.0f;
			}

			FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);

			APickupItemActor* LootActor = GetWorld()->SpawnActorDeferred<APickupItemActor>(
				PickupItemActorClass,
				SpawnTransform,
				nullptr,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);

			if (LootActor)
			{
				LootActor->SetReplicates(true);
				LootActor->InitializeLootDrop(Loot);
				LootActor->FinishSpawning(SpawnTransform);

				if (UStaticMeshComponent* MeshComp = LootActor->FindComponentByClass<UStaticMeshComponent>())
				{
					// 캐릭터 충돌 무시
					TArray<AActor*> FoundCharacters;
					UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), FoundCharacters);
					for (AActor* Character : FoundCharacters)
					{
						MeshComp->IgnoreActorWhenMoving(Character, true);
					}

					// 물리 활성화 + 충돌 설정
					MeshComp->SetSimulatePhysics(true);
					MeshComp->SetEnableGravity(true);
					MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					MeshComp->SetCollisionResponseToAllChannels(ECR_Block);
					MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
					MeshComp->SetNotifyRigidBodyCollision(true);

					// 바닥과 부딪히면 물리 꺼짐 (ActorHasTag 사용 X)
					//  바닥 충돌 시 처리용 함수 바인딩
					MeshComp->OnComponentHit.AddDynamic(this, &ULootDropManagerComponent::OnLootItemHit);
					// 드롭된 아이템이 캐릭터와 충돌하지 않도록
					ActiveLootMap.Add(MeshComp, LootActor); // (필요 시 활용)

					// Impulse 적용
					FVector ImpulseDir = UKismetMathLibrary::RandomUnitVector();
					ImpulseDir.Z = FMath::Clamp(ImpulseDir.Z, 0.3f, 0.8f);
					const float ImpulseStrength = 500.f;
					MeshComp->AddImpulse(ImpulseDir * ImpulseStrength, NAME_None, true);
				}

				if (Loot.ItemTemplateClass)
				{
					const UItemTemplate* ItemTemplate = Loot.ItemTemplateClass->GetDefaultObject<UItemTemplate>();
					FString NameStr = ItemTemplate ? ItemTemplate->DisplayName.ToString() : TEXT("Unknown Item");
					UE_LOG(LogTemp, Log, TEXT("[LootDropManager] Dropped item: %s x%d (Rarity: %d)"),
						*NameStr,
						Loot.Quantity,
						(int32)Loot.Rarity);
				}
				else if (Loot.RuneTemplateClass)
				{
					const URuneItemTemplate* RuneTemplate = Loot.RuneTemplateClass->GetDefaultObject<URuneItemTemplate>();
					FString NameStr = RuneTemplate ? RuneTemplate->RuneName.ToString() : TEXT("Unknown RUNE");
					UE_LOG(LogTemp, Log, TEXT("[LootDropManager] Dropped RUNE: %s x%d (Rarity: %d)"),
						*NameStr,
						Loot.Quantity,
						(int32)Loot.Rarity);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("[LootDropManager] Dropped item with no valid template!"));
				}
			}
		}
	}
}
