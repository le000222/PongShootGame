// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NgocLePongGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class NGOCLEPONG_API ANgocLePongGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	AActor* BallSpawner;

public:
	UPROPERTY(EditAnywhere)
	float SpawnChance = 0.05f;//0.1;

	void OnSpawnBall(class AMainPawn* Player, AActor* Ring);

};
