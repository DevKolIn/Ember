#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/LootResultData.h"
#include "PickupItemActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class EMBER_API APickupItemActor : public AActor
{
	GENERATED_BODY()

public:
	APickupItemActor();
	void InitializeLootDrop(const FLootResultData& InLootData);
	void OnPickedUp(AActor* Picker);

	// Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPickupEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(Replicated)
	FLootResultData LootData;
};
