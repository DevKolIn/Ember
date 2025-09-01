#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "UseItem.generated.h"

UCLASS()
class EMBER_API AUseItem : public ABaseItem
{
    GENERATED_BODY()

public:
    AUseItem();

    virtual void Use(AActor* User) override;

protected:
    UPROPERTY(EditAnywhere, Category = "Effect")
    float HealAmount = 30.0f;
};
