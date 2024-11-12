// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/SmashCharacterStateWalk.h"

#include "Character/SmashCharacter.h"

ESmashCharacterStateID USmashCharacterStateWalk::GetStateID()
{
	return ESmashCharacterStateID::Walk;
}

void USmashCharacterStateWalk::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(WalkAnim);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Cyan,
		TEXT("Enter StateWalk")
	);
}

void USmashCharacterStateWalk::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Red,
		TEXT("Exit StateWalk")
	);
}

void USmashCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	if (Character->GetVelocity().X < WalkSpeed)
	{
		Character->AddMovementInput(FVector(WalkSpeed, 0, 0), 1);
	}
}
