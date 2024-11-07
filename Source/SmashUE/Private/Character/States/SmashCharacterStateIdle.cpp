// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/Character/States/SmashCharacterStateIdle.h"

ESmashCharacterStateID USmashCharacterStateIdle::GetStateID()
{
	return ESmashCharacterStateID::Idle;
}

void USmashCharacterStateIdle::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Cyan,
		TEXT("Enter StateIdle")
	);
}

void USmashCharacterStateIdle::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Red,
		TEXT("Exit StateIdle")
	);
}