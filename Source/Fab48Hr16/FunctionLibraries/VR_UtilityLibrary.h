// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "VR_UtilityLibrary.generated.h"

/**
*
*/
UCLASS()
class FAB48HR16_API UVR_UtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Utility")
		static AActor* GetClosestActor(const FVector& Location, const TArray<AActor*>& Actors);
	UFUNCTION(BlueprintCallable, Category = "Utility")
		static UPrimitiveComponent* GetClosestPrimitive(const FVector& Location, const TArray<UPrimitiveComponent*>& Primitives);
};
