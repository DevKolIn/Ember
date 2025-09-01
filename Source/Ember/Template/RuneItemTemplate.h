#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "RuneItemTemplate.generated.h"

class ARuneItem;
class UGameplayAbility;
class UGameplayEffect;
class UNiagaraSystem;
class UStaticMesh;

UCLASS(BlueprintType) // DataAsset은 BlueprintType만으로 충분
class EMBER_API URuneItemTemplate : public UDataAsset
{
    GENERATED_BODY()

public:
    /** (옵션) 룬 액터 클래스를 별도로 쓰실 거면 유지, 2안에서는 보통 불필요 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rune")
    TSubclassOf<ARuneItem> RuneItemClass;

    /** 부여할 Ability/Effect */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rune")
    TSubclassOf<UGameplayAbility> GrantedAbility;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rune")
    TSubclassOf<UGameplayEffect>  GrantedEffect;


    /** 템플릿 스탯(원시 필드) — 컴포넌트에서 FRuneStat로 조립 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rune|Stat")
    float BasePower = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rune|Stat")
    float BaseCooldownReduction = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rune|Stat")
    FString BaseElement;

    /** 표시용 텍스트/설명 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rune")
    FText RuneName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rune")
    FText Description;

    /** 비주얼(메시/이펙트) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
    UStaticMesh* RuneMesh = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
    UNiagaraSystem* DropEffect = nullptr;
};
