#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/LootableInterface.h"
#include "LootDropManagerComponent.generated.h"

class UDataTable;
class APickupItemActor;

/**
 * ���� ��� �� �������� ����ϴ� ������Ʈ
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EMBER_API ULootDropManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULootDropManagerComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable)
	void HandleMonsterDied(const FMonsterDiedMessage& Message);

protected:
	/** �� ���̺� ������ */
	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	UDataTable* MasterLootTable;

	/** ��ӵǴ� ������ ���� Ŭ���� */
	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	TSubclassOf<APickupItemActor> PickupItemActorClass;
	// �������� �Լ� �� �ش� ���뿡�� �� TMap
	UFUNCTION()
	void OnLootItemHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	TMap<UPrimitiveComponent*, APickupItemActor*> ActiveLootMap;

};
