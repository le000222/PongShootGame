// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "../Weapon/WeaponBase.h"

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
	WeaponMount = CreateDefaultSubobject<UArrowComponent>("Weapon Mount");

	ColorShoot->SetSimulatePhysics(true);
	ColorShoot->SetEnableGravity(true);
	ColorShoot->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ColorShoot->SetCollisionProfileName("Pawn");
	ColorShoot->SetLinearDamping(10);

	SetRootComponent(ColorShoot);
	VisualMesh->SetupAttachment(RootComponent);
	WeaponMount->SetupAttachment(RootComponent);
	SpringArm->SetupAttachment(RootComponent);
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

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	ColorShoot->OnComponentHit.AddDynamic(this, &AMainPlayer::OnCollisionSphereHit);

	SetReplicates(true);
	SetReplicateMovement(true);
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainPlayer::Interact()
{
	//IF InteractSphere is null
	if (!InteractSphere)
	{
		//ASSIGN InteractSphere to the return value of NewObject<USphereComponent>(this, USphereComponent::StaticClass())
		InteractSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass());
		//CALL RegisterComponent() on InteractSphere
		InteractSphere->RegisterComponent();
		//CALL SetRelativeLocation() on InteractSphere passing in GetActorLocation()  
		InteractSphere->SetRelativeLocation(this->GetActorLocation());

		//CALL SetCollisionEnabled() on InteractSphere passing in ECollisionEnabled::QueryAndPhysics  
		InteractSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//CALL SetGenerateOverlapEvents() on InteractSphere passing in true 
		InteractSphere->SetGenerateOverlapEvents(true);

		DrawDebugSphere(GetWorld(), GetActorLocation(), InteractSphere->GetUnscaledSphereRadius(), 26, FColor(181, 0, 0), false, 2, 0, 2);

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
			//RETURN
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

void AMainPlayer::HoldWeapon(AWeaponBase* Weapon)
{
	check(Weapon != nullptr && "Passed a null weapon!");

	DropWeapon();

	/* Attach weapon to the character.*/
	//SET CurrentWeapon to weapon
	CurrentWeapon = Weapon;
	//CALL Attach() on the CurrentWeapon and pass in this
	CurrentWeapon->Attach(this);

	//CALL Clear() on the CurrentWeapon's OnWeaponFired event
	CurrentWeapon->OnWeaponFired.Clear();
}

void AMainPlayer::DropWeapon()
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

void AMainPlayer::FirePressed()
{
	Fire(true);
}

//WEEK8
void AMainPlayer::FireReleased()
{
	Fire(false);
}

void AMainPlayer::Fire(bool Toggle)
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

void AMainPlayer::DecreaseHealth()
{
	CurrentHealth -= 10;
}

void AMainPlayer::OnHitActor(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
}

void AMainPlayer::OnCollisionSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AMainPlayer::Interact);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AMainPlayer::FirePressed);
	InputComponent->BindAction("Fire", IE_Released, this, &AMainPlayer::FireReleased);

	InputComponent->BindAction("DecreaseHealth", IE_Released, this, &AMainPlayer::DecreaseHealth);
}

