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
    // 룬 장착 동안 상시 유지 → 해제 시 RemoveActiveGameplayEffect 로 제거
    DurationPolicy = EGameplayEffectDurationType::Infinite;

    // ---- AttackDamage += SetByCaller(Data.Power)
    {
        FGameplayModifierInfo Mod;   // ← 여기! FGameplayModifierInfo 가 맞습니다.

        FSetByCallerFloat SBCPower;
        SBCPower.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Power"));

        Mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(SBCPower);
        Mod.ModifierOp = EGameplayModOp::Additive;
        Mod.Attribute = UEmberAS_Player::GetAttackDamageAttribute();

        Modifiers.Add(Mod);
    }

    // ---- (옵션) CooldownReduction += SetByCaller(Data.CDR)
    // UEmberAS_Player에 해당 Attribute가 있을 때만 주석 해제하세요.
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
