// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PongHUD.generated.h"

/**
 * 
 */
UCLASS()
class NGOCLEPONG_API UPongHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Player1Score = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Player2Score = nullptr;

	virtual bool Initialize() override;

private:
	UFUNCTION()
	FText UpdateTextScore();

};
