#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utility/GameFlag.h"
#include "CustomMoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMBER_API UCustomMoveComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, category = "Move")
	float Speed[(int32)ESpeedType::Max]{ 200,400,600 };

public:	
	UCustomMoveComponent();

protected:
	virtual void BeginPlay() override;

public:	
	void Move(const struct FInputActionValue& Value);

	void OnSprint();
	void OnRun();
	void OnWalk();

	void SetSpeed(ESpeedType SpeedType);

	void ShouldMove(bool InCanMove);

public:
	UFUNCTION(BlueprintCallable)
	float GetCurrentSpeed() const;

private:
	TObjectPtr<ACharacter> OwnerCharacter;
	ESpeedType CurrentSpeedType;
};
