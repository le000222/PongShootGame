// Fill out your copyright notice in the Description page of Project Settings.

#include "Ball.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Ball"));
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));

	BallCollision->SetCollisionProfileName("Pawn");
	BallCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	SetRootComponent(BallCollision);
	VisualMesh->SetupAttachment(RootComponent);
	ArrowComponent->SetupAttachment(RootComponent);
	SetActorEnableCollision(true);

	BallCollision->GetBodyInstance()->bLockXTranslation = true;

	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->InitialSpeed = 300.f;
	ProjectileMovement->MaxSpeed = 600.f;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 1.1;
	ProjectileMovement->Friction = 0.f;
	
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(ArrowComponent->GetRelativeRotation());
	FVector ArrowDirection = GetActorForwardVector();
	ProjectileMovement->Velocity = ArrowDirection * ProjectileMovement->InitialSpeed;
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABall::PrintMessageOnScreen(FString Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Red, Message);
	}
}

