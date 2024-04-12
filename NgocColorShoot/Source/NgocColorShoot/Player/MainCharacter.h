// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class NGOCCOLORSHOOT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character")
	class USphereComponent* InteractSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* MainCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	class UArrowComponent* BlueWeaponMount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	class UArrowComponent* RedWeaponMount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character")
	class UWidgetComponent* HealthBar;

	UPROPERTY()
	class AWeaponBase* CurrentBlueWeapon;
	
	UPROPERTY()
	class AWeaponBase* CurrentRedWeapon;

	UPROPERTY(EditDefaultsOnly)
	float MaxSpeed;

	UPROPERTY(EditDefaultsOnly)
	float MaximumHealth = 100;

	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth;

	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = "PlayerHud");
	TSubclassOf<class UPlayerHud> WBP_PlayerHud;

	class UPlayerHud* MyHud;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float CurrentVelocity = 0;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void HoldWeapon(class AWeaponBase* Weapon);
	void DropWeapon();
	void FireBluePressed();
	void FireBlueReleased();
	void FireRedPressed();
	void FireRedReleased();
	void FireBlue(bool Toggle);
	void FireRed(bool Toggle);
	void Interact();

	UFUNCTION()
	void OnWeaponFired();

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void Move(FVector Direction, float Scale);
	void Turn(float Amount);
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void Pause();
	void DecreaseHealth();

	// Boolean flag to indicate aiming state.
	bool bIsAiming;
	// Boolean flag to indicate firing state.
	bool bIsFiring;

	// Boolean flag to indicate if the character has a weapon.
	bool bHasWeapon;

	bool bPaused;
};
