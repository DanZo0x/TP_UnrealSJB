// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SmashCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "Inputs/SmashCharacterInputData.h"
#include "MatchGameMode.generated.h"

class AArenaPlayerStart;
class ASmashCharacter;
class USmashCharacterSettings;

/**
 * 
 */
UCLASS()
class SMASHUE_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void BeginPlay() override;

protected:
	UPROPERTY()
	TArray<ASmashCharacter*> CharactersInsideArena;

private:
	USmashCharacterInputData* LoadInputDataFromConfig();

	UInputMappingContext* LoadInputMappingContextFromConfig();
	
	void FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& ResultsActors);

	void SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints);

	TSubclassOf<ASmashCharacter> GetSmashCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const;
};
