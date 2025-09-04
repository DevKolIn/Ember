#include "QuickSlotComponent.h"
#include "Template/ItemTemplate.h"
#include "Template/RuneItemTemplate.h"
#include "Item/Function/ItemEffectLibrary.h"            //   라이브러리
#include "Character/EmberCharacter.h"     //   실제 경로로 수정하세요
#include "Net/UnrealNetwork.h"

// -----------------------------------------------------
// 생성자
// -----------------------------------------------------
UQuickSlotComponent::UQuickSlotComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // (선택) 컴포넌트 자체 복제 여부 – RPC에는 필수 아님. 소유 액터가 Replicate면 충분.
    // SetIsReplicatedByDefault(true);

    QuickSlots.SetNum(MaxSlots); // 슬롯 초기화
}

// -----------------------------------------------------
// BeginPlay: 소유 캐릭터 캐시
// -----------------------------------------------------
void UQuickSlotComponent::BeginPlay()
{
    Super::BeginPlay();
    OwnerChar = Cast<AEmberCharacter>(GetOwner());
}

// -----------------------------------------------------
// 적합한 슬롯 찾기 (기존 그대로)
// -----------------------------------------------------
int32 UQuickSlotComponent::FindSuitableSlot(TSubclassOf<UItemTemplate> ItemClass)
{
    for (int32 i = 0; i < QuickSlots.Num(); ++i)
    {
        if (QuickSlots[i].ItemTemplateClass == ItemClass || QuickSlots[i].ItemTemplateClass == nullptr)
        {
            return i;
        }
    }
    return INDEX_NONE;
}

// -----------------------------------------------------
// 아이템 추가 (기존 로직 유지)
// -----------------------------------------------------
void UQuickSlotComponent::AddItemToQuickSlot(TSubclassOf<UItemTemplate> ItemClass, int32 Quantity)
{
    if (!ItemClass || Quantity <= 0) return;

    int32 Index = FindSuitableSlot(ItemClass);
    if (Index != INDEX_NONE)
    {
        FQuickSlot& Slot = QuickSlots[Index];
        Slot.ItemTemplateClass = ItemClass;
        Slot.Quantity += Quantity;

        const UItemTemplate* CDO = ItemClass->GetDefaultObject<UItemTemplate>();
        const FText DisplayName = CDO ? CDO->DisplayName : FText::FromString(TEXT("Unknown"));

        UE_LOG(LogTemp, Warning, TEXT("[QuickSlot] Added item: %s x%d to Slot[%d]"),
            *DisplayName.ToString(), Quantity, Index);

        // TODO: UpdateQuickSlotUI(Index);
        LogQuickSlotState();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[QuickSlot] No available slot found for item: %s"),
            *ItemClass->GetName());
    }
}

// -----------------------------------------------------
// 아이템 사용: 로컬 프리체크 → 서버 RPC
// -----------------------------------------------------
void UQuickSlotComponent::UseQuickSlot(int32 Index)
{
    if (!QuickSlots.IsValidIndex(Index) || !OwnerChar) return;

    const FQuickSlot& Slot = QuickSlots[Index];
    if (!Slot.ItemTemplateClass || Slot.Quantity <= 0) return;

    const UItemTemplate* Template = Slot.ItemTemplateClass->GetDefaultObject<UItemTemplate>();
    if (!Template) return;

    UE_LOG(LogTemp, Log, TEXT("[QuickSlot] Try use item: %s from Slot[%d] (Before: %d)"),
        *Template->DisplayName.ToString(), Index, Slot.Quantity);

    //   로컬 프리체크(UI 피드백)
    FString Reason;
    if (!UItemEffectLibrary::CanUseItem(Template, OwnerChar, Reason))
    {
        Client_NotifyUseDenied(Reason);
        return;
    }

    //   서버에서 실제 처리 (치트 방지)
    Server_UseQuickSlot(Index);
}

// -----------------------------------------------------
// 서버에서 실제 적용/소비
// -----------------------------------------------------
void UQuickSlotComponent::Server_UseQuickSlot_Implementation(int32 Index)
{
    if (!QuickSlots.IsValidIndex(Index) || !OwnerChar) return;

    FQuickSlot& Slot = QuickSlots[Index];
    if (!Slot.ItemTemplateClass || Slot.Quantity <= 0) return;

    const UItemTemplate* Template = Slot.ItemTemplateClass->GetDefaultObject<UItemTemplate>();
    if (!Template) return;

    // 서버 재검증
    FString Reason;
    const bool bCan = UItemEffectLibrary::CanUseItem(Template, OwnerChar, Reason);
    if (!bCan && Template->bConsumeOnlyIfAnyEffectApplied)
    {
        Client_NotifyUseDenied(Reason);
        return;
    }

    // 효과 적용
    const bool bApplied = UItemEffectLibrary::ApplyItemEffects(Template, OwnerChar, /*bServerAuthoritative*/true);

    // 소비 규칙
    if (bApplied || !Template->bConsumeOnlyIfAnyEffectApplied)
    {
        ConsumeOne(Index);
        LogQuickSlotState();
        // TODO: Multicast_PlayUseFX(Index);
    }
    else
    {
        Client_NotifyUseDenied(TEXT("적용될 효과가 없습니다."));
    }
}

// -----------------------------------------------------
// 클라 피드백
// -----------------------------------------------------
void UQuickSlotComponent::Client_NotifyUseDenied_Implementation(const FString& Reason)
{
    UE_LOG(LogTemp, Warning, TEXT("아이템 사용 불가: %s"), *Reason);
    // TODO: UI 토스트/사운드/진동
}

// -----------------------------------------------------
// 1개 소비 및 비우기
// -----------------------------------------------------
void UQuickSlotComponent::ConsumeOne(int32 Index)
{
    if (!QuickSlots.IsValidIndex(Index)) return;
    FQuickSlot& Slot = QuickSlots[Index];

    Slot.Quantity = FMath::Max(0, Slot.Quantity - 1);
    if (Slot.Quantity <= 0)
    {
        Slot.ItemTemplateClass = nullptr;
        UE_LOG(LogTemp, Log, TEXT("[QuickSlot] Slot[%d] now empty"), Index);
    }

    // TODO: UpdateQuickSlotUI(Index);
}

// -----------------------------------------------------
// 룬 추가 (기존 유지)
// -----------------------------------------------------
void UQuickSlotComponent::AddRuneToQuickSlot(TSubclassOf<URuneItemTemplate> RuneTemplateClass, int32 Quantity)
{
    if (!*RuneTemplateClass || Quantity <= 0) return;

    int32& Count = RuneStacks.FindOrAdd(RuneTemplateClass);
    Count += Quantity;

    UE_LOG(LogTemp, Log, TEXT("[QuickSlot] Added RUNE %s x%d (total=%d)"),
        *RuneTemplateClass->GetName(), Quantity, Count);

    LogQuickSlotState();
    // TODO: UI 갱신 브로드캐스트
}

// -----------------------------------------------------
// 슬롯 조회 (기존 유지)
// -----------------------------------------------------
const FQuickSlot* UQuickSlotComponent::GetSlot(int32 Index) const
{
    return QuickSlots.IsValidIndex(Index) ? &QuickSlots[Index] : nullptr;
}

// -----------------------------------------------------
// 상태 로그 (기존 유지)
// -----------------------------------------------------
void UQuickSlotComponent::LogQuickSlotState() const
{
    UE_LOG(LogTemp, Warning, TEXT("===== Current QuickSlot State ====="));
    for (int32 i = 0; i < QuickSlots.Num(); ++i)
    {
        const FQuickSlot& Slot = QuickSlots[i];

        if (Slot.ItemTemplateClass)
        {
            const UItemTemplate* DefaultItem = Slot.ItemTemplateClass->GetDefaultObject<UItemTemplate>();
            const FString ItemName = DefaultItem ? DefaultItem->DisplayName.ToString() : TEXT("Unknown");

            UE_LOG(LogTemp, Warning, TEXT("Slot[%d]: %s x%d"), i, *ItemName, Slot.Quantity);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Slot[%d]: (empty)"), i);
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("===================================="));
}
