// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemiesSpawner.generated.h"

UCLASS()
class NGOCCOLORSHOOT_API AEnemiesSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemiesSpawner();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	class UBoxComponent* SpawnArea;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	float SpawnDelayRangeLow;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	float SpawnDelayRangeHigh;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	float DifficultyScalar;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	TSubclassOf<class AEnemy> RedEnemy;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	TSubclassOf<class AEnemy> BlueEnemy;

	void SpawnEnemy();

	FVector GetRandomPointInVolume();

	FTimerHandle SpawnTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
