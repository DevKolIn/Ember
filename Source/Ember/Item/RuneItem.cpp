#include "RuneItem.h"
#include "Template/RuneItemTemplate.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Character/EmberCharacter.h"
#include "Component/RuneSystemComponent.h"

ARuneItem::ARuneItem()
{
    // Constructor에서 특이점 없으면 생략 가능
}

void ARuneItem::Use(AActor* User)
{
    if (AEmberCharacter* Character = Cast<AEmberCharacter>(User))
    {
        if (const URuneItemTemplate* Template = GetRuneTemplate())
        {
            const bool bWasServer = HasAuthority();

            // 클라면 서버 RPC로 넘어가고, 서버면 바로 처리됩니다.
            // 자동 슬롯 선택: PreferredSlotIndex = -1
            Character->TryEquipRune(Template, -1);

            // 파괴는 반드시 서버에서만!
            if (bWasServer)
            {
                Destroy();
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[RuneItem] No Template set."));
        }
    }
}
void ARuneItem::InitializeFromTemplate(URuneItemTemplate* InTemplate)
{
    if (!InTemplate) return;

    TemplateAsset = InTemplate;

    // Ability / Effect
    GrantedAbility = InTemplate->GrantedAbility;
    GrantedEffect = InTemplate->GrantedEffect;

    // 템플릿의 '원시 필드'를 사용해서 RuneStat 조립 (FRuneStat 그대로 쓰는 구조)
    RuneStat.Power = InTemplate->BasePower;
    RuneStat.CooldownReduction = InTemplate->BaseCooldownReduction;
    RuneStat.Element = InTemplate->BaseElement;

    // 표시용 텍스트
    RuneName = InTemplate->RuneName;
    Description = InTemplate->Description;

    // (옵션) 비주얼 이펙트
    if (InTemplate->DropEffect)
    {
        UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
            InTemplate->DropEffect,
            GetRootComponent(),
            NAME_None,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::SnapToTargetIncludingScale,
            true
        );
        DropEffectComponent = NiagaraComp;
    }

    UE_LOG(LogTemp, Log, TEXT("[RuneItem] Initialized: %s"), *RuneName.ToString());
}

FString ARuneItem::GetName() const
{
    return GetNameSafe(this->GetClass());
}
