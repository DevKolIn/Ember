#include "MonsterAITemplate.h"

#include "Kismet/KismetMathLibrary.h"

UMonsterAITemplate::UMonsterAITemplate(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

UAnimMontage* UMonsterAITemplate::GetHitMontage(AActor* HitActor, const FVector& HitLocation) const
{
	UAnimMontage* SelectedMontage = nullptr;

	if (HitActor)
	{
		FVector CharacterLocation = HitActor->GetActorLocation();
		FVector CharacterDirection = HitActor->GetActorForwardVector();

		FRotator FacingRotator = UKismetMathLibrary::Conv_VectorToRotator(CharacterDirection);
		FRotator CharacterToHitRotator = UKismetMathLibrary::Conv_VectorToRotator((HitLocation - CharacterLocation).GetSafeNormal());
			
		FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(CharacterToHitRotator, FacingRotator);
		float YawAbs = FMath::Abs(DeltaRotator.Yaw);

		if (YawAbs < 60.f)
		{
			SelectedMontage = FrontHitMontage;
		}
		else if (YawAbs > 120.f)
		{
			SelectedMontage = BackHitMontage;
		}
		else if (DeltaRotator.Yaw < 0.f)
		{
			SelectedMontage = LeftHitMontage;
		}
		else
		{
			SelectedMontage = RightHitMontage;
		}
	}

	return SelectedMontage;
}

EDataValidationResult UMonsterAITemplate::IsDataValid(FDataValidationContext& Context) const
{
	return UObject::IsDataValid(Context);
}
