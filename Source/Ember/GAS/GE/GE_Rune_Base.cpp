#include "GE_Rune_Base.h"
#include "GAS/Attribute/EmberAS_Player.h"   // AttackDamage Attribute
#include "GameplayEffect.h"                 // FGameplayEffectModifierInfo, EGameplayModOp
#include "GameplayEffectTypes.h"            // FSetByCallerFloat
#include "GameplayTagContainer.h"           // FGameplayTag
#include "GameplayTagsManager.h"            // RequestGameplayTag

#include "GE_Rune_Base.h"

#include "GAS/Attribute/EmberAS_Player.h"   // UEmberAS_Player::GetAttackDamageAttribute()
#include "GameplayEffect.h"                 // UGameplayEffect, EGameplayModOp
#include "GameplayEffectTypes.h"            // FGameplayEffectModifierMagnitude, FSetByCallerFloat
#include "GameplayTagContainer.h"           // FGameplayTag
#include "GameplayTagsManager.h"            // FGameplayTag::RequestGameplayTag

UGE_Rune_Base::UGE_Rune_Base()
{
    // �� ���� ���� ��� ���� �� ���� �� RemoveActiveGameplayEffect �� ����
    DurationPolicy = EGameplayEffectDurationType::Infinite;

    // ---- AttackDamage += SetByCaller(Data.Power)
    {
        FGameplayModifierInfo Mod;   // �� ����! FGameplayModifierInfo �� �½��ϴ�.

        FSetByCallerFloat SBCPower;
        SBCPower.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Power"));

        Mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(SBCPower);
        Mod.ModifierOp = EGameplayModOp::Additive;
        Mod.Attribute = UEmberAS_Player::GetAttackDamageAttribute();

        Modifiers.Add(Mod);
    }

    // ---- (�ɼ�) CooldownReduction += SetByCaller(Data.CDR)
    // UEmberAS_Player�� �ش� Attribute�� ���� ���� �ּ� �����ϼ���.
    /*
    {
        FGameplayModifierInfo ModCDR;

        FSetByCallerFloat SBCCdr;
        SBCCdr.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Data.CDR"));

        ModCDR.ModifierMagnitude = FGameplayEffectModifierMagnitude(SBCCdr);
        ModCDR.ModifierOp        = EGameplayModOp::Additive;
        ModCDR.Attribute         = UEmberAS_Player::GetCooldownReductionAttribute();

        Modifiers.Add(ModCDR);
    }
    */
}
