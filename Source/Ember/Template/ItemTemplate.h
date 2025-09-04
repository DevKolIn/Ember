// ItemTemplate.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "ItemTemplate.generated.h"

UENUM(BlueprintType)
enum class EItemEffectKind : uint8
{
    RestoreHP,
    RestoreBodyTemp,
    Buff_StatHold,
    Buff_MaxStat,
    RandomDebuff,
    CastSpell
};
UENUM(BlueprintType)
enum class EItemStatTarget : uint8
{
    HP, Stamina, BodyTemp
};

USTRUCT(BlueprintType)
struct FItemEffectSpec
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
    EItemEffectKind Type = EItemEffectKind::RestoreHP;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
    float Chance = 1.0f;                   // 0~1

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "Type==EItemEffectKind::RestoreHP || Type==EItemEffectKind::RestoreBodyTemp || Type==EItemEffectKind::Buff_MaxStat"))
    float Magnitude = 0.f;                 // 회복/증가량

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "Type==EItemEffectKind::Buff_StatHold || Type==EItemEffectKind::Buff_MaxStat"))
    float Duration = 0.f;                  // 지속시간(초)

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "Type==EItemEffectKind::Buff_StatHold || Type==EItemEffectKind::Buff_MaxStat"))
    EItemStatTarget TargetStat = EItemStatTarget::BodyTemp;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "Type==EItemEffectKind::RandomDebuff"))
    float DebuffMagnitude = 0.f;
};

UCLASS(BlueprintType, Blueprintable)
class UItemTemplate : public UDataAsset
{
    GENERATED_BODY()

public:
    // 기본 정보
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item") FName ItemID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item") FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item") FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item") UTexture2D* Icon = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item") UStaticMesh* ItemMesh = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item") UNiagaraSystem* DropEffect = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item") int32 MaxStackSize = 1;

    //  한 아이템에 복수 효과를 담는 핵심 필드
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
    TArray<FItemEffectSpec> Effects;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rules")
    bool bConsumeOnlyIfAnyEffectApplied = true; // 하나도 적용 안되면 소비/사용 금지
};
