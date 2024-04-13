// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthBar3D.h"
#include "../Player/MainCharacter.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

bool UHealthBar3D::Initialize()
{
	bool Success = Super::Initialize();

	if (HealthBar)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainCharacter::StaticClass()));
		HealthBar->SetPercent(100);
		if (MainCharacter)
		{
			HealthBar->PercentDelegate.BindUFunction(this, "SetHealthProgress");
		}
	}
	return true;
}

void UHealthBar3D::SetHealthProgress(float HealthPercent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(HealthPercent);
	}
}

