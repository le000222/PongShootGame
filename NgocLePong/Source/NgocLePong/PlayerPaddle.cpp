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

	SetRootComponent(PlayerPaddle);
	VisualMesh->SetupAttachment(RootComponent);

	VisualMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	VisualMesh->SetCollisionProfileName(TEXT("IgnoreAll"));
}

void APlayerPaddle::MoveRight(float Amount)
{
}

// Called when the game starts or when spawned
void APlayerPaddle::BeginPlay()
{
	Super::BeginPlay();

	PlayerPaddle->OnComponentHit.AddDynamic(this, &APlayerPaddle::OnCollisionSphereHit);
	
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

}

// Called to bind functionality to input
void APlayerPaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPaddle::MoveRight);

}

void APlayerPaddle::MoveRight(float Amount)
{
	//PlayerPaddle->AddInputVector(GetActorRightVector() * Amount);
}

void APlayerPaddle::PrintMessageOnScreen(FString Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Red, Message);
	}
}

