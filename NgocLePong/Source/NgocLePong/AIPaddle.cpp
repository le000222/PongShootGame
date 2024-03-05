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
	UE_LOG(LogTemp, Warning, TEXT("ai starting"));

}

void AAIPaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("Going to Tick"));
	Ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));

	if (Ball != NULL)
	{
		FVector BallLocation = Ball->GetActorLocation();
		FVector TargetLocation = GetActorLocation();
		TargetLocation.Z = Ball->GetActorLocation().Z;
		FVector NewLocation = FMath::Lerp(GetActorLocation(), TargetLocation, 0.08f);
		SetActorLocation(NewLocation);
	}
	else
	{
		PrintMessageOnScreen("Ball is NULL");
	}
}

void AAIPaddle::PrintMessageOnScreen(FString Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Red, Message);
	}
}
