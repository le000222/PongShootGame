// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainPlayer.generated.h"

UCLASS()
class NGOCCOLORSHOOT_API AMainPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainPlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ColorShoot")
	class UBoxComponent* ColorShoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ColorShoot")
	UStaticMeshComponent* VisualMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* MainCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly)
	float MaxSpeed;

	UFUNCTION(BlueprintPure)
	class AWeapon* GetCurrentWeapon();

	UPROPERTY(EditDefaultsOnly)
	float MaximumHealth = 40.0f;

	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth;

	//UPROPERTY(EditAnywhere, Category = "HUD");
	//TSubclassOf<class UPongHUD> WBP_PongHUD;

	//class UPongHUD* MyHud;

	// Function to fire the weapon.
	//void Fire(bool Toggle);
	// Function to toggle aiming mode.
	//void Aim(bool Toggle);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class UFloatingPawnMovement* FloatingPawnMovement;

	float CurrentVelocity = 0;

	UFUNCTION()
	void OnHitActor(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	float MoveVerticalAmount = 0;
	float MoveHorizontalAmount = 0;

	void HoldWeapon(class AWeapon* Weapon);
	// Function to drop the currently held weapon.
	void DropWeapon();

	void Turn(float Amount);
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void Pause();

	UPROPERTY()
	class AWeapon* CurrentWeapon;

	// Boolean flag to indicate aiming state.
	bool bIsAiming;
	// Boolean flag to indicate firing state.
	bool bIsFiring;

	// Boolean flag to indicate if the character has a weapon.
	bool bHasWeapon;

	bool bPaused;
};
