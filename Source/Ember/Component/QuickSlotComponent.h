#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Template/ItemTemplate.h"
#include "QuickSlotComponent.generated.h"
class URuneItemTemplate;       // ���� ���� (��� ��ȯ ����)
USTRUCT(BlueprintType)
struct FQuickSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemTemplate> ItemTemplateClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 0;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EMBER_API UQuickSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuickSlotComponent();

	// (����) ������ �߰�
	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	void AddItemToQuickSlot(TSubclassOf<UItemTemplate> ItemTemplateClass, int32 Quantity);

	// ���� �߰�: �� ���ø� �߰�
	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	void AddRuneToQuickSlot(TSubclassOf<URuneItemTemplate> RuneTemplateClass, int32 Quantity);
	void UseQuickSlot(int32 Index);
	const FQuickSlot* GetSlot(int32 Index) const;

	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuickSlot")
	TArray<FQuickSlot> QuickSlots;

	UPROPERTY(EditDefaultsOnly, Category = "QuickSlot")
	int32 MaxSlots = 5;

	int32 FindSuitableSlot(TSubclassOf<UItemTemplate> ItemClass);

	void LogQuickSlotState() const;

private:
	//  ������ ����� (���ϸ� ������Ʈ ������ ���� �ٲټ���)
	UPROPERTY()
	TMap<TSubclassOf<URuneItemTemplate>, int32> RuneStacks;
};
