// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "../Weapon/WeaponBase.h"
#include "../UI/PlayerHud.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/WidgetComponent.h"
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
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));

	BlueWeaponMount->SetupAttachment(GetRootComponent());
	RedWeaponMount->SetupAttachment(GetRootComponent());
	SpringArm->SetupAttachment(GetRootComponent());
	HealthBar->SetupAttachment(GetRootComponent());
	MainCamera->SetupAttachment(SpringArm);

	SpringArm->bDoCollisionTest = false;

	CurrentBlueWeapon = nullptr;
	CurrentRedWeapon = nullptr;
	CurrentHealth = MaximumHealth;

	bIsAiming = false;
	bIsFiring = false;
	bPaused = false;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (WBP_PlayerHud)
	{
		MyHud = CreateWidget<UPlayerHud>(GetWorld(), WBP_PlayerHud);
		MyHud->AddToViewport();
	}

	SetCanBeDamaged(true);

	SetReplicates(true);
	SetReplicateMovement(true);
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bEnableClickEvents = true;
		PC->bShowMouseCursor = true;
	}
	
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
		AItemBase* Item = Cast<AItemBase>(items[0]);
		if (Item == nullptr)
			return;

		AWeaponBase* Weapon = Cast<AWeaponBase>(Item);
		if (Weapon != nullptr)
		{
			HoldWeapon(Weapon);
		}
	}
}

void AMainCharacter::HoldWeapon(AWeaponBase* Weapon)
{
	check(Weapon != nullptr && "Passed a null weapon!");

	if (Weapon->PositionWeapon == EWeaponPosition::Option1) {
		CurrentBlueWeapon = Weapon;

		CurrentBlueWeapon->Attach(this);

		CurrentBlueWeapon->OnWeaponFired.Clear();
	}

	else {
		CurrentRedWeapon = Weapon;

		CurrentRedWeapon->Attach(this);

		CurrentRedWeapon->OnWeaponFired.Clear();

		//CurrentRedWeapon->OnWeaponFired.AddDynamic(this, &AMainCharacter::OnWeaponFired);

	}
}

void AMainCharacter::OnWeaponFired()
{
	UE_LOG(LogTemp, Warning, TEXT("Red is here"));
}

void AMainCharacter::DropWeapon()
{
	if (CurrentBlueWeapon != NULL)
	{
		/* Detach weapon from the character.*/
		CurrentBlueWeapon->Detach();
		CurrentBlueWeapon = nullptr;
	}

	else if (CurrentRedWeapon != NULL) 
	{
		CurrentRedWeapon->Detach();
		CurrentRedWeapon = nullptr;
	}
		/* Reset weapon states states.*/
		bIsFiring = false;
		bIsAiming = false;
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

void AMainCharacter::FireBluePressed()
{
	FireBlue(true);
}

void AMainCharacter::FireRedPressed()
{
	FireRed(true);
}


void AMainCharacter::FireBlueReleased()
{
	FireBlue(false);
}

void AMainCharacter::FireRedReleased()
{
	FireRed(false);
}

void AMainCharacter::FireBlue(bool Toggle)
{
	if (CurrentBlueWeapon)
	{
		if (Toggle)
		{
			CurrentBlueWeapon->PullTrigger();
		}
		else
		{
			CurrentBlueWeapon->ReleaseTrigger();
		}

		//SET bIsFiring to Toggle
		bIsFiring = Toggle;
	}
}

void AMainCharacter::FireRed(bool Toggle)
{
	if (CurrentRedWeapon)
	{
		if (Toggle)
		{
			CurrentRedWeapon->PullTrigger();
		}
		else
		{
			CurrentRedWeapon->ReleaseTrigger();
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

	InputComponent->BindAction("FireBlue", IE_Pressed, this, &AMainCharacter::FireBluePressed);
	InputComponent->BindAction("FireBlue", IE_Released, this, &AMainCharacter::FireBlueReleased);
	InputComponent->BindAction("FireRed", IE_Pressed, this, &AMainCharacter::FireRedPressed);
	InputComponent->BindAction("FireRed", IE_Released, this, &AMainCharacter::FireRedReleased);

	InputComponent->BindAction("DecreaseHealth", IE_Released, this, &AMainCharacter::DecreaseHealth);

}

