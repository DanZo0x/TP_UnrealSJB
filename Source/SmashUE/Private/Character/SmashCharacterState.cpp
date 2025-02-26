// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SmashCharacterState.h"

#include "Character/SmashCharacter.h"
#include "Character/SmashCharacterStateMachine.h"


// Sets default values for this component's properties
USmashCharacterState::USmashCharacterState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

ESmashCharacterStateID USmashCharacterState::GetStateID()
{
	return ESmashCharacterStateID::None;
}

void USmashCharacterState::StateInit(USmashCharacterStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;
	Character = InStateMachine->GetCharacter();
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Magenta,
		FString::Printf(TEXT("Init State: %d"), GetStateID())
	);
}

void USmashCharacterState::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Character->PlayAnimMontage(AnimMontage);
}

void USmashCharacterState::StateExit(ESmashCharacterStateID NextStateID)
{
	
}

void USmashCharacterState::StateTick(float DeltaTime)
{
	
}
