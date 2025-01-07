// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SmashCharacterState.h"
#include "SmashCharacterStateRun.generated.h"

class ASmashCharacter;
class USmashCharacterStateMachine;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateRun : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;

	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

	UFUNCTION()
	void OnInputJump(float InputMoveY);

#pragma region Run Variables

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Smash Character State Run")
	float RunSpeed = 250.0f;
#pragma  endregion
};
