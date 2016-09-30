// Fill out your copyright notice in the Description page of Project Settings.

#include "Fab48Hr16.h"
#include "VR_GrabComponent.h"

// Sets default values for this component's properties
UVR_GrabComponent::UVR_GrabComponent(const FObjectInitializer& ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UVR_GrabComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = this->GetOwner();
	USceneComponent* Root = Owner->GetRootComponent();

	m_PhysicsHandle = NewObject<UPhysicsHandleComponent>(Owner);
	m_PhysicsHandle->RegisterComponent();

	m_GrabPoint = NewObject<USceneComponent>(Owner);
	m_GrabPoint->RegisterComponent();
	m_GrabPoint->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetIncludingScale);
}


// Called every frame
void UVR_GrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsObjectGrabbed() == false)
		return;

	switch (m_DesiredMode)
	{
	case EGrabMode::PHYSICS:
		Tick_Physics(DeltaTime, m_GrabInfo);
		break;
	case EGrabMode::SOCKET:
		Tick_Socket(DeltaTime, m_GrabInfo);
		break;
	}
}

void UVR_GrabComponent::SetSnappingEnabled(bool IsSnapping)
{
	m_IsSnappingEnabled = IsSnapping;

	if (m_IsSnappingEnabled == true)
	{
		m_GrabPoint->SetWorldLocation(this->GetComponentLocation());
		m_GrabPoint->SetWorldRotation(this->GetComponentRotation());
		m_GrabPoint->SetWorldScale3D(this->GetComponentScale());
	}
}

bool UVR_GrabComponent::IsObjectGrabbed() const
{
	return m_GrabInfo.Actor != nullptr
		&& m_GrabInfo.Primitive != nullptr;
}

bool UVR_GrabComponent::GrabBegin(const FGrabInfo& Info)
{
	UE_LOG(LogTemp, Log, TEXT("UVR_GrabComponent::GrabBegin"));

	if (Info.IsValid() == false)
		return false;

	if (IsObjectGrabbed() == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to Grab an object while an object is already Grabped. UnGrabping the old object and Grabping the new object."));
		FGrabInfo OutInfo; // doesn't do anything
		GrabFinish(OutInfo);
	}

	m_ObjectMode = CalculateModeForPrimitive(Info.Primitive);
	if (m_DesiredMode == EGrabMode::NONE)
		m_DesiredMode = m_ObjectMode;

	m_GrabInfo = FGrabInfo(Info);

	if (m_IsSnappingEnabled == false)
	{
		m_GrabPoint->SetWorldLocation(Info.Primitive->GetComponentLocation());
		m_GrabPoint->SetWorldRotation(Info.Primitive->GetComponentRotation());
		m_GrabPoint->SetWorldScale3D(Info.Primitive->GetComponentScale());
	}

	switch (m_DesiredMode)
	{
	case EGrabMode::PHYSICS:
		Begin_Physics(m_GrabInfo);
		break;
	case EGrabMode::SOCKET:
		Begin_Socket(m_GrabInfo);
		break;
	}

	return true;
}

bool UVR_GrabComponent::GrabFinish(FGrabInfo& OutInfo)
{
	UE_LOG(LogTemp, Log, TEXT("UVR_GrabComponent::GrabFinish"));

	if (IsObjectGrabbed() == false)
		return false;

	if (m_GrabInfo.IsValid() == false)
		return false;

	switch (m_DesiredMode)
	{
	case EGrabMode::PHYSICS:
		Finish_Physics(m_GrabInfo);
		break;
	case EGrabMode::SOCKET:
		Finish_Socket(m_GrabInfo);
		break;
	}

	OutInfo = FGrabInfo(m_GrabInfo);
	m_GrabInfo.Clear();

	return true;
}

void UVR_GrabComponent::Begin_Physics(FGrabInfo& Info)
{
	UE_LOG(LogTemp, Log, TEXT("UVR_GrabComponent::Begin_Physics"));

	if (Info.IsValid() == false)
		return;

	switch (m_ObjectMode)
	{
	case EGrabMode::PHYSICS:
		break;
	case EGrabMode::SOCKET:
		Info.Primitive->SetSimulatePhysics(true);
		break;
	}

	m_PhysicsHandle->GrabComponent(Info.Primitive, NAME_None, Info.Primitive->GetComponentLocation(), true);
}

void UVR_GrabComponent::Tick_Physics(float DeltaSeconds, const FGrabInfo& Info)
{
	if (Info.IsValid() == false)
		return;

	switch (m_ObjectMode)
	{
	case EGrabMode::PHYSICS:
		break;
	case EGrabMode::SOCKET:
		break;
	}

	m_PhysicsHandle->SetTargetLocation(m_GrabPoint->GetComponentLocation());
	m_PhysicsHandle->SetTargetRotation(m_GrabPoint->GetComponentRotation());
}

void UVR_GrabComponent::Finish_Physics(const FGrabInfo& Info)
{
	UE_LOG(LogTemp, Log, TEXT("UVR_GrabComponent::Finish_Physics"));

	if (Info.IsValid() == false)
		return;

	m_PhysicsHandle->ReleaseComponent();

	switch (m_ObjectMode)
	{
	case EGrabMode::PHYSICS:
		break;
	case EGrabMode::SOCKET:
		Info.Primitive->SetSimulatePhysics(false);
		break;
	}
}

void UVR_GrabComponent::Begin_Socket(const FGrabInfo& Info)
{
	UE_LOG(LogTemp, Log, TEXT("UVR_GrabComponent::Begin_Socket"));

	if (Info.IsValid() == false)
		return;

	switch (m_ObjectMode)
	{
	case EGrabMode::PHYSICS:
		Info.Primitive->SetSimulatePhysics(false);
		Info.Primitive->AttachToComponent(Info.Actor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		Info.Primitive->SetWorldLocation(Info.Actor->GetActorLocation());
		Info.Primitive->SetWorldRotation(Info.Actor->GetActorRotation());
		break;
	case EGrabMode::SOCKET:
		break;
	}

	Info.Actor->AttachToComponent(m_GrabPoint, FAttachmentTransformRules::KeepWorldTransform, NAME_None);
	Info.Actor->SetActorLocation(m_GrabPoint->GetComponentLocation());
	Info.Actor->SetActorRotation(m_GrabPoint->GetComponentRotation());
}

void UVR_GrabComponent::Tick_Socket(float DeltaSeconds, const FGrabInfo& Info)
{
	if (Info.IsValid() == false)
		return;

	switch (m_ObjectMode)
	{
	case EGrabMode::PHYSICS:
	case EGrabMode::SOCKET:
		break;
	}
}

void UVR_GrabComponent::Finish_Socket(const FGrabInfo& Info)
{
	UE_LOG(LogTemp, Log, TEXT("UVR_GrabComponent::Finish_Socket"));

	if (Info.IsValid() == false)
		return;

	switch (m_ObjectMode)
	{
	case EGrabMode::PHYSICS:
		Info.Primitive->SetSimulatePhysics(true);
		break;
	case EGrabMode::SOCKET:
		break;
	}

	Info.Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

const EGrabMode UVR_GrabComponent::CalculateModeForPrimitive(UPrimitiveComponent* Primitive) const
{
	if (Primitive == nullptr)
		return EGrabMode::NONE;
	else if (Primitive->IsSimulatingPhysics() == true)
		return EGrabMode::PHYSICS;
	else
		return EGrabMode::SOCKET;
}
