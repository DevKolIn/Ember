// ItemEffectLibrary.cpp
#include "ItemEffectLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attribute/EmberAS_Player.h"
#include "GameFramework/Actor.h"
#include "Character/EmberCharacter.h"   // 실제 경로에 맞게 추가

static bool NearlyGEQ(float A, float B) { return A >= B - KINDA_SMALL_NUMBER; }

// ⚡️ EmberAS_Player에 맞춰 체력/체온만 읽도록 변경
static void ReadVitals(UAbilitySystemComponent* ASC,
    float& CurHP, float& MaxHP, float& CurTemp, float& MaxTemp)
{
    CurHP = ASC->GetNumericAttribute(UEmberAS_Player::GetHealthAttribute());
    MaxHP = ASC->GetNumericAttribute(UEmberAS_Player::GetMaxHealthAttribute());
    CurTemp = ASC->GetNumericAttribute(UEmberAS_Player::GetPlayerTemperatureAttribute());
    MaxTemp = ASC->GetNumericAttribute(UEmberAS_Player::GetMaxPlayerTemperatureAttribute());
}

bool UItemEffectLibrary::CanUseItem(const UItemTemplate* Template, AEmberCharacter* Character, FString& OutReason)
{
    OutReason.Reset();
    if (!Template || !Character) { OutReason = TEXT("Invalid"); return false; }

    UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
    if (!ASC) { OutReason = TEXT("No ASC"); return false; }

    float CurHP, MaxHP, CurTemp, MaxTemp;
    ReadVitals(ASC, CurHP, MaxHP, CurTemp, MaxTemp);

    bool bAnyWillApply = false;

    for (const FItemEffectSpec& E : Template->Effects)
    {
        switch (E.Type)
        {
        case EItemEffectKind::RestoreHP:
            if (!NearlyGEQ(CurHP, MaxHP)) bAnyWillApply = true;
            break;

        case EItemEffectKind::RestoreBodyTemp:
            if (!NearlyGEQ(CurTemp, MaxTemp)) bAnyWillApply = true;
            break;

        case EItemEffectKind::Buff_StatHold:
        case EItemEffectKind::Buff_MaxStat:
        case EItemEffectKind::RandomDebuff:
        case EItemEffectKind::CastSpell:
            bAnyWillApply = true; // 버프/디버프/스킬은 항상 적용된다고 간주
            break;
        }

        if (bAnyWillApply) break;
    }

    if (Template->bConsumeOnlyIfAnyEffectApplied && !bAnyWillApply)
    {
        OutReason = TEXT("최대치라 적용될 효과가 없습니다.");
        return false;
    }
    return true;
}

bool UItemEffectLibrary::ApplyItemEffects(const UItemTemplate* Template, AEmberCharacter* Character, bool bServerAuthoritative)
{
    if (!Template || !Character) return false;

    UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
    if (!ASC) return false;

    float CurHP, MaxHP, CurTemp, MaxTemp;
    ReadVitals(ASC, CurHP, MaxHP, CurTemp, MaxTemp);

    bool bAppliedAny = false;

    for (const FItemEffectSpec& E : Template->Effects)
    {
        if (FMath::FRand() > E.Chance) continue;

        switch (E.Type)
        {
        case EItemEffectKind::RestoreHP:
            if (CurHP < MaxHP)
            {
                const float NewHP = FMath::Clamp(CurHP + E.Magnitude, 0.f, MaxHP);
                ASC->SetNumericAttributeBase(UEmberAS_Player::GetHealthAttribute(), NewHP);
                CurHP = NewHP;
                bAppliedAny = true;
            }
            break;

        case EItemEffectKind::RestoreBodyTemp:
            if (CurTemp < MaxTemp)
            {
                const float NewTemp = FMath::Clamp(CurTemp + E.Magnitude, 0.f, MaxTemp);
                ASC->SetNumericAttributeBase(UEmberAS_Player::GetPlayerTemperatureAttribute(), NewTemp);
                CurTemp = NewTemp;
                bAppliedAny = true;
            }
            break;

        case EItemEffectKind::Buff_StatHold:
            // TODO: 태그/GE로 버프 적용
            bAppliedAny = true;
            break;

        case EItemEffectKind::Buff_MaxStat:
            // TODO: MaxHealth 같은 스탯 확장 GE 적용
            bAppliedAny = true;
            break;

        case EItemEffectKind::RandomDebuff:
            // TODO: Debuff GE 적용
            bAppliedAny = true;
            break;

        case EItemEffectKind::CastSpell:
            // TODO: Ability/이펙트 트리거
            bAppliedAny = true;
            break;
        }
    }

    return bAppliedAny;
}
