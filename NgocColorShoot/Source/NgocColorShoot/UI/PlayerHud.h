// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHud.generated.h"

/**
 * 
 */
UCLASS()
class NGOCCOLORSHOOT_API UPlayerHud : public UUserWidget
{
	GENERATED_BODY()

	//UI HUD
public:
	void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* WeaponAmmoCountText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* ClickMeButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* HealthBar = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* YouLost = nullptr;

	virtual bool Initialize() override;

	UFUNCTION()
	FText SetWeaponAmmoCount();

	UFUNCTION()
	float SetHealthProgress();

	UFUNCTION()
	void EndGame();

private:
	UFUNCTION()
	void ButtonClicked();

};
