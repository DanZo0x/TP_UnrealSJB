// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"

#include "Camera/CameraFollowTarget.h"
#include "Character/SmashCharacter.h"
#include "Kismet/GameplayStatics.h"

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(TEXT("CameraMain"));
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
}

void UCameraWorldSubsystem::AddFollowTarget(UObject* FollowTarget)
{
	FollowTargets.Add(FollowTarget);
}

void UCameraWorldSubsystem::RemoveFollowTarget(UObject* FollowTarget)
{
	if (FollowTargets.Contains(FollowTarget))
	{
		FollowTargets.Remove(FollowTarget);
	}
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	FVector TargetCameraPosition = CalculateAveragePositionBetweenTargets();
	if (TargetCameraPosition.IsZero()) return;
	
	if (IsValid(CameraMain))
	{
		FVector CurrentCameraPosition = CameraMain->GetComponentLocation();
		FVector NewCameraPosition = FMath::Lerp(CurrentCameraPosition, TargetCameraPosition, DeltaTime * CameraLerpSpeed);
		
		CameraMain->SetWorldLocation(NewCameraPosition);
	}
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	if (FollowTargets.Num() == 0) return FVector::ZeroVector;

	FVector CurrentCameraPositionY = FVector(0, CameraMain->GetAttachParentActor()->GetActorLocation().Y, 0);
	FVector AveragePosition = FVector::ZeroVector;
	int CheckedTargets = 0;
	
	for (UObject* Target : FollowTargets)
	{
		if (IsValid(Target) && FollowTargets[CheckedTargets]->GetClass()->ImplementsInterface(UCameraFollowTarget::StaticClass()) && Cast<ASmashCharacter>(FollowTargets[CheckedTargets])->IsFollowable())
		{
			AveragePosition += Cast<ASmashCharacter>(FollowTargets[CheckedTargets])->GetFollowPosition();
			CheckedTargets++;
		}
	}

	if (CheckedTargets == 0) return FVector::ZeroVector;

	AveragePosition = FVector(AveragePosition.X / CheckedTargets, 0, AveragePosition.Z / CheckedTargets) + CurrentCameraPositionY;

	return AveragePosition;
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
	if (!IsValid(World)) return nullptr;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(World, Tag, FoundActors);
	for (AActor* Actor : FoundActors)
	{
		if (IsValid(Actor))
		{
			UCameraComponent* CameraComponent = Actor->FindComponentByClass<UCameraComponent>();
			if (IsValid(CameraComponent))
			{
				return CameraComponent;
			}
		}
	}

	return nullptr;
}
