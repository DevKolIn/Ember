#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LootableInterface.generated.h"

//////////////////////////////////////////////////////////
// 1. 몬스터 사망 정보 구조체
//////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FMonsterDiedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loot")
	FName MonsterID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loot")
	FVector DeathLocation;
};

//////////////////////////////////////////////////////////
// 2. 인터페이스 선언부 (UINTERFACE)
//////////////////////////////////////////////////////////

UINTERFACE(Blueprintable)
class ULootableInterface : public UInterface
{
	GENERATED_BODY()
};

//////////////////////////////////////////////////////////
// 3. 실제 인터페이스 클래스 정의 (ILootableInterface)
//////////////////////////////////////////////////////////

class EMBER_API ILootableInterface
{
	GENERATED_BODY()

public:
	// 인터페이스에서 반드시 선언할 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Loot")
	void NotifyMonsterDied(const FMonsterDiedMessage& Message);
};
