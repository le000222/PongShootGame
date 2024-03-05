// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerPaddle.h"
#include "Ball.h"
#include "AIPaddle.generated.h"

/**
 * 
 */
UCLASS()
class NGOCLEPONG_API AAIPaddle : public APlayerPaddle
{
	GENERATED_BODY()
	

public:
	AAIPaddle();

	class ABall* Ball;

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PrintMessageOnScreen(FString Message) override;
};
