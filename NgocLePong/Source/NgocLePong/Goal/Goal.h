// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Goal.generated.h"

UENUM(BlueprintType)
enum class GoalPositionEnum
{
	Option1 UMETA(DisplayName = "Left"),
	Option2 UMETA(DisplayName = "Right"),
};

UCLASS()
class NGOCLEPONG_API AGoal : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AGoal();

	UPROPERTY(VisibleAnywhere, Category = "Goal")
	class UBoxComponent* GoalCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Position")
	TEnumAsByte<GoalPositionEnum> GoalPosition;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PrintMessageOnScreen(FString Message);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComponentOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

};
