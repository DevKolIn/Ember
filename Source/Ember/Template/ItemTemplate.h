#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "ItemTemplate.generated.h"

class UNiagaraSystem;
UENUM(BlueprintType)
enum class EItemEffectType : uint8
{
	RestoreHP             UMETA(DisplayName = "체력 회복"),
	RestoreStamina        UMETA(DisplayName = "스태미너 회복"),
	RestoreBodyTemp       UMETA(DisplayName = "체온 회복"),
	Buff_StatHold         UMETA(DisplayName = "스탯 유지 버프"),
	Buff_MaxStat          UMETA(DisplayName = "최대 스탯 증가"),
	RandomDebuff          UMETA(DisplayName = "확률 디버프"),
	CastSpell             UMETA(DisplayName = "마법 스크롤 발동"),
	None                  UMETA(DisplayName = "없음")
};
UCLASS(BlueprintType, Blueprintable)
class UItemTemplate : public UDataAsset
{
	GENERATED_BODY()

public:
	// 기본 정보
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

