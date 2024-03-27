// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "../Weapon/Weapon.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColorShoot = CreateDefaultSubobject<UBoxComponent>(TEXT("ColorShoot"));
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	ColorShoot->SetSimulatePhysics(true);
	ColorShoot->SetEnableGravity(true);
	ColorShoot->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ColorShoot->SetCollisionProfileName("Pawn");
	ColorShoot->SetLinearDamping(10);

	SetRootComponent(ColorShoot);
	SpringArm->SetupAttachment(RootComponent);
	VisualMesh->SetupAttachment(RootComponent);
	MainCamera->SetupAttachment(SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisualMesh->SetCollisionProfileName(TEXT("IgnoreAll"));

	CurrentWeapon = nullptr;
	CurrentHealth = MaximumHealth;

	bIsAiming = false;
	bIsFiring = false;
	bPaused = false;
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMainPlayer::HoldWeapon(AWeapon* Weapon)
{
	check(Weapon != nullptr && "Passed a null weapon!");
	//TODO Lab3 ACharacterBase::HoldWeapon(...):
	/* Drop currently carried weapon first.*/
	//CALL DropWeapon()
	DropWeapon();

	/* Attach weapon to the character.*/
	//SET CurrentWeapon to weapon
	CurrentWeapon = Weapon;
	//CALL Attach() on the CurrentWeapon and pass in this
	//CurrentWeapon->Attach(this);

	//CALL Clear() on the CurrentWeapon's OnWeaponFired event
	//CurrentWeapon->OnWeaponFired.Clear();
	/* Subscribe to weapon's events.*/
	//SUBSCRIBE to the CurrentWeapon's OnWeaponFired and pass in (this, &ACharacterBase::OnWeaponFired)
	//CurrentWeapon->OnWeaponFired.AddDynamic(this, &ACharacterBase::OnWeaponFired);
}

void AMainPlayer::DropWeapon()
{
	if (CurrentWeapon != NULL)
	{
		/* Unsubscribe from weapon's events.*/
		//CALL RemoveDynamic(this, &ACharacterBase::OnWeaponFired) on the CurrentWeapon's OnWeaponFired event
		//CurrentWeapon->OnWeaponFired.RemoveDynamic(this, &ACharacterBase::OnWeaponFired);

		/* Detach weapon from the character.*/
		//CALL Detach() on the CurrentWeapon()
		//CurrentWeapon->Detach();
		//SET CurrentWeapon to null
		CurrentWeapon = nullptr;

		/* Reset weapon states states.*/
		//SET bIsFiring to false
		bIsFiring = false;
		//SET bIsAiming to false
		bIsAiming = false;
	}
}

void AMainPlayer::MoveRight(float Amount)
{
	if (!bPaused)
		FloatingPawnMovement->AddInputVector(GetActorRightVector() * Amount);
}

void AMainPlayer::MoveForward(float Amount)
{
	if (!bPaused)
		FloatingPawnMovement->AddInputVector(GetActorForwardVector() * Amount);
}

void AMainPlayer::Turn(float Amount)
{
	if (!bPaused)
		AddControllerYawInput(Amount);
}

void AMainPlayer::Pause()
{
	//UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("MainMenu")));
	bPaused = !bPaused;

}

//void AMainPlayer::Fire(bool Toggle)
//{
//	if (CurrentWeapon)
//	{
//		if (Toggle)
//		{
//			CurrentWeapon->PullTrigger();
//		}
//		else
//		{
//			CurrentWeapon->ReleaseTrigger();
//		}
//
//		//SET bIsFiring to Toggle
//		bIsFiring = Toggle;
//	}
//}

//void AMainPlayer::Aim(bool Toggle)
//{
//	bIsAiming = Toggle;
//}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainPlayer::OnHitActor(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMainPlayer::Turn);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AMainPlayer::Pause);
}


AWeapon* AMainPlayer::GetCurrentWeapon()
{
	return CurrentWeapon;
}

