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

	/** ������ �޽� (����) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UStaticMeshComponent* ItemMesh;

	/** ������ �̸� (�����/�α׿�) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemName;

	/** ������ ID �Ǵ� Ÿ�� ���п� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemID;

	/** ��ȣ�ۿ� �Լ����� ������Ʈ�� ����*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInteractionComponent* InteractionComp;

	// ������ ��� - ��� Ŭ�������� ����
	virtual void Use(AActor* User);
};
