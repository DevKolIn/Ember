#include "BaseItem.h"
#include "Character/EmberCharacter.h"
#include "Item/Drop/PickupItemActor.h"
#include "Component/InteractionComponent.h"
#include "Components/StaticMeshComponent.h"


ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;

	InteractionComp = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	// 네트워크 상호작용을 위한 설정
	SetReplicates(true);
}
void ABaseItem::Use(AActor* User)
{
    // 인벤토리에서 아이템을 '사용'할 때만 진입
    AEmberCharacter* Character = Cast<AEmberCharacter>(User);
    if (!Character)
    {
        return;
    }

    // ⚠️ 주의: 월드에서 '줍기'는 여기서 처리하지 않습니다.
    // APickupItemActor::OnPickedUp에서 서버 권한으로만 수행하세요.
    // (형제 타입 간 Cast<APickupItemActor>(this) 같은 코드는 제거)

    // 기본 동작 없음 — 파생 클래스에서 오버라이드하여 실제 효과를 구현하세요.
    // 예: ARuneItem::Use에서 TryEquipRune(Template, -1) 호출
    UE_LOG(LogTemp, Verbose, TEXT("ABaseItem::Use: %s (inventory use only)"), *GetName());
}