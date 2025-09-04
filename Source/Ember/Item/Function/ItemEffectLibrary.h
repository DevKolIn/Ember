// ItemEffectLibrary.h
#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Template/ItemTemplate.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemEffectLibrary.generated.h"

/**
 * 
 */
class AEmberCharacter;
class UAbilitySystemComponent;

UCLASS()
class UItemEffectLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Item")
    static bool CanUseItem(const UItemTemplate* Template, AEmberCharacter* Character, FString& OutReason);

    UFUNCTION(BlueprintCallable, Category = "Item")
    static bool ApplyItemEffects(const UItemTemplate* Template, AEmberCharacter* Character, bool bServerAuthoritative = true);
};
