#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EmberPlayerController.generated.h"

class UInputMappingContext;

/**
 * 플레이어 컨트롤러 - 입력 매핑 컨텍스트 설정용
 */
UCLASS()
class EMBER_API AEmberPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	//Snow VFX
	
	void SpawnFX(); //VFX 스폰
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Weather") //블루프린트 구현
	void ChangeFX(); //VFX 변경
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TSubclassOf<AActor> SnowFXClass;

	UPROPERTY(BlueprintReadOnly, Category = "FX")
	AActor* SnowFXActor;

	// Enhanced Input Mapping Context
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* SprintAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* InteractAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* Avoid;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* Quick1Action;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* Quick2Action;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* Quick3Action;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* Quick4Action;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* Quick5Action;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void SetupInputMapping();
};
