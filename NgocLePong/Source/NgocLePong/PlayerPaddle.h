// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPaddle.generated.h"

UCLASS()
class NGOCLEPONG_API APlayerPaddle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPaddle();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerPaddle")
	class UBoxComponent* PlayerPaddle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerPaddle")
	USceneComponent* VisualMeshRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerPaddle")
	UStaticMeshComponent* VisualMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHitActor(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

private:
	float MoveUpAmount = 0;

	void MoveUp(float Amount);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PrintMessageOnScreen(FString Message);

};
