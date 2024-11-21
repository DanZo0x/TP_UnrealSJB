// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SmashCharacterState.h"
#include "SmashCharacterStateFall.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateFall : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;
	
	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

#pragma region Animation

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Smash Character State Jump")
	UAnimMontage* FallAnim;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Smash Character State Jump")
	float FallHorizontalMoveSpeed = 400.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Smash Character State Jump")
	float FallAirControl = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Smash Character State Jump")
	float FallGravityScale = 3.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Smash Character State Jump")
	float FallFastGravityScale = 10.0f;
#pragma  endregion
};
