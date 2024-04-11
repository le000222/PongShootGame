// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/PlayerHud.h"
#include "../Player/MainCharacter.h"
#include "../Weapon/WeaponBase.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Styling/SlateColor.h"

void UPlayerHud::NativeConstruct()
{
	if (ClickMeButton)
	{
		ClickMeButton->OnClicked.AddDynamic(this, &UPlayerHud::ButtonClicked);
	}
}

bool UPlayerHud::Initialize()
{
	bool Success = Super::Initialize();
	if (WeaponAmmoCountText)
	{
		WeaponAmmoCountText->TextDelegate.BindUFunction(this, "SetWeaponAmmoCount");
	}

	if (HealthBar)
	{
		HealthBar->PercentDelegate.BindUFunction(this, "SetHealthProgress");
	}
	return true;

}

FText UPlayerHud::SetWeaponAmmoCount()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(PC->GetPawn());
		if (PlayerCharacter)
		{
			AWeaponBase* BlueWeapon = PlayerCharacter->CurrentBlueWeapon;
			AWeaponBase* RedWeapon = PlayerCharacter->CurrentRedWeapon;
			//WeaponAmmoCountText->SetText(FText::AsNumber(Weapon->CurrentAmmo) );
			//return FText::FromString(FString::FromInt(Weapon->CurrentAmmo));
			int BlueAmmoCount = 0;
			int RedAmmoCount = 0;
			int MaxAmmo = 0;
			
			if (BlueWeapon)
			{
				BlueAmmoCount = BlueWeapon->CurrentAmmo;
				MaxAmmo = BlueWeapon->MaximumAmmo;
			}
			if (RedWeapon)
			{
				RedAmmoCount = RedWeapon->CurrentAmmo;
				MaxAmmo = RedWeapon->MaximumAmmo;
			}

			return FText::FromString("Blue Ammo: " + FString::FromInt(BlueAmmoCount) + "/" + FString::FromInt(MaxAmmo) 
				+ "\nRed Ammo: " + FString::FromInt(RedAmmoCount) + "/" + FString::FromInt(MaxAmmo));
		}
	}
	return FText::FromString("NULL");
}

float UPlayerHud::SetHealthProgress()
{

	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		AMainCharacter* PlayerCharacter = Cast<AMainCharacter>(PC->GetPawn());
		if (PlayerCharacter)
		{
			return PlayerCharacter->CurrentHealth / PlayerCharacter->MaximumHealth;
		}

	}
	return 1;
}

void UPlayerHud::ButtonClicked()
{
	//WeaponAmmoCountText->SetColorAndOpacity(FSlateColor(FLinearColor::Blue));

	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->ConsoleCommand("RestartLevel");
	}

}