#pragma once

#include "CoreMinimal.h"
#include "Template/ItemTemplate.h"
#include "Template/RuneItemTemplate.h"
#include "LootResultData.generated.h"


// 레어리티 직접 선언 (별도 파일 없이)
UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Common     UMETA(DisplayName = "Common"),
	Rare       UMETA(DisplayName = "Rare"),
	Epic       UMETA(DisplayName = "Epic"),
	Legendary  UMETA(DisplayName = "Legendary")
};

USTRUCT(BlueprintType)
struct FLootResultData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TSubclassOf<UItemTemplate> ItemTemplateClass;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TSubclassOf<URuneItemTemplate> RuneTemplateClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	EItemRarity Rarity = EItemRarity::Common;
};
