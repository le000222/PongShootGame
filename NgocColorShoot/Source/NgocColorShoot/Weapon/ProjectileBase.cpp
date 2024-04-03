// Copyright (c) 2017 GAM1528. All Rights Reserved.

#include "ProjectileBase.h"
#include "Engine/DamageEvents.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Default projectile parameters.
	Damage = 10.0f;
	Speed = 1200.0f;
	LifeTime = 1.0f;
}

void AProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//TODO:
		// Subscribe to actor's hit event. 
		//SUBSCRIBE to the AActors hit event. Look at this, another way to detect collisions. Un-Comment the line below
	AActor::OnActorHit.AddDynamic(this, &AProjectileBase::OnActorHit);
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	//TODO:
		/* Destroy the projectile after some time.*/
		//Un-Comment the line below
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AProjectileBase::DestroySelf, LifeTime, false);
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//TODO:
		/* Move the projectile in a forward direction.*/
		//Un-Comment the lines below
	FVector Location = GetActorLocation();
	Location += GetActorForwardVector() * Speed * DeltaTime;
	SetActorLocation(Location, true);
}

void AProjectileBase::OnActorHit(AActor* Self, AActor* Other, FVector NormalImpulse, const FHitResult& Hit)
{
	//TODO:
		// Cause damage to the hit actor.
		/*Un-Comment the lines below*/
	if (Other != nullptr)
	{
		FDamageEvent DamageEvent;
		Other->TakeDamage(Damage, DamageEvent, nullptr, this);
	}

	// Destroy self.
	Destroy();
}

void AProjectileBase::DestroySelf()
{
	// Destroy self.
	Destroy();
}
