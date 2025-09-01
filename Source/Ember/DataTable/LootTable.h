#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Item/LootResultData.h" // ���⼭ enum ���Ե�
#include "Template/ItemTemplate.h"
#include "Template/RuneItemTemplate.h"
#include "LootTable.generated.h"



// ��͵��� ����ġ ����
USTRUCT(BlueprintType)
struct FRarityDropInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	EItemRarity Rarity = EItemRarity::Common;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	float Weight = 1.0f;
};

// ������ ��� ��Ģ
USTRUCT(BlueprintType)
struct FLootDropItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TSubclassOf<class UItemTemplate> ItemTemplateClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<URuneItemTemplate> RuneTemplateClass; // �߰�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	float DropChance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	FIntPoint QuantityRange = FIntPoint(1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TArray<FRarityDropInfo> PossibleRarities;
};

// ���ͺ� ��ü ��� ����Ʈ (DataTable�� Row)
USTRUCT(BlueprintType)
struct FMonsterLootProfile : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TArray<FLootDropItem> LootDrops;
};
