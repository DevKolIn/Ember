#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/LootableInterface.h"
#include "LootDropManagerComponent.generated.h"

class UDataTable;
class APickupItemActor;

/**
 * 몬스터 사망 시 아이템을 드롭하는 컴포넌트
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
	/** 룻 테이블 데이터 */
	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	UDataTable* MasterLootTable;

	/** 드롭되는 아이템 액터 클래스 */
	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	TSubclassOf<APickupItemActor> PickupItemActorClass;
	// 피직스용 함수 및 해당 내용에서 쓸 TMap
	UFUNCTION()
	void OnLootItemHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	TMap<UPrimitiveComponent*, APickupItemActor*> ActiveLootMap;

};
