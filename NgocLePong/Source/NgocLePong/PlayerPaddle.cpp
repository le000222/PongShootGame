// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPaddle.h"
#include "Components/BoxComponent.h"

// Sets default values
APlayerPaddle::APlayerPaddle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerPaddle = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerPaddle"));
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));

	PlayerPaddle->SetSimulatePhysics(true);
	PlayerPaddle->SetEnableGravity(false);
	PlayerPaddle->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	PlayerPaddle->SetCollisionProfileName("Pawn");
	PlayerPaddle->SetLinearDamping(10);

	SetRootComponent(PlayerPaddle);
	VisualMesh->SetupAttachment(RootComponent);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisualMesh->SetCollisionProfileName(TEXT("IgnoreAll"));

	//Restrict Movement and/or rotation of a Actor or Pawn
	PlayerPaddle->GetBodyInstance()->bLockXRotation = true;
	PlayerPaddle->GetBodyInstance()->bLockYRotation = true;
	PlayerPaddle->GetBodyInstance()->bLockZRotation = true;
	PlayerPaddle->GetBodyInstance()->bLockXTranslation = true;
	PlayerPaddle->GetBodyInstance()->bLockYTranslation = true;
	//PlayerPaddle->GetBodyInstance()->bLockZTranslation = true;

}

// Called when the game starts or when spawned
void APlayerPaddle::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &APlayerPaddle::OnHitActor);
	
}

void APlayerPaddle::OnHitActor(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		FString Message = FString("OnHitActor: ") + OtherActor->GetName();
		PrintMessageOnScreen(Message);
	}
}

void APlayerPaddle::OnCollisionSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->GetName() != "Floor") /* CHANGE LATER */
	{
		FString Message = FString("OnCollisionSphereHit: ") + OtherActor->GetName();
		PrintMessageOnScreen(Message);
	}
}

// Called every frame
void APlayerPaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller)
	{
		FVector Direction(0);
		if (MoveUpAmount != 0)
		{
			Direction = GetActorLocation() + MoveUpAmount;
			SetActorLocation(Direction);
		}
	}

}

// Called to bind functionality to input
void APlayerPaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent != NULL)
	{
		PlayerInputComponent->BindAxis("MoveUp", this, &APlayerPaddle::MoveUp);
	}
}

void APlayerPaddle::MoveUp(float Amount)
{
	MoveUpAmount = Amount;
}

void APlayerPaddle::PrintMessageOnScreen(FString Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Red, Message);
	}
}

