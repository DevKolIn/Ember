#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffectTypes.h"

// 반드시 마지막 include
#include "RuneSystemComponent.generated.h"

//  FRuneStat를 먼저 정의
USTRUCT(BlueprintType)
struct FRuneStat
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Power = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CooldownReduction = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Element;

    bool operator==(const FRuneStat& Other) const
    {
        return FMath::IsNearlyEqual(Power, Other.Power)
            && FMath::IsNearlyEqual(CooldownReduction, Other.CooldownReduction)
            && Element == Other.Element;
    }

    bool IsBetterThan(const FRuneStat& Other) const
    {
        if (Power > Other.Power) return true;
        if (FMath::IsNearlyEqual(Power, Other.Power) && CooldownReduction > Other.CooldownReduction) return true;
        return false;
    }
};

// 그 다음에 FRuneSlot
USTRUCT(BlueprintType)
struct FRuneSlot
{
    GENERATED_BODY()

    UPROPERTY() FGameplayAbilitySpecHandle   AbilityHandle;
    UPROPERTY() FActiveGameplayEffectHandle  EffectHandle;

    UPROPERTY() FRuneStat Stat;            //  이제 UHT가 인식 가능
    UPROPERTY() FName     TemplateName { NAME_None };
};

// 3) 마지막에 UCLASS
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EMBER_API URuneSystemComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URuneSystemComponent();

    UPROPERTY(ReplicatedUsing = OnRep_RuneSlots, VisibleAnywhere, BlueprintReadOnly, Category = "Rune")
    TArray<FRuneSlot> RuneSlots;

    UPROPERTY(EditAnywhere, Category = "Rune", meta = (ClampMin = 1))
    int32 MaxRuneSlots = 1;

    UFUNCTION(BlueprintCallable, Category = "Rune")
    bool EquipRuneFromTemplate(const URuneItemTemplate* Template,
        int32 PreferredSlotIndex,
        int32& OutResolvedSlotIndex);


    UFUNCTION(BlueprintCallable, Category = "Rune")
    bool RemoveRune(int32 SlotIndex);

    UFUNCTION(BlueprintPure, Category = "Rune")
    FRuneStat GetRuneStatAtSlot(int32 SlotIndex) const;

    UFUNCTION(BlueprintPure, Category = "Rune")
    FName GetRuneTemplateName(int32 SlotIndex) const;

    UFUNCTION(BlueprintPure, Category = "Rune")
    bool IsBetterRune(int32 SlotIndex, const FRuneStat& NewStat) const;

protected:
    UFUNCTION(Server, Reliable)
    void Server_EquipRuneFromTemplate(const class URuneItemTemplate* Template, int32 PreferredSlotIndex);

    bool EquipRuneFromTemplate_Internal(const class URuneItemTemplate* Template, int32 SlotIndex);
    int32 ChooseSlotIndexFor(const FRuneStat& NewStat, int32 PreferredSlotIndex) const;

    UFUNCTION()
    void OnRep_RuneSlots();

    class UAbilitySystemComponent* GetASC() const;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};