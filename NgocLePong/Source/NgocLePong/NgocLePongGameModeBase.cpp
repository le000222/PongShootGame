// Copyright Epic Games, Inc. All Rights Reserved.

#include "NgocLePongGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "./BallSpawner.h"

void ANgocLePongGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	// Find all item spawner nodes
	BallSpawner = UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass());
}

void ANgocLePongGameModeBase::OnSpawnBall(AMainPawn* Player, AActor* Ring)
{
	if (ABallSpawner* MySpawner = Cast<ABallSpawner>(BallSpawner))
	{
		MySpawner->SpawnBall();
	}
}
