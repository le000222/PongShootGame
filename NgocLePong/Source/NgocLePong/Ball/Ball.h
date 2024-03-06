// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class NGOCLEPONG_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ball")
	class USphereComponent* BallCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ball")
	UStaticMeshComponent* VisualMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ball")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ball")
	class UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UPongHUD> WBP_PongHUD;

	class UPongHUD* MyHud;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PrintMessageOnScreen(FString Message);

};
