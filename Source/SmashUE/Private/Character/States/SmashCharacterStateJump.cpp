// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/SmashCharacterStateJump.h"

#include "Character/SmashCharacter.h"
#include "Character/SmashCharacterStateMachine.h"
#include "Character/SmashCharacterSettings.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	
	Jump(JumpWalkSpeed, MaxHeight, JumpDuration, JumpAirControl);
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	const USmashCharacterSettings* SmashCharacterSettings = GetDefault<USmashCharacterSettings>();
	if (SmashCharacterSettings == nullptr) return;

	if (FMath::Abs(Character->GetInputX()) > SmashCharacterSettings->InputMoveXThreshold)
	{
		Character->SetOrientX(Character->GetInputX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrient());
	}

	if (Character->GetVelocity().Z < 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}

void USmashCharacterStateJump::Jump(float inJumpWalkSpeed, float inMaxHeight, float inJumpDuration, float inJumpAirControl) const
{
	const float JumpVelocity = (2.0f * inMaxHeight) / inJumpDuration;

	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	if (!IsValid(MovementComponent)) return;

	MovementComponent->MaxWalkSpeed = inJumpWalkSpeed;
	MovementComponent->AirControl = inJumpAirControl;
	
	Character->LaunchCharacter(FVector(0.f, 0.f, JumpVelocity), false, true);
}
