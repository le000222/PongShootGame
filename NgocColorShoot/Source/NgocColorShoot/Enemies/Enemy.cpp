// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "../Player/MainCharacter.h"
#include "../Weapon/ProjectileBase.h"
#include "NavigationSystem.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Perception/PawnSensingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

AEnemy::AEnemy()
{
	//SET PrimaryActorTick.bCanEverTick to true	
	PrimaryActorTick.bCanEverTick = true;

	//CREATE The PawnSensingComp Component
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	//SUBSCRIBE to the OnSeePawn Event in the PawnSensingComp, passing in the OnPawnSeen function
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemy::OnPawnSeen);

	//SUBSCRIBE to the OnHearNoise Event in the PawnSensingComp, passing in the OnNoiseHeard function
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AEnemy::OnNoiseHeard);

	//SET the GuardState to EAIState::Idle
	GuardState = EAIState::Idle;	//or SetGuardState(EAIState::Idle);
}

void AEnemy::BeginPlay()
{
	//CALL Super
	Super::BeginPlay();
	//SET the OriginalRotator to the return value of GetActorRotation()
	OriginalRotator = GetActorRotation();
	//IF bPatrol is true
	if (bPatrol)
	{
		//CALL MoveToNextPatrolPoint()
		MoveToNextPatrolPoint();
	}
	
	OnActorHit.AddDynamic(this, &AEnemy::OnHitActor);
}

void AEnemy::EnemyTakeDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Take Damage"));
	float Damage = 1.0f;
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaximumHealth);

	if (CurrentHealth <= 0)
	{
		this->Destroy();
	}
}

void AEnemy::OnHitActor(AActor* SelfActor, AActor* Other, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Other != nullptr && Other->IsA<AProjectileBase>())
	{
		AProjectileBase* PB = Cast<AProjectileBase>(Other);
		if (PB->ProjectileColor == ProjectileColor::blue && this->EnemyColor == EnemyColor::blue)
		{
			this->EnemyTakeDamage();
		}
		else if (PB->ProjectileColor == ProjectileColor::red && this->EnemyColor == EnemyColor::red)
		{
			this->EnemyTakeDamage();
		}
	}
}

void AEnemy::OnPawnSeen(APawn* SeenPawn)
{
	//IF the SeenPawn is nullptr
	if (SeenPawn == nullptr)
	{
		//RETURN
		return;
	}
	//ENDIF

	//SET the TargetActor to SeenPawn
	TargetActor = SeenPawn;

	//CALL SetGuardState() passing in EAIState::Alerted
	SetGuardState(EAIState::Alerted);

	/*Stop Movement if Patrolling*/
	//GET the Controller for this Pawn
	AController* PlayerController = GetController();
	//IF the COntroller is NOT nullptr
	if (PlayerController)
	{
		//CALL StopMovement() on the Controller
		PlayerController->StopMovement();
		//CLEAR the TimerHandle_ResetState timer
		GetWorldTimerManager().ClearTimer(TimerHandle_ResetState);
		//START the TimerHandle_ResetState, to Start the ResetPatrol function after 3.0f seconds
		GetWorldTimerManager().SetTimer(TimerHandle_ResetState, this, &AEnemy::ResetPatrol, 3.0f);
	}
	//ENDIF
}

void AEnemy::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	//IF the GuardState is EAIState::Alerted
	if (GuardState == EAIState::Alerted)
	{
		//RETURN
		return;
	}
	//ENDIF

	//DrawDebugSphere --> Input params GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);
	UE_LOG(LogTemp, Warning, TEXT("heard"));

	//DECLARE a FVector called Direction and assign it to the Direction of where the Noise was heard, Distance between heard Location and this Actors Location, normalized
	FVector Direction = Location - GetActorLocation();
	//NORMALIZE the Direction
	Direction.Normalize();
	//DECLARE a FRotator called NewLookAt and Set it to the Rotation of the Direction RotationMatrix::MakeFromX(Direction).Rotator()
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator(); //Can use the one in kismet library (LookAt()) X is to look forward dir
	//SET the NewLookAt's Pitch to 0
	NewLookAt.Pitch = 0.0f;
	//SET the NewLookAt's Roll to 0
	NewLookAt.Roll = 0.0f;
	//CALL SetActorRotation() passing in NewLookAt
	SetActorRotation(NewLookAt);
	//CLEAR the TimerHandle_ResetState
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetState);
	//START the TimerHandle_ResetState, to Start the ResetState function after 3.0f seconds
	GetWorldTimerManager().SetTimer(TimerHandle_ResetState, this, &AEnemy::ResetState, 3.0f);

	//CALL SetGuardState to EAIState::Suspicious
	SetGuardState(EAIState::Suspicious);

	/*Stop Movement if Patrolling*/
	//GET the Controller for this Pawn
	AController* PlayerController = GetController();
	//IF the Controller is NOT nullptr
	if (PlayerController)
	{
		//CALL StopMovement() on the Controller
		PlayerController->StopMovement();
	}
	//ENDIF
}

void AEnemy::ResetState()
{
	//IF GuardState is EAIState::Alerted 
	if (GuardState == EAIState::Alerted)
	{
		//RETURN
		return;
	}
	//ENDIF

	//CALL SetActorRotation() passing in the OriginalRotator
	SetActorRotation(OriginalRotator);
	//CALL ResetPatrol()
	ResetPatrol();
}

void AEnemy::ResetPatrol()
{
	//SET TargetActor to nullptr
	TargetActor = nullptr;
	//CALL SetGuardState() passing in EAIState::Idle
	SetGuardState(EAIState::Idle);

	/*Stop Investigating ... if we are a patrolling pawn. Pick a new Patrol point to move to*/
	//IF bPatrol is true
	if (bPatrol)
	{
		//CALL MoveToNextPatrolPoint()
		MoveToNextPatrolPoint();
	}
	//ENDIF
}


void AEnemy::SetGuardState(EAIState NewState)
{
	//IF GuardState is NewState (Do Nothing, already in the state)
	if (GuardState == NewState)
	{
		//RETURN
		return;
	}
	//ENDIF

	//SET GuardState to NewState
	GuardState = NewState;
}

void AEnemy::MoveToNextPatrolPoint()
{
	//IF CurrentPatrolPoint is nullptr OR CurrentPatrolPoint is SecondPatrolPoint 
	if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == SecondPatrolPoint)
	{
		//SET CurrentPatrolPoint to the FirstPatrolPoint
		CurrentPatrolPoint = FirstPatrolPoint;
	}
	//ELSE
	else
	{
		//SET CurrentPatrolPoint to the SecondPatrolPoint
		CurrentPatrolPoint = SecondPatrolPoint;
	}
	//ENDIF

	/*Here is where we move the character along the NavMesh*/
	//CALL UAIBlueprintHelperLibrary::SimpleMoveToActor() passing in this Pawn's Controller ( CALL GetController() ), and the target (CurrentPatrolPoint)
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);

}

void AEnemy::Tick(float DeltaTime)
{
	//CALL Super
	Super::Tick(DeltaTime);

	/*Patrol Goal Checks*/
	//IF CurrentPatrolPoint is not nullptr
	if (CurrentPatrolPoint)
	{
		//DECLARE a FVector called Delta and Calculate the Delta Distance between this Actors Location and the CurrentPatrolPoint's Actor Location
		FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();

		//DECLARE a float called DistanceToGoal and assign it to the magnitude of the Delta, by CALLING the Size() function on Delta
		float DistanceToGoal = Delta.Size();

		UE_LOG(LogTemp, Warning, TEXT("DistanceToGoal = %f"), DistanceToGoal);

		/*Check if we are within 150 units of our goal. If so, pick a new patrol point*/
		//IF DistanceToGoal is LESS THAN 150
		if (DistanceToGoal < 150)
		{
			//CALL MoveToNextPatrolPoint()
			MoveToNextPatrolPoint();
		}
		//ENDIF
	}
	//ENDIF

	 /*Rotate towards the TargetActor if we have one*/
	//IF TargetActor is NOT nullptr
	if (TargetActor)
	{
		//DECLARE a FVector called MyLoc and assign it to the return value of GetActorLocation()
		FVector MyLoc = GetActorLocation();
		//DECLARE a FVector called TargetLoc and Assign it to the TargetActor's Location
		FVector TargetLoc = TargetActor->GetActorLocation();
		//DECLARE a FVector called Dir and assign it to the distance between the TargetLoc and MyLoc
		FVector Dir = (TargetLoc - MyLoc);
		//NORMALIZE the Dir vector
		Dir.Normalize();
		//CALL SetActorRotation() passing in a Lerp'd value between the this Actors Rotation and the Dir's Rotation  -- >FMath::Lerp(GetActorRotation(), Dir.Rotation(), 0.05f)
		SetActorRotation(FMath::Lerp(GetActorRotation(), Dir.Rotation(), 0.05f));

		if (GuardState == EAIState::Alerted)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), TargetActor);
		}
	}
	//ENDIF
}

