// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABoard::ABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMeshRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VisualMeshRoot"));
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SetRootComponent(VisualMeshRoot);
	VisualMesh->SetupAttachment(RootComponent);
	SpringArm->SetupAttachment(RootComponent);
	MainCamera->SetupAttachment(SpringArm);

	SpringArm->bDoCollisionTest = false;
}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	PC->SetViewTarget(this);
	
}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

