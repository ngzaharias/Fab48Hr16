// Fill out your copyright notice in the Description page of Project Settings.

#include "Fab48Hr16.h"
#include "VR_PlayerHand.h"


// Sets default values
AVR_PlayerHand::AVR_PlayerHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVR_PlayerHand::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVR_PlayerHand::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

