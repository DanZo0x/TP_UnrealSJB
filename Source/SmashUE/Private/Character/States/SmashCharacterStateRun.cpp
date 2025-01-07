// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/SmashCharacterStateRun.h"

#include "Character/SmashCharacter.h"
#include "Character/SmashCharacterStateMachine.h"

ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->InputMoveYPressEvent.AddDynamic(this, &USmashCharacterStateRun::OnInputJump);
}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	Character->InputMoveYPressEvent.RemoveDynamic(this, &USmashCharacterStateRun::OnInputJump);
}

void USmashCharacterStateRun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	const USmashCharacterSettings* SmashCharacterSettings = GetDefault<USmashCharacterSettings>();
	if (SmashCharacterSettings == nullptr) return;

	if (FMath::Abs(Character->GetInputX()) < SmashCharacterSettings->InputMoveXThreshold)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	else
	{
		Character->SetOrientX(Character->GetInputX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrient());
	}
}

void USmashCharacterStateRun::OnInputJump(float InputMoveY)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}
