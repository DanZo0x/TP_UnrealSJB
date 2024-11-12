// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/SmashCharacterStateRun.h"

#include "Character/SmashCharacter.h"

ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(RunAnim);
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Cyan,
		TEXT("Enter StateWalk")
	);
}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Red,
		TEXT("Exit StateWalk")
	);
}

void USmashCharacterStateRun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character->GetVelocity().X < RunSpeed)
	{
		Character->AddMovementInput(FVector(RunSpeed, 0, 0), 1);
	}
}
