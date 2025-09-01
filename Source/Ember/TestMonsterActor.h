#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestMonsterActor.generated.h"

UCLASS()
class EMBER_API ATestMonsterActor : public AActor
{
	GENERATED_BODY()

public:
	ATestMonsterActor();

protected:
	virtual void BeginPlay() override;

public:
	// 테스트용 사망 함수 (블루프린트에서도 호출 가능)
	UFUNCTION(BlueprintCallable, Category = "Test")
	void SimulateDeath();
};
