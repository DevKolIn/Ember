#include "RuneSystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "Template/RuneItemTemplate.h"   // 템플릿은 CPP에서만 include
#include "GameFramework/Actor.h"
#include "Character/EmberCharacter.h"   //  캐릭터의 GetASC() 호출용
#include "GAS/Attribute/EmberAS_Player.h"   // UEmberAS_Player
#include "AbilitySystemComponent.h"          // UAbilitySystemComponent, GetNumericAttribute

namespace RuneSBC
{
    static FGameplayTag Power = FGameplayTag::RequestGameplayTag(TEXT("Data.Power"));
    static FGameplayTag CDR = FGameplayTag::RequestGameplayTag(TEXT("Data.CDR"));
}
URuneSystemComponent::URuneSystemComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
    RuneSlots.SetNum(MaxRuneSlots);
}

void URuneSystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(URuneSystemComponent, RuneSlots);
}

void URuneSystemComponent::OnRep_RuneSlots()
{
    UE_LOG(LogTemp, Verbose, TEXT("[OnRep_RuneSlots]"));
}

UAbilitySystemComponent* URuneSystemComponent::GetASC() const
{
    if (const AEmberCharacter* Char = Cast<AEmberCharacter>(GetOwner()))
    {
        // ✅ 프로젝트의 캐릭터에 있는 GetASC() 우선 사용
        if (UAbilitySystemComponent* FromChar = Char->GetASC())
        {
            return FromChar;
        }
    }

    // ✅ 그 외에는 컴포넌트로 붙은 ASC 탐색
    if (const AActor* Owner = GetOwner())
    {

        if (UActorComponent* Comp = Owner->FindComponentByClass(UAbilitySystemComponent::StaticClass()))
        {
            return Cast<UAbilitySystemComponent>(Comp);
        }
    }

    return nullptr;
}

FRuneStat URuneSystemComponent::GetRuneStatAtSlot(int32 SlotIndex) const
{
    return RuneSlots.IsValidIndex(SlotIndex) ? RuneSlots[SlotIndex].Stat : FRuneStat();
}

FName URuneSystemComponent::GetRuneTemplateName(int32 SlotIndex) const
{
    return RuneSlots.IsValidIndex(SlotIndex) ? RuneSlots[SlotIndex].TemplateName : NAME_None;
}

bool URuneSystemComponent::IsBetterRune(int32 SlotIndex, const FRuneStat& NewStat) const
{
    if (!RuneSlots.IsValidIndex(SlotIndex)) return false;
    const FRuneSlot& S = RuneSlots[SlotIndex];
    if (S.TemplateName.IsNone()) return true;  // 빈 슬롯이면 교체 OK
    return NewStat.IsBetterThan(S.Stat);
}

int32 URuneSystemComponent::ChooseSlotIndexFor(const FRuneStat& NewStat, int32 PreferredSlotIndex) const
{
    // 1) 선호 슬롯이 비어있으면 그 슬롯
    if (RuneSlots.IsValidIndex(PreferredSlotIndex) && GetRuneTemplateName(PreferredSlotIndex).IsNone())
        return PreferredSlotIndex;

    // 2) 빈 슬롯
    for (int32 i = 0; i < RuneSlots.Num(); ++i)
        if (RuneSlots[i].TemplateName.IsNone())
            return i;

    // 3) 교체 가능 슬롯
    for (int32 i = 0; i < RuneSlots.Num(); ++i)
        if (IsBetterRune(i, NewStat))
            return i;

    return INDEX_NONE;
}

bool URuneSystemComponent::RemoveRune(int32 SlotIndex)
{
    if (!RuneSlots.IsValidIndex(SlotIndex)) return false;

    if (UAbilitySystemComponent* ASC = GetASC())
    {
        FRuneSlot& Slot = RuneSlots[SlotIndex];

        if (Slot.AbilityHandle.IsValid())
        {
            ASC->ClearAbility(Slot.AbilityHandle);
            Slot.AbilityHandle = FGameplayAbilitySpecHandle();
        }

        if (Slot.EffectHandle.IsValid())
        {
            ASC->RemoveActiveGameplayEffect(Slot.EffectHandle);
            Slot.EffectHandle = FActiveGameplayEffectHandle();
        }
    }

    RuneSlots[SlotIndex] = FRuneSlot(); // 초기화
    return true;
}


bool URuneSystemComponent::EquipRuneFromTemplate(const URuneItemTemplate* Template,
    int32 PreferredSlotIndex,
    int32& OutResolvedSlotIndex)
{
    UE_LOG(LogTemp, Log, TEXT("[EquipRuneFromTemplate] HasAuth=%d Template=%s PrefSlot=%d"),
        GetOwner() ? GetOwner()->HasAuthority() : -1,
        Template ? *Template->GetName() : TEXT("NULL"),
        PreferredSlotIndex);

    OutResolvedSlotIndex = INDEX_NONE;

    if (!Template) return false;

    if (AActor* Owner = GetOwner())
    {
        if (!Owner->HasAuthority())
        {
            Server_EquipRuneFromTemplate(Template, PreferredSlotIndex);
            return false; // 클라에서는 확정 못함 → false 반환
        }
    }

    // 템플릿 → FRuneStat 조립
    FRuneStat NewStat;
    NewStat.Power = Template->BasePower;
    NewStat.CooldownReduction = Template->BaseCooldownReduction;
    NewStat.Element = Template->BaseElement;

    const int32 SlotIdx = ChooseSlotIndexFor(NewStat, PreferredSlotIndex);
    if (SlotIdx == INDEX_NONE) return false;

    const bool bOK = EquipRuneFromTemplate_Internal(Template, SlotIdx);
    if (bOK) OutResolvedSlotIndex = SlotIdx;

    return bOK;
}


void URuneSystemComponent::Server_EquipRuneFromTemplate_Implementation(const URuneItemTemplate* Template, int32 PreferredSlotIndex)
{
    if (!Template) return;

    FRuneStat NewStat;
    NewStat.Power = Template->BasePower;
    NewStat.CooldownReduction = Template->BaseCooldownReduction;
    NewStat.Element = Template->BaseElement;

    const int32 SlotIdx = ChooseSlotIndexFor(NewStat, PreferredSlotIndex);
    if (SlotIdx == INDEX_NONE) return;

    EquipRuneFromTemplate_Internal(Template, SlotIdx);
}

bool URuneSystemComponent::EquipRuneFromTemplate_Internal(const URuneItemTemplate* Template, int32 SlotIndex)
{
    if (!Template || !RuneSlots.IsValidIndex(SlotIndex)) return false;

    if (UAbilitySystemComponent* ASC = GetASC())
    {
        FRuneSlot& Slot = RuneSlots[SlotIndex];

        // 0) 기존 효과/어빌리티 제거
        if (Slot.AbilityHandle.IsValid())
        {
            ASC->ClearAbility(Slot.AbilityHandle);
            Slot.AbilityHandle = FGameplayAbilitySpecHandle();
        }
        if (Slot.EffectHandle.IsValid())
        {
            ASC->RemoveActiveGameplayEffect(Slot.EffectHandle);
            Slot.EffectHandle = FActiveGameplayEffectHandle();
        }

        // 1) (옵션) Ability 부여
        if (Template->GrantedAbility)
        {
            FGameplayAbilitySpec GivenSpec(Template->GrantedAbility, 1);
            Slot.AbilityHandle = ASC->GiveAbility(GivenSpec);
        }

        // 2) 적용 전 스냅샷
        const FGameplayAttribute ADAttr = UEmberAS_Player::GetAttackDamageAttribute();
        const float OldAD = ASC->GetNumericAttribute(ADAttr);

        // 3) GE 적용 (SetByCaller: Data.Power)
        if (Template->GrantedEffect)
        {
            FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
            if (AActor* Owner = GetOwner()) { Ctx.AddInstigator(Owner, Owner); }

            FGameplayEffectSpecHandle SpecH = ASC->MakeOutgoingSpec(Template->GrantedEffect, /*Level=*/1.f, Ctx);
            if (!SpecH.IsValid())
            {
                UE_LOG(LogTemp, Error, TEXT("[Rune] MakeOutgoingSpec failed (GE=%s)"), *GetNameSafe(Template->GrantedEffect));
                return false;
            }

            FGameplayEffectSpec* Spec = SpecH.Data.Get();

            // ★ SetByCaller 주입 (Tag/Name 둘 다 — BP가 Tag/Name 중 무엇을 쓰든 대응)
            Spec->SetSetByCallerMagnitude(RuneSBC::Power, Template->BasePower);
            Spec->SetSetByCallerMagnitude(FName(TEXT("Data.Power")), Template->BasePower);

            // 주입 검증 로그
            const float InTag = Spec->GetSetByCallerMagnitude(RuneSBC::Power, /*WarnIfNotFound=*/false, /*DefaultIfNotFound=*/-999.f);
            const float InName = Spec->GetSetByCallerMagnitude(FName(TEXT("Data.Power")), /*WarnIfNotFound=*/false, /*DefaultIfNotFound=*/-999.f);
            UE_LOG(LogTemp, Warning, TEXT("[Rune] SBC in Spec — Tag:%s=%.2f  Name:Data.Power=%.2f"),
                *RuneSBC::Power.ToString(), InTag, InName);


            // 적용
            FActiveGameplayEffectHandle Handle = ASC->ApplyGameplayEffectSpecToSelf(*Spec);
            const bool bApplied = Handle.IsValid();
            Slot.EffectHandle = Handle;

            UE_LOG(LogTemp, Warning, TEXT("[Rune] ApplyGameplayEffectSpecToSelf %s (HandleValid=%d)"),
                bApplied ? TEXT("SUCCEEDED") : TEXT("FAILED"), bApplied);

            if (!bApplied)
            {
                UE_LOG(LogTemp, Error, TEXT("[Rune] GE application FAILED — check Required Tags / Immunity / Requirements in BP_GE_RuneBase"));
                return false;
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[Rune] No GrantedEffect on Template %s"), *GetNameSafe(Template));
            return false;
        }

        // 4) 슬롯 메타 업데이트
        Slot.Stat.Power = Template->BasePower;
        Slot.Stat.CooldownReduction = Template->BaseCooldownReduction;
        Slot.Stat.Element = Template->BaseElement;
        Slot.TemplateName = FName(*Template->RuneName.ToString());

        // 5) 적용 후 스냅샷
        const float NewAD = ASC->GetNumericAttribute(ADAttr);
        UE_LOG(LogTemp, Warning, TEXT("[Rune Equipped] %s | Slot=%d  AD: %+0.1f (Now: %.1f)"),
            *Template->RuneName.ToString(), SlotIndex, (NewAD - OldAD), NewAD);

        return true;
    }

    UE_LOG(LogTemp, Warning, TEXT("[Rune] Equip failed: ASC missing on %s"), *GetNameSafe(GetOwner()));
    return false;
}