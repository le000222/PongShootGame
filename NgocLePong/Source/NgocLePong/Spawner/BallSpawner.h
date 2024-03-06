// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "./Ball/Ball.h"
#include "BallSpawner.generated.h"

UCLASS()
class NGOCLEPONG_API ABallSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABallSpawner();

	UPROPERTY(EditAnywhere, Category = "Ball To Spawn")
	TSubclassOf<class ABall> BallBlueprint;

	UPROPERTY(Transient)
	class ABall* BallObject;

	void SpawnBall();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBallDestroyed(AActor* DestroyedActor);

};
