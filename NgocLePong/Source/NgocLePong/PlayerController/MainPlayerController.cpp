// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"

// Sets default values
AMainPlayerController::AMainPlayerController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainPlayerController::Tick(float const DeltaTime)
{
	Super::Tick(DeltaTime);

}

