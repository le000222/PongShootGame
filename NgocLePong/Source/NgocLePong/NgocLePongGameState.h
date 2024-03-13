// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NgocLePongGameState.generated.h"

/**
 * 
 */
UCLASS()
class NGOCLEPONG_API ANgocLePongGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ANgocLePongGameState();

	int P1Score = 0;

	int P2Score = 0;

protected:
	virtual void BeginPlay() override;
};
