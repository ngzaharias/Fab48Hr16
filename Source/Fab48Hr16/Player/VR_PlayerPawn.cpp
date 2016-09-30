// Fill out your copyright notice in the Description page of Project Settings.

#include "Fab48Hr16.h"
#include "VR_PlayerPawn.h"


// Sets default values
AVR_PlayerPawn::AVR_PlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVR_PlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVR_PlayerPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AVR_PlayerPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

