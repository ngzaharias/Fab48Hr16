// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "VR_GrabComponent.generated.h"

UENUM(BlueprintType)
enum class EGrabMode : uint8
{
	NONE		UMETA(DisplayName = "None"),
	PHYSICS		UMETA(DisplayName = "Physics"),
	SOCKET		UMETA(DisplayName = "Socket"),
};

USTRUCT(BlueprintType)
struct FGrabInfo
{
	GENERATED_BODY()

		bool IsValid() const { return Actor != nullptr && Primitive != nullptr; }

	void Clear() { Actor = nullptr; Primitive = nullptr; }

	UPROPERTY(BlueprintReadWrite)
		AActor* Actor = nullptr;
	UPROPERTY(BlueprintReadWrite)
		UPrimitiveComponent* Primitive = nullptr;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FAB48HR16_API UVR_GrabComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UVR_GrabComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Grab")
		void SetSnappingEnabled(bool IsSnapping);

	UFUNCTION(BlueprintCallable, Category = "Grab")
		bool IsObjectGrabbed() const;

	UFUNCTION(BlueprintCallable, Category = "Grab")
		bool GrabBegin(const FGrabInfo& Info);
	UFUNCTION(BlueprintCallable, Category = "Grab")
		bool GrabFinish(FGrabInfo& OutInfo);

protected:
	void Begin_Physics(FGrabInfo& Info);
	void Tick_Physics(float DeltaSeconds, const FGrabInfo& Info);
	void Finish_Physics(const FGrabInfo& Info);

	void Begin_Socket(const FGrabInfo& Info);
	void Tick_Socket(float DeltaSeconds, const FGrabInfo& Info);
	void Finish_Socket(const FGrabInfo& Info);

	const EGrabMode CalculateModeForPrimitive(UPrimitiveComponent* Primitive) const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Grab")
		UPhysicsHandleComponent* m_PhysicsHandle = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Grab")
		USceneComponent* m_GrabPoint;

	UPROPERTY(BlueprintReadOnly, Category = "Grab")
		FGrabInfo m_GrabInfo;

	UPROPERTY(BlueprintReadOnly, Category = "Grab")
		EGrabMode m_ObjectMode = EGrabMode::NONE;
	UPROPERTY(EditAnywhere, Category = "Grab")
		EGrabMode m_DesiredMode = EGrabMode::NONE;

	UPROPERTY(BlueprintReadOnly, Category = "Grab")
		bool m_IsSnappingEnabled = false;
};
