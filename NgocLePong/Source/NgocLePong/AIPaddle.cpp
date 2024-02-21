// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPaddle.h"
#include "Kismet/GameplayStatics.h"

AAIPaddle::AAIPaddle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAIPaddle::BeginPlay()
{
	Super::BeginPlay();

	Ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));
}

void AAIPaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Ball != NULL)
	{
		FVector BallLocation = Ball->GetActorLocation();
		FVector TargetLocation = GetActorLocation();
		TargetLocation.Z = Ball->GetActorLocation().Z;
		FVector NewLocation = FMath::Lerp(GetActorLocation(), TargetLocation, 0.08f);
		SetActorLocation(NewLocation);
	}
}

void AAIPaddle::PrintMessageOnScreen(FString Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Red, Message);
	}
}
