// Fill out your copyright notice in the Description page of Project Settings.


#include "PongHUD.h"
#include "./Ball/Ball.h"
#include "Components/TextBlock.h"

FText UPongHUD::UpdateTextScore()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		if (Player1Score)
		{
			//return FText::FromString(FString::FromInt(AmmoCount) + "/" + FString::FromInt(MaxAmmo));
			//return FText::AsNumber(AmmoCount);
		}
	}
	return FText::FromString("NULL");
}

bool UPongHUD::Initialize()
{
	bool Success = Super::Initialize();

	if (Player1Score && Player2Score)
	{
		Player1Score->TextDelegate.BindUFunction(this, "UpdateScoreText");
		Player2Score->TextDelegate.BindUFunction(this, "UpdateScoreText");
	}

	return true;
}
