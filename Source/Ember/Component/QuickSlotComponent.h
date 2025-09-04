#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Template/ItemTemplate.h"
#include "QuickSlotComponent.generated.h"

class URuneItemTemplate;
class AEmberCharacter;          // 전방선언 추가
class UAbilitySystemComponent;  // (ASC로 쓰실 경우)

USTRUCT(BlueprintType)
struct FQuickSlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UItemTemplate> ItemTemplateClass;  // 그대로 유지 (BP 클래스 CDO 사용)

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity = 0;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EMBER_API UQuickSlotComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UQuickSlotComponent();

    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void AddItemToQuickSlot(TSubclassOf<UItemTemplate> ItemTemplateClass, int32 Quantity);

    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void AddRuneToQuickSlot(TSubclassOf<URuneItemTemplate> RuneTemplateClass, int32 Quantity);

    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void UseQuickSlot(int32 Index);

    const FQuickSlot* GetSlot(int32 Index) const;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuickSlot")
    TArray<FQuickSlot> QuickSlots;

    UPROPERTY(EditDefaultsOnly, Category = "QuickSlot")
    int32 MaxSlots = 5;

    int32 FindSuitableSlot(TSubclassOf<UItemTemplate> ItemClass);
    void LogQuickSlotState() const;

    // 서버에서 실제 처리
    UFUNCTION(Server, Reliable)
    void Server_UseQuickSlot(int32 Index);

    // 거부 피드백(선택)
    UFUNCTION(Client, Unreliable)
    void Client_NotifyUseDenied(const FString& Reason);

    void ConsumeOne(int32 Index);

private:
    UPROPERTY() AEmberCharacter* OwnerChar = nullptr;
    UPROPERTY() TMap<TSubclassOf<URuneItemTemplate>, int32> RuneStacks;
};
