// Fill out your copyright notice in the Description page of Project Settings.


#include "BallSpawner.h"

// Sets default values
ABallSpawner::ABallSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABallSpawner::SpawnBall()
{
	if (!IsValid(BallObject))
	{
		if (bIsBallActive == false)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			BallObject = GetWorld()->SpawnActor<ABall>(BallBlueprint, GetActorLocation(), GetActorRotation(), SpawnParams);
		}

		if (BallObject)
		{
			BallObject->OnDestroyed.AddDynamic(this, &ABallSpawner::OnBallDestroyed);
			bIsBallActive = true;
		}
	}
}

// Called when the game starts or when spawned
void ABallSpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABallSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnBall();
}


void ABallSpawner::OnBallDestroyed(AActor* DestroyedActor)
{
	bIsBallActive = false;
	DestroyedActor->OnDestroyed.RemoveAll(this);
}

