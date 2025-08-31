#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Component/InteractionComponent.h"
#include "BaseItem.generated.h"

class UStaticMeshComponent;
class UInteractionComponent;
UCLASS()
class EMBER_API ABaseItem : public AActor
{
	GENERATED_BODY()

public:
	ABaseItem();


public:

	/** 아이템 메시 (외형) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UStaticMeshComponent* ItemMesh;

	/** 아이템 이름 (디버그/로그용) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemName;

	/** 아이템 ID 또는 타입 구분용 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemID;

	/** 상호작용 함수에서 컴포넌트로 변경*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInteractionComponent* InteractionComp;

	// 아이템 사용 - 상속 클래스에서 구현
	virtual void Use(AActor* User);
};
