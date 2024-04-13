// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar3D.generated.h"

/**
 * 
 */
UCLASS()
class NGOCCOLORSHOOT_API UHealthBar3D : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* HealthBar = nullptr;

	virtual bool Initialize() override;

	UFUNCTION()
	void SetHealthProgress(float HealthPercent);
};
