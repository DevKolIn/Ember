#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_Rune_Base.generated.h"

/** �� ���� GE: AttackDamage += SetByCaller(Data.Power)
 *  (���ϸ� CDR �� �߰� ����)
 */
UCLASS()
class EMBER_API UGE_Rune_Base : public UGameplayEffect
{
    GENERATED_BODY()
public:
    UGE_Rune_Base();
};
