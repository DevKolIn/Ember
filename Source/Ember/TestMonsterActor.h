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
	// �׽�Ʈ�� ��� �Լ� (�������Ʈ������ ȣ�� ����)
	UFUNCTION(BlueprintCallable, Category = "Test")
	void SimulateDeath();
};
