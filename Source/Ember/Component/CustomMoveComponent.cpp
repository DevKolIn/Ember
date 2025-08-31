// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CustomMoveComponent.h"

#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/CLog.h"

// Sets default values for this component's properties
UCustomMoveComponent::UCustomMoveComponent()
{
}

void UCustomMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter == nullptr)
	{
		DebugLogE("Owner Character is null");
		return;
	}
}

void UCustomMoveComponent::Move(const FInputActionValue& Value)
{
	FVector2D moveVector = Value.Get<FVector2D>();
	if (OwnerCharacter->GetController() == nullptr)
	{
		DebugLogE("controller is null");
		return;
	}

	const FRotator rotation = OwnerCharacter->GetController()->GetControlRotation();
	const FRotator yaw(0,rotation.Yaw,0);
	const FVector foward = FRotationMatrix(yaw).GetUnitAxis(EAxis::X);
	const FVector right = FRotationMatrix(yaw).GetUnitAxis(EAxis::Y);

	OwnerCharacter->AddMovementInput(foward,moveVector.Y);
	OwnerCharacter->AddMovementInput(right,moveVector.X);
}

void UCustomMoveComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
}

void UCustomMoveComponent::OnRun()
{
	SetSpeed(ESpeedType::Run);
}

void UCustomMoveComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}

void UCustomMoveComponent::SetSpeed(ESpeedType SpeedType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)SpeedType];
}

void UCustomMoveComponent::ShouldMove(bool InCanMove)
{
	if (InCanMove == false)
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_None);
	else
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	
}

float UCustomMoveComponent::GetCurrentSpeed() const
{
	return Speed[(int32)CurrentSpeedType];
}
