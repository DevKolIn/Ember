#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "ItemTemplate.generated.h"

class UNiagaraSystem;
UENUM(BlueprintType)
enum class EItemEffectType : uint8
{
	RestoreHP             UMETA(DisplayName = "ü�� ȸ��"),
	RestoreStamina        UMETA(DisplayName = "���¹̳� ȸ��"),
	RestoreBodyTemp       UMETA(DisplayName = "ü�� ȸ��"),
	Buff_StatHold         UMETA(DisplayName = "���� ���� ����"),
	Buff_MaxStat          UMETA(DisplayName = "�ִ� ���� ����"),
	RandomDebuff          UMETA(DisplayName = "Ȯ�� �����"),
	CastSpell             UMETA(DisplayName = "���� ��ũ�� �ߵ�"),
	None                  UMETA(DisplayName = "����")
};
UCLASS(BlueprintType, Blueprintable)
class UItemTemplate : public UDataAsset
{
	GENERATED_BODY()

public:
	// �⺻ ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UStaticMesh* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UNiagaraSystem* DropEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 MaxStackSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	EItemEffectType EffectType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (EditCondition = "EffectType == EItemEffectType::RestoreHP"))
	float HealAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (EditCondition = "EffectType == EItemEffectType::RestoreBodyTemp"))
	float TemperatureRestoreAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (EditCondition = "EffectType == EItemEffectType::Buff_StatHold"))
	float BuffDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (EditCondition = "EffectType == EItemEffectType::RandomDebuff"))
	float DebuffChance;
};

