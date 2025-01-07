// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/SmashCharacterStateFall.h"

#include "Character/SmashCharacter.h"
#include "Character/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateFall::GetStateID()
{
	return ESmashCharacterStateID::Fall;
}

void USmashCharacterStateFall::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Fall(FallHorizontalMoveSpeed, FallAirControl, FallGravityScale, FallFastGravityScale);
}

void USmashCharacterStateFall::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}

void USmashCharacterStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	const USmashCharacterSettings* SmashCharacterSettings = GetDefault<USmashCharacterSettings>();
	if (SmashCharacterSettings == nullptr) return;

	if (FMath::Abs(Character->GetInputX()) > SmashCharacterSettings->InputMoveXThreshold)
	{
		Character->SetOrientX(Character->GetInputX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrient());
	}

	if (Character->GetVelocity().Z >= 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
}

void USmashCharacterStateFall::Fall(float inFallHorizontalMoveSpeed, float inFallAirControl, float inFallGravityScale, float inFallFastGravityScale) const
{
	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	if (!IsValid(MovementComponent)) return;

	MovementComponent->AirControl = inFallAirControl;
	MovementComponent->MaxWalkSpeed = inFallHorizontalMoveSpeed;

	// Prendre en compte FallFastGravityScale si assez de temps
	MovementComponent->GravityScale = inFallGravityScale;
}
