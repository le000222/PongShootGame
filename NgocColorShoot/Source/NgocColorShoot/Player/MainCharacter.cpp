// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "../Weapon/WeaponBase.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/PlayerController.h>

// Sets default values
AMainCharacter::AMainCharacter() : 
	SkeletalMesh(nullptr)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	BlueWeaponMount = CreateDefaultSubobject<UArrowComponent>(TEXT("BlueMount"));
	RedWeaponMount = CreateDefaultSubobject<UArrowComponent>(TEXT("RedMount"));

	SpringArm->SetupAttachment(GetRootComponent());
	BlueWeaponMount->SetupAttachment(GetRootComponent());
	RedWeaponMount->SetupAttachment(GetRootComponent());
	MainCamera->SetupAttachment(SpringArm);

	SpringArm->bDoCollisionTest = false;

	CurrentWeapon = nullptr;
	CurrentHealth = MaximumHealth;

	bIsAiming = false;
	bIsFiring = false;
	bPaused = false;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetCanBeDamaged(true);

	SetReplicates(true);
	SetReplicateMovement(true);
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->CurrentHealth = MaximumHealth;

}

void AMainCharacter::Interact()
{
	if (!InteractSphere)
	{
		InteractSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass());
		//CALL RegisterComponent() on InteractSphere
		InteractSphere->RegisterComponent();
		//CALL SetRelativeLocation() on InteractSphere passing in GetActorLocation()  
		InteractSphere->SetRelativeLocation(this->GetActorLocation());

		//CALL SetCollisionEnabled() on InteractSphere passing in ECollisionEnabled::QueryAndPhysics  
		InteractSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//CALL SetGenerateOverlapEvents() on InteractSphere passing in true 
		InteractSphere->SetGenerateOverlapEvents(true);
	}
	else
	{
		//CALL DestroyComponent() on InteractSphere 
		InteractSphere->DestroyComponent();
		//SET InteractSphere to null
		InteractSphere = nullptr;
		//CALL Interact()
		Interact();
		//InteractSphere->SetRelativeLocation(this->GetActorLocation());

	}

	//DECLARE a TArray<AActor*> called items
	TArray<AActor*> items;
	//CALL GetOverlappingActors() on InteractSphere passing in items, AItemBase::StaticClass()  
	InteractSphere->GetOverlappingActors(items, AItemBase::StaticClass());
	//If we have any items
	if (items.Num() > 0)
	{
		//DECLARE variable called Item of type AItemBase* and assign it to the return value of  Cast<AItemBase>(items[0])
		AItemBase* Item = Cast<AItemBase>(items[0]);
		//IF Item is null
		if (Item == nullptr)
			return;

		//DECLARE variable called Weapon of type AWeaponBase* and assign it to the return value of  Cast<AWeaponBase>(Item)
		AWeaponBase* Weapon = Cast<AWeaponBase>(Item);
		//IF Weapon not null
		if (Weapon != nullptr)
		{
			//CALL HoldWeapon() passing in Weapon 
			HoldWeapon(Weapon);
		}
	}
}

void AMainCharacter::HoldWeapon(AWeaponBase* Weapon)
{
	check(Weapon != nullptr && "Passed a null weapon!");

	/* Attach weapon to the character.*/
	//SET CurrentWeapon to weapon
	CurrentWeapon = Weapon;
	//CALL Attach() on the CurrentWeapon and pass in this
	CurrentWeapon->Attach(this);

	//CALL Clear() on the CurrentWeapon's OnWeaponFired event
	CurrentWeapon->OnWeaponFired.Clear();
}

void AMainCharacter::DropWeapon()
{
	if (CurrentWeapon != NULL)
	{
		/* Detach weapon from the character.*/
		//CALL Detach() on the CurrentWeapon()
		CurrentWeapon->Detach();
		//SET CurrentWeapon to null
		CurrentWeapon = nullptr;

		/* Reset weapon states states.*/
		//SET bIsFiring to false
		bIsFiring = false;
		//SET bIsAiming to false
		bIsAiming = false;
	}
}

void AMainCharacter::Move(FVector Direction, float Scale)
{
	AddMovementInput(Direction, Scale);
}

void AMainCharacter::MoveRight(float Scale)
{
	if (!bPaused)
		Move(GetActorRightVector(), Scale);
}

void AMainCharacter::MoveForward(float Scale)
{
	if (!bPaused)
		Move(GetActorForwardVector(), Scale);
}

void AMainCharacter::Turn(float Scale)
{
	if (!bPaused)
		AddControllerYawInput(Scale);
}

void AMainCharacter::Pause()
{
	bPaused = !bPaused;

}

void AMainCharacter::FirePressed()
{
	Fire(true);
}

//WEEK8
void AMainCharacter::FireReleased()
{
	Fire(false);
}

void AMainCharacter::Fire(bool Toggle)
{
	if (CurrentWeapon)
	{
		if (Toggle)
		{
			CurrentWeapon->PullTrigger();
		}
		else
		{
			CurrentWeapon->ReleaseTrigger();
		}

		//SET bIsFiring to Toggle
		bIsFiring = Toggle;
	}
}

void AMainCharacter::DecreaseHealth()
{
	CurrentHealth -= 10;
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AMainCharacter::Pause);
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AMainCharacter::Interact);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::FirePressed);
	InputComponent->BindAction("Fire", IE_Released, this, &AMainCharacter::FireReleased);

	InputComponent->BindAction("DecreaseHealth", IE_Released, this, &AMainCharacter::DecreaseHealth);

}

