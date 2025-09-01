#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h" // �Ǵ� ��ӹ޴� ������ ���̽� Ŭ����
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

    // ����Ʈ�� ǥ���� ������Ʈ (�ʿ� ��)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
    UNiagaraComponent* DropEffectComponent;
};
