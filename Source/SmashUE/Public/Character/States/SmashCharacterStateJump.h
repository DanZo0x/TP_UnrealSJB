// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SmashCharacterState.h"
#include "SmashCharacterStateJump.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateJump : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;
	
	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;
	
#pragma region Jump Variables

protected:
	void Jump(float inJumpWalkSpeed, float inMaxHeight, float inJumpDuration, float inJumpAirControl) const;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Smash Character State Jump")
	float JumpWalkSpeed = 400.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Smash Character State Jump")
	float MaxHeight = 280.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Smash Character State Jump")
	float JumpDuration = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Smash Character State Jump")
	float JumpAirControl = 1.0f;
#pragma  endregion
};
