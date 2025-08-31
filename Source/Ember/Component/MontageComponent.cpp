#include "Component/MontageComponent.h"

#include "GameFramework/Character.h"
#include "Tag/EmberGameplayTag.h"
#include "Utility/CLog.h"

UMontageComponent::UMontageComponent()
{

}

// Called when the game starts
void UMontageComponent::BeginPlay()
{
	Super::BeginPlay();

	if (DataTable == nullptr)
	{
		DebugLogE("Data Table is not selected");
		return;
	}

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	DataTable->GetAllRows<FMontageData>("", MontageData);
}

FMontageData* UMontageComponent::GetHitData()
{
	for (auto data : MontageData)
		if (data != nullptr && data->StateTag == ABTAG_DATA_HIT)
			return data;
	return nullptr;
}

FMontageData* UMontageComponent::GetDeadData()
{
	for (auto data : MontageData)
		if (data != nullptr && data->StateTag == ABTAG_DATA_Dead)
			return data;
	return nullptr;
}

FMontageData* UMontageComponent::GetAvoidData(FGameplayTag InTag)
{
	for (auto data : MontageData)
		if (data != nullptr && data->StateTag == InTag)
			return data;
	return nullptr;
}

