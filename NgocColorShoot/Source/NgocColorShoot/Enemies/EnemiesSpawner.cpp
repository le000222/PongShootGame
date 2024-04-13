// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemiesSpawner.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemiesSpawner::AEnemiesSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>("SpawnArea");

	SetRootComponent(SpawnArea);

	SpawnDelayRangeLow = 5.0f;
	SpawnDelayRangeHigh = 20.0f;
	DifficultyScalar = 1.1f;

}

// Called when the game starts or when spawned
void AEnemiesSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnEnemy();
	
}

// Called every frame
void AEnemiesSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemiesSpawner::SpawnEnemy()
{
	// Calculate some new spawn delays so that the game gets harder over time
	SpawnDelayRangeLow /= DifficultyScalar;
	SpawnDelayRangeHigh /= DifficultyScalar;
	// We want this delay to decrease over time, so that the game gets harder
	float SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);

	FVector SpawnLocation = GetRandomPointInVolume();

	UClass* EnemyClass = (FMath::RandRange(0, 1) < 0.5) ? RedEnemy : BlueEnemy;
	AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
	
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemiesSpawner::SpawnEnemy, SpawnDelay, false);
}

FVector AEnemiesSpawner::GetRandomPointInVolume()
{
	//WhereToSpawn is our Box Component
	FVector SpawnOrigin = SpawnArea->Bounds.Origin;
	FVector SpawnExtent = SpawnArea->Bounds.BoxExtent;
	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

