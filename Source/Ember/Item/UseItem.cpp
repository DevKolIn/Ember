#include "UseItem.h"
#include "GameFramework/Actor.h"
#include "Character/EmberCharacter.h" // ĳ���Ͱ� Heal() �Լ��� ���� �ִٰ� ����

AUseItem::AUseItem()
{
    // �Һ� ������ ���� ����
}

void AUseItem::Use(AActor* User)
{
    if (AEmberCharacter* Character = Cast<AEmberCharacter>(User))
    {
        //Character->Heal(HealAmount); // ü�� ȸ��
        Destroy(); // ������ ����
    }
}