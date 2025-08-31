#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ALootTestActor.generated.h"

UCLASS()
class EMBER_API ALootTestActor : public AActor
{
	GENERATED_BODY()

public:
	ALootTestActor();

protected:
	virtual void BeginPlay() override;

	// 타이머용
	void SendLootMessage();

	FTimerHandle MessageTimerHandle;
};
