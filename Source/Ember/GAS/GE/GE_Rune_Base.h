#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_Rune_Base.generated.h"

/** 룬 공통 GE: AttackDamage += SetByCaller(Data.Power)
 *  (원하면 CDR 등 추가 가능)
 */
UCLASS()
class EMBER_API UGE_Rune_Base : public UGameplayEffect
{
    GENERATED_BODY()
public:
    UGE_Rune_Base();
};
