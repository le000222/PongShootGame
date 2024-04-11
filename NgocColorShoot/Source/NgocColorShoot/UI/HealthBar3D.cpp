// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthBar3D.h"
#include "Components/ProgressBar.h"

bool UHealthBar3D::Initialize()
{
	bool Success = Super::Initialize();

	if (HealthBar)
	{
		HealthBar->PercentDelegate.BindUFunction(this, "SetHealthProgress");
	}
	return true;
}

float UHealthBar3D::SetHealthProgress()
{
	return 1;
}

