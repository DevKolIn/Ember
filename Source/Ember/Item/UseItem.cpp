#include "UseItem.h"
#include "GameFramework/Actor.h"
#include "Character/EmberCharacter.h" // 캐릭터가 Heal() 함수를 갖고 있다고 가정

AUseItem::AUseItem()
{
    // 소비 아이템 설정 가능
}

void AUseItem::Use(AActor* User)
{
    if (AEmberCharacter* Character = Cast<AEmberCharacter>(User))
    {
        //Character->Heal(HealAmount); // 체력 회복
        Destroy(); // 아이템 제거
    }
}