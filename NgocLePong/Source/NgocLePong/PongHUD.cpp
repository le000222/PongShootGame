// Fill out your copyright notice in the Description page of Project Settings.


#include "PongHUD.h"
#include "./Ball/Ball.h"
#include "./NgocLePongGameState.h"
#include "Components/TextBlock.h"

FText UPongHUD::UpdateP1Score()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		if (P1ScoreText)
		{

			ANgocLePongGameState* GS = 	GetWorld()->GetGameState<ANgocLePongGameState>();
			return FText::FromString("Player Score: " + FString::FromInt(GS->P1Score));

		}
	}
	return FText::FromString("NULL");
}

FText UPongHUD::UpdateP2Score()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		if (P2ScoreText)
		{

			ANgocLePongGameState* GS = 	GetWorld()->GetGameState<ANgocLePongGameState>();
			return FText::FromString("AI Score: " + FString::FromInt(GS->P2Score));

		}
	}
	return FText::FromString("NULL");
}

bool UPongHUD::Initialize()
{
	bool Success = Super::Initialize();

	if (P1ScoreText)
	{
		P1ScoreText->TextDelegate.BindUFunction(this, "UpdateP1Score");
	}

	if (P2ScoreText)
	{
		P2ScoreText->TextDelegate.BindUFunction(this, "UpdateP2Score");
	}

	return true;
}
