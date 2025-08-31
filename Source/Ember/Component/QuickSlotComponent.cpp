#include "QuickSlotComponent.h"
#include "Template/RuneItemTemplate.h"     
UQuickSlotComponent::UQuickSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	QuickSlots.SetNum(MaxSlots); // ���� �ʱ�ȭ
}

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

void UQuickSlotComponent::AddItemToQuickSlot(TSubclassOf<UItemTemplate> ItemClass, int32 Quantity)
{
	if (!ItemClass || Quantity <= 0) return;

	int32 Index = FindSuitableSlot(ItemClass);
	if (Index != INDEX_NONE)
	{
		FQuickSlot& Slot = QuickSlots[Index];
		Slot.ItemTemplateClass = ItemClass;
		Slot.Quantity += Quantity;
		FText DisplayName = ItemClass->GetDefaultObject<UItemTemplate>()->DisplayName;
		//  �α� ���: � �������� �� �� ���Կ� �� �� ������
		UE_LOG(LogTemp, Warning, TEXT("[QuickSlot] Added item: %s x%d to Slot[%d]"),
			*DisplayName.ToString(), Quantity, Index);

		//  UI ���� ���� (���⼭ ������ �ݿ����ִ� ������ ȣ���ϼ���)
		// UpdateQuickSlotUI(Index); // �� ����. ���� UI �Լ��� �°� ����

		LogQuickSlotState();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuickSlot] No available slot found for item: %s"),
			*ItemClass->GetName());
	}
}


void UQuickSlotComponent::UseQuickSlot(int32 Index)
{
	if (!QuickSlots.IsValidIndex(Index)) return;

	FQuickSlot& Slot = QuickSlots[Index];
	if (!Slot.ItemTemplateClass || Slot.Quantity <= 0) return;

	const UItemTemplate* Template = Slot.ItemTemplateClass->GetDefaultObject<UItemTemplate>();
	if (!Template) return;

	UE_LOG(LogTemp, Log, TEXT("[QuickSlot] Used item: %s from Slot[%d] (Before: %d)"),
		*Template->DisplayName.ToString(), Index, Slot.Quantity);

	// TODO: ���⿡ ���� ȿ�� ����(GAS) ���߿� �߰�
	// ex) ASC->ApplyGameplayEffectToSelf(Template->ConsumableEffect ...);

	Slot.Quantity = FMath::Max(0, Slot.Quantity - 1);
	if (Slot.Quantity <= 0)
	{
		Slot.ItemTemplateClass = nullptr;
		UE_LOG(LogTemp, Log, TEXT("[QuickSlot] Slot[%d] now empty"), Index);
	}

	// UI ���� ��
	// UpdateQuickSlotUI(Index);

	// ���� �α�
	LogQuickSlotState();
}
void UQuickSlotComponent::AddRuneToQuickSlot(TSubclassOf<URuneItemTemplate> RuneTemplateClass, int32 Quantity)
{
	if (!*RuneTemplateClass || Quantity <= 0) return;

	int32& Count = RuneStacks.FindOrAdd(RuneTemplateClass);
	Count += Quantity;

	UE_LOG(LogTemp, Log, TEXT("[QuickSlot] Added RUNE %s x%d (total=%d)"),
		*RuneTemplateClass->GetName(), Quantity, Count);
	// ���� �α�
	LogQuickSlotState();
	// TODO: ���⼭ UI ���� ��ε�ĳ��Ʈ�� �ִٸ� ȣ��
}


const FQuickSlot* UQuickSlotComponent::GetSlot(int32 Index) const
{
	return QuickSlots.IsValidIndex(Index) ? &QuickSlots[Index] : nullptr;
}


void UQuickSlotComponent::LogQuickSlotState() const
{
	UE_LOG(LogTemp, Warning, TEXT("===== Current QuickSlot State ====="));
	for (int32 i = 0; i < QuickSlots.Num(); ++i)
	{
		const FQuickSlot& Slot = QuickSlots[i];

		if (Slot.ItemTemplateClass)
		{
			const UItemTemplate* DefaultItem = Slot.ItemTemplateClass->GetDefaultObject<UItemTemplate>();
			FString ItemName = DefaultItem ? DefaultItem->DisplayName.ToString() : TEXT("Unknown");

			UE_LOG(LogTemp, Warning, TEXT("Slot[%d]: %s x%d"),
				i,
				*ItemName,
				Slot.Quantity);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Slot[%d]: (empty)"), i);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("===================================="));
}

