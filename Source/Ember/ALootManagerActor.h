#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/LootableInterface.h"
#include "Component/LootDropManagerComponent.h"
#include "ALootManagerActor.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable)
class EMBER_API ALootManagerActor : public AActor, public ILootableInterface
{
	GENERATED_BODY()

public:
	ALootManagerActor();

protected:
	virtual void BeginPlay() override;

public:
	// 싱글톤 접근 함수
	static ALootManagerActor* GetLootManager(const UObject* WorldContext);

	// 테스트용 사망 트리거 함수
	UFUNCTION(BlueprintCallable, Category = "Test")
	void SimulateDeath();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void NotifyMonsterDied(const FMonsterDiedMessage& Message);

	// ILootableInterface 구현
	virtual void NotifyMonsterDied_Implementation(const FMonsterDiedMessage& Message) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Loot", meta = (AllowPrivateAccess = "true"))
	ULootDropManagerComponent* LootDropComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	FTimerHandle SimulateDeathTimerHandle;

private:
	// 내부 정적 포인터
	static TWeakObjectPtr<ALootManagerActor> SingletonInstance;
};
