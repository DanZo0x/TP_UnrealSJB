// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmashCharacterSettings.h"
#include "Camera/CameraFollowTarget.h"
#include "GameFramework/Character.h"
#include "SmashCharacter.generated.h"

class USmashCharacterStateMachine;
class USmashCharacterInputData;
class UEnhancedInputLocalPlayerSubsystem;

UCLASS()
class SMASHUE_API ASmashCharacter : public ACharacter, public ICameraFollowTarget
{
	GENERATED_BODY()

#pragma region Unreal Default
public:
	ASmashCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma endregion

#pragma region Orient

public:
	float GetOrient() const;

	void SetOrientX(float NewOrientX);

protected:
	UPROPERTY(BlueprintReadOnly)
	float OrientX = 1.0f;

	void RotateMeshUsingOrientX() const;
	
#pragma endregion

#pragma region State Machine
	
public:
	void CreateStateMachine();

	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USmashCharacterStateMachine> StateMachine;
	
#pragma endregion

#pragma region Input Data / Mapping Context

public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;

protected:
	void SetupMappingContextIntoController() const;
	
#pragma endregion

#pragma region Input Move X

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveXEvent, float, InputMoveX);

public:
	float GetInputX();

	UPROPERTY()
	FInputMoveXEvent InputMoveXFastEvent;

protected:
	UPROPERTY()
	float InputMoveX = 0.0f;

private:
	void OnInputMoveX(const FInputActionValue& InputActionValue);
	
	void OnInputMoveXFast(const FInputActionValue& InputActionValue);
	
	void BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);

#pragma endregion

#pragma region Jump

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveYEvent, float, InputMoveY);

public:
	float GetInputY() const;
	
	UPROPERTY()
	FInputMoveYEvent InputMoveYPressEvent;

protected:
	UPROPERTY()
	float InputMoveY = 0.0f;

private:
	void OnInputJump(const FInputActionValue& InputActionValue);
	
#pragma endregion

#pragma region Camera Target

public:
	virtual FVector GetFollowPosition() override;
	
	virtual bool IsFollowable() override;

#pragma endregion
};