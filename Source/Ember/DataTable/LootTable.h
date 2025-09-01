#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Item/LootResultData.h" // 여기서 enum 포함됨
#include "Template/ItemTemplate.h"
#include "Template/RuneItemTemplate.h"
#include "LootTable.generated.h"



// 희귀도별 가중치 정보
USTRUCT(BlueprintType)
struct FRarityDropInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	EItemRarity Rarity = EItemRarity::Common;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	float Weight = 1.0f;
};

// 아이템 드롭 규칙
USTRUCT(BlueprintType)
struct FLootDropItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TSubclassOf<class UItemTemplate> ItemTemplateClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<URuneItemTemplate> RuneTemplateClass; // 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	float DropChance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	FIntPoint QuantityRange = FIntPoint(1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TArray<FRarityDropInfo> PossibleRarities;
};

// 몬스터별 전체 드롭 리스트 (DataTable의 Row)
USTRUCT(BlueprintType)
struct FMonsterLootProfile : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TArray<FLootDropItem> LootDrops;
};
