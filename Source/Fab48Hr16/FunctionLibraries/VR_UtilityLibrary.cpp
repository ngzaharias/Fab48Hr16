// Fill out your copyright notice in the Description page of Project Settings.

#include "Fab48Hr16.h"
#include "VR_UtilityLibrary.h"

AActor* UVR_UtilityLibrary::GetClosestActor(const FVector& Location, const TArray<AActor*>& Actors)
{
	if (Actors.Num() <= 0)
	{
		return nullptr;
	}

	AActor* Target = Actors[0];
	float Distance = FVector::DistSquared(Location, Target->GetActorLocation());
	for (int32 i = 1; i < Actors.Num(); ++i)
	{
		AActor* TargetNext = Actors[i];
		float DistanceNext = FVector::DistSquared(Location, TargetNext->GetActorLocation());
		if (DistanceNext < Distance)
		{
			Target = TargetNext;
			Distance = DistanceNext;
		}
	}

	return Target;
}

UPrimitiveComponent* UVR_UtilityLibrary::GetClosestPrimitive(const FVector& Location, const TArray<UPrimitiveComponent*>& Primitives)
{
	if (Primitives.Num() <= 0)
	{
		return nullptr;
	}

	UPrimitiveComponent* Target = Primitives[0];
	float Distance = FVector::DistSquared(Location, Target->GetComponentLocation());
	for (int32 i = 1; i < Primitives.Num(); ++i)
	{
		UPrimitiveComponent* TargetNext = Primitives[i];
		float DistanceNext = FVector::DistSquared(Location, TargetNext->GetComponentLocation());
		if (DistanceNext < Distance)
		{
			Target = TargetNext;
			Distance = DistanceNext;
		}
	}

	return Target;
}
