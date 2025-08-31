#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h" // 또는 상속받는 아이템 베이스 클래스
#include "GameplayAbilitySpec.h"
#include "GameplayEffectTypes.h"
#include "Component/RuneSystemComponent.h"
#include "Template/RuneItemTemplate.h"
#include "RuneItem.generated.h"


UCLASS()
class EMBER_API ARuneItem : public ABaseItem
{
    GENERATED_BODY()

public:
    ARuneItem();
    void Use(AActor* User);
    void InitializeFromTemplate(URuneItemTemplate* InTemplate);
    FString GetName() const;
    const FRuneStat& GetRuneStat() const { return RuneStat; }
    TSubclassOf<UGameplayAbility> GetGrantedAbility() const { return GrantedAbility; }
    TSubclassOf<UGameplayEffect> GetGrantedEffect() const { return GrantedEffect; }
    URuneItemTemplate* GetRuneTemplate() const { return TemplateAsset; }
protected:
    UPROPERTY()
    TObjectPtr<URuneItemTemplate> TemplateAsset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rune")
    TSubclassOf<UGameplayAbility> GrantedAbility;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rune")
    TSubclassOf<UGameplayEffect> GrantedEffect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rune")
    FRuneStat RuneStat;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rune")
    FText RuneName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rune")
    FText Description;

    // 이펙트를 표시할 컴포넌트 (필요 시)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
    UNiagaraComponent* DropEffectComponent;
};
