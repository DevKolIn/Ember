#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LootableInterface.generated.h"

//////////////////////////////////////////////////////////
// 1. ���� ��� ���� ����ü
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
// 2. �������̽� ����� (UINTERFACE)
//////////////////////////////////////////////////////////

UINTERFACE(Blueprintable)
class ULootableInterface : public UInterface
{
	GENERATED_BODY()
};

//////////////////////////////////////////////////////////
// 3. ���� �������̽� Ŭ���� ���� (ILootableInterface)
//////////////////////////////////////////////////////////

class EMBER_API ILootableInterface
{
	GENERATED_BODY()

public:
	// �������̽����� �ݵ�� ������ �Լ�
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Loot")
	void NotifyMonsterDied(const FMonsterDiedMessage& Message);
};
