// Fill out your copyright notice in the Description page of Project Settings.


#include "Goal.h"
#include "../Ball/Ball.h"
#include "Components/BoxComponent.h"

// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GoalCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("GoalComponent"));

	SetRootComponent(GoalCollision);

}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AGoal::OnComponentOverlapBegin);
	//GoalCollision->OnComponentBeginOverlap.AddDynamic(this, &AGoal::OnComponentOverlapBegin);
	
}

void AGoal::OnComponentOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	FString Message = FString("OnComponentOverlapBegin: ") + OtherActor->GetName();
	PrintMessageOnScreen(Message);

	if (OtherActor)
	{
		ABall* Ball = Cast<ABall>(OtherActor);
		if (Ball)
		{
			Ball->Destroy();
		}
	}
}


// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoal::PrintMessageOnScreen(FString Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Red, Message);
	}
}

