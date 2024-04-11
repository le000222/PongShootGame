// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted,
	Attack
};

UCLASS()
class NGOCCOLORSHOOT_API AEnemy : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()//Must be a UFUnc otherwise we cant bind this function
		void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	FRotator OriginalRotator;

	UFUNCTION()
	void ResetState();

	UFUNCTION()
	void ResetPatrol();

	FTimerHandle TimerHandle_ResetState;
	FTimerHandle TimerHandle_ResetPatrol;

	EAIState GuardState;

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);

	//Challenge - Adding Patrol
	UPROPERTY(EditInstanceOnly, Category = "AI") //Can set in editor for each instance. CAN BE CHANGED ONLY FROM BLUEPRINTS
		bool bPatrol;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol")) //Will be enabled if bPatrol is true in the editor props
		AActor* FirstPatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))//Will be enabled if bPatrol is true in the editor props
		AActor* SecondPatrolPoint;

	AActor* CurrentPatrolPoint;

	UFUNCTION()
	void MoveToNextPatrolPoint();


	TSubclassOf<class AWeaponBase> WeaponBlueprint;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	AActor* TargetActor;

};
