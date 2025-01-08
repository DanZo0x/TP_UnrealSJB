// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"

#include "Camera/CameraFollowTarget.h"
#include "Character/SmashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	CameraMain = FindCameraByTag(TEXT("CameraMain"));

	AActor* CameraBoundsActor = FindCameraBoundsActor();
	if (!IsValid(CameraBoundsActor)) return;

	InitCameraBounds(CameraBoundsActor);
	InitCameraZoomParameters();
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraZoom(DeltaTime);
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
	
	ClampPositionIntoCameraBounds(TargetCameraPosition);
	
	if (IsValid(CameraMain))
	{
		FVector CurrentCameraPosition = CameraMain->GetComponentLocation();
		FVector NewCameraPosition = FMath::Lerp(CurrentCameraPosition, TargetCameraPosition, DeltaTime * CameraLerpSpeed);
		
		CameraMain->SetWorldLocation(NewCameraPosition);
	}
}

void UCameraWorldSubsystem::TickUpdateCameraZoom(float DeltaTime)
{
	if (!IsValid(CameraMain)) return;

	float GreatestDistanceBetweenTargets = CalculateGreatestDistanceBetweenTargets();
	float Percent = FMath::Clamp(
		UKismetMathLibrary::NormalizeToRange(
			GreatestDistanceBetweenTargets, 
			CameraZoomDistanceBetweenTargetsMin, 
			CameraZoomDistanceBetweenTargetsMax
		), 
		0.0f, 
		1.0f
	);

	FVector StartPosition = CameraMain->GetAttachParentActor()->GetActorLocation();
	FVector CurrentPosition = CameraMain->GetAttachParentActor()->GetActorLocation();

	CurrentPosition.Y = FMath::Lerp(CameraZoomYMin, CameraZoomYMax, Percent);

	CameraMain->GetAttachParentActor()->SetActorLocation(StartPosition + (CurrentPosition - StartPosition) * DeltaTime);
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

float UCameraWorldSubsystem::CalculateGreatestDistanceBetweenTargets()
{
	TArray<FVector> CheckedTargetPositions;
	
	for (UObject* Target : FollowTargets)
	{
		if (Target->Implements<UCameraFollowTarget>())
		{
			ICameraFollowTarget* FollowTargetInterface = Cast<ICameraFollowTarget>(Target);

			if (FollowTargetInterface->IsFollowable())
			{
				CheckedTargetPositions.Add(FollowTargetInterface->GetFollowPosition());
			}
		}
	}

	float GreatestDistance = 0.0f;

	for (int i = 0; i < CheckedTargetPositions.Num(); i++)
	{
		for (int j = i + 1; j < CheckedTargetPositions.Num(); j++)
		{
			float DistanceBetweenTargets = FVector::Dist(CheckedTargetPositions[i], CheckedTargetPositions[j]);

			GreatestDistance = FMath::Max(GreatestDistance, DistanceBetweenTargets);
		}
	}

	return GreatestDistance;
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(this, Tag, FoundActors);

	/*return FoundActors[0]->GetComponentByClass<UCameraComponent>();*/
	
	for (AActor* Actor : FoundActors)
	{
		if (IsValid(Actor))
		{
			UCameraComponent* CameraComponent = Actor->GetComponentByClass<UCameraComponent>();
			if (CameraComponent != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Hello World"));
				
				return CameraComponent;
			}
		}
	}

	return nullptr;
}

AActor* UCameraWorldSubsystem::FindCameraBoundsActor()
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
	if (!IsValid(World)) return nullptr;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(World, "CameraBounds", FoundActors);
	
	for (AActor* Actor : FoundActors)
	{
		if (Actor != nullptr)
		{
			return Actor;
		}
	}

	return nullptr;
}

void UCameraWorldSubsystem::InitCameraBounds(AActor* CameraBoundsActor)
{
	FVector BoundsCenter;
	FVector BoundsExtents;
	
	CameraBoundsActor->GetActorBounds(false, BoundsCenter, BoundsExtents);

	CameraBoundsMin = FVector2D(BoundsCenter.X - BoundsExtents.X, BoundsCenter.Z - BoundsExtents.Z);
	CameraBoundsMax = FVector2D(BoundsCenter.X + BoundsExtents.X, BoundsCenter.Z + BoundsExtents.Z);

	CameraBoundsYProjectionCenter = BoundsCenter.Y;
}

void UCameraWorldSubsystem::ClampPositionIntoCameraBounds(FVector& Position)
{
	FVector2D ViewportBoundsMin, ViewportBoundsMax;
	GetViewportBounds(ViewportBoundsMin, ViewportBoundsMax);

	FVector WorldBoundsMin = CalculateWorldPositionFromViewportPosition(ViewportBoundsMin);
	FVector WorldBoundsMax = CalculateWorldPositionFromViewportPosition(ViewportBoundsMax);

	float MinX = CameraBoundsMin.X + WorldBoundsMin.X;
	float MaxX = CameraBoundsMax.X - WorldBoundsMax.X;

	float MinY = CameraBoundsMin.Y + WorldBoundsMin.Y;
	float MaxY = CameraBoundsMax.Y - WorldBoundsMax.Y;

	Position.X = FMath::Clamp(Position.X, MinX, MaxX);
	Position.Z = FMath::Clamp(Position.Z, MinY, MaxY);
}

void UCameraWorldSubsystem::GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax)
{
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient == nullptr) return;

	FViewport* Viewport = ViewportClient->Viewport;
	if (Viewport == nullptr) return;

	FIntRect ViewRect(
		Viewport->GetInitialPositionXY(),
		Viewport->GetInitialPositionXY() + Viewport->GetSizeXY()
	);
	FIntRect ViewportRect = Viewport->CalculateViewExtents(CameraMain->AspectRatio, ViewRect);

	OutViewportBoundsMin.X = ViewportRect.Min.X;
	OutViewportBoundsMin.Y = ViewportRect.Min.Y;

	OutViewportBoundsMax.X = ViewportRect.Max.X;
	OutViewportBoundsMax.Y = ViewportRect.Max.Y;
}

FVector UCameraWorldSubsystem::CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition)
{
	if (CameraMain == nullptr) return FVector::Zero();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController == nullptr) return FVector::Zero();

	float YDistanceToCenter = CameraMain->GetOwner()->GetActorLocation().Y - CameraBoundsYProjectionCenter;

	FVector CameraWorldProjectDir;
	FVector WorldPosition;
	UGameplayStatics::DeprojectScreenToWorld(
		PlayerController,
		ViewportPosition,
		WorldPosition,
		CameraWorldProjectDir
	);

	WorldPosition += CameraWorldProjectDir * YDistanceToCenter;

	return WorldPosition;
}

void UCameraWorldSubsystem::InitCameraZoomParameters()
{
	UCameraComponent* CameraDistanceMin = FindCameraByTag("CameraDistanceMin");
	UCameraComponent* CameraDistanceMax = FindCameraByTag("CameraDistanceMax");

	IsValid(CameraDistanceMin) ? CameraZoomYMin = CameraDistanceMin->GetComponentLocation().Y : CameraZoomYMin = 0.0f;
	IsValid(CameraDistanceMax) ? CameraZoomYMax = CameraDistanceMax->GetComponentLocation().Y : CameraZoomYMax = 0.0f;
}
