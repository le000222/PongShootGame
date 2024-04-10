// Copyright (c) 2017 GAM1528. All Rights Reserved.

#include "WeaponBase.h"

#include <Components/SkeletalMeshComponent.h>
#include <Components/PrimitiveComponent.h>
#include <Components/ArrowComponent.h>
#include "../Player/MainCharacter.h"

AWeaponBase::AWeaponBase() :
	Primitive(nullptr),
	Muzzle(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	// Set default variables.
	FireRate = 10.0f;

	MaximumAmmo = 30;
	CurrentAmmo = -1;
}

void AWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//TODO Lab3 AWeaponBase::PostInitializeComponents():
		/* Get the root primitive component.*/
		//SET Primitive by Casting the RootComponent to a UPrimitiveComponent. CALL GetRootComponent(). The RootComponent is the weapon mesh. A StaticMeshComponent that will be added via the Blueprint for this Actor
	Primitive = Cast<UPrimitiveComponent>(GetRootComponent());


	/*Assert - If the Primitive is not null, continue, otherwise CRASH with the reason. Leave this commented*/
	//check(Primitive != nullptr && "Actor is mising a primitive component!");

	/* Get the muzzle arrow component.*/
	//DECLARE a auto variable called Components and SET it to the return value of GetComponents()
	auto Components = GetComponents();
	for (auto Component : Components)
	{
		if (Component->GetFName() == "Muzzle")
		{
			//SET the Muzzle to the Component, Cast it as a UArrowComponent
			Muzzle = Cast<UArrowComponent>(Component);
			//BREAK
			break;
		}
		//ENDIF
	}
	//END FOR

	/* initialize default values.*/
	//SET MaximumAmmo to the return value of FMath::Max(1, MaximumAmmo)
	MaximumAmmo = FMath::Max(1, MaximumAmmo);
	//SET CurrentAmmo to the return value of FMath::Min(CurrentAmmo, MaximumAmmo)
	CurrentAmmo = FMath::Min(CurrentAmmo, MaximumAmmo);

	/* Set current ammo to maximum ammo.*/
	//IF the CurrentAmmo is LESS than 0
	if (CurrentAmmo < 0)
	{
		//SET CurrentAmmo to MaximumAmmo
		CurrentAmmo = MaximumAmmo;
	}
	//ENDIF
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::Attach(class APawn* Character)
{
	verify(Character != nullptr && "Weapon's attach function called with null character!");

	//TODO Lab3 AWeaponBase::Attach(...):
	/* Set owner of this weapon.*/
	//CALL SetOwner() and pass in Character
	SetOwner(Character);
	//ALSO usefull to pass along
	SetInstigator(Character);

	/* Disable weapon's physics.*/
	//CALL SetActorEnableCollision() to false
	SetActorEnableCollision(false);
	//CALL SetSimulatePhysics() in the Primitive and pass in false, disabling physics
	Primitive->SetSimulatePhysics(false);

	/* Attach weapon to the character's mesh.*/
	if (PositionWeapon == EWeaponPosition::Option1)
		AttachToComponent(Cast<AMainCharacter>(Character)->BlueWeaponMount, FAttachmentTransformRules::SnapToTargetIncludingScale);
	else
		AttachToComponent(Cast<AMainCharacter>(Character)->RedWeaponMount, FAttachmentTransformRules::SnapToTargetIncludingScale);

}

void AWeaponBase::Detach()
{
	//TODO Lab3 AWeaponBase::Detach():
	/* Stop firing when the weapon is detached.*/
	//CALL ReleaseTrigger()
	ReleaseTrigger();

	/* Unset owner of the weapon.*/
	//CALL SetOwner() to null
	SetOwner(NULL);

	/* Re-enable weapon's physics.*/
	//CALL SetActorEnableCollision() and pass in true
	SetActorEnableCollision(true);
	//CALL SetSimulatePhysics() on the Primitive to true
	Primitive->SetSimulatePhysics(true);

	/* Detach weapon from the character's mesh. */
	//Call DetachFromActor() and pass in FDetachmentTransformRules::KeepWorldTransform
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AWeaponBase::PullTrigger()
{

	auto& TimerManager = GetWorld()->GetTimerManager();

	/* Check current ammo value before attempting to firing.*/
	if (CurrentAmmo > 0)
	{
		/*Start the firing timer and use the remaining time of the previous timer.*/
		//DECLARE a float called RemainingTime and assign it to the Max value between the time remaining before the specified timer is called and 0. FMath::Max(TimerManager.GetTimerRemaining(FireTimer), 0.0f);
		float RemainingTime = FMath::Max(TimerManager.GetTimerRemaining(FireTimer), 0.0f);
		//CALL SetTimer() on the TimerManager and pass in  - (FireTimer, this, &AWeaponBase::Fire, 1.0f / FireRate, true, RemainingTime)
		TimerManager.SetTimer(FireTimer, this, &AWeaponBase::Fire, 1.0f / FireRate, true, RemainingTime);
	}
	//ENDIF
}

void AWeaponBase::ReleaseTrigger()
{
	//TODO Lab3 AWeaponBase::ReleaseTrigger():
	//DECLARE a auto variable called TimerManager and assign it to the return value of GetWorld()->GetTimerManager()
	auto& TimerManager = GetWorld()->GetTimerManager();

	/* Replace timer with one that will prevent the primary fire timer from triggering again too quickly.*/
	//IF The FireTimer Exists --> TimerManager.TimerExists(FireTimer)
	if (TimerManager.TimerExists(FireTimer))
	{
		//DECLARE a float called RemainingTime and SET it to the return value of TimerManager.GetTimerRemaining(FireTimer)
		float RemainingTime = TimerManager.GetTimerRemaining(FireTimer);
		//CALL SetTimer() on the TimerManager and pass in (FireTimer, this, &AWeaponBase::ClearFireTimer, RemainingTime, false)
		TimerManager.SetTimer(FireTimer, this, &AWeaponBase::ClearFireTimer, RemainingTime, false);
	}
	//ENDIF
}

void AWeaponBase::Fire()
{
	//TODO Lab3 AWeaponBase::Fire():
	/* Check current ammo value before actually firing.*/
	//IF CurrentAmmo is GREATER than 0
	if (CurrentAmmo > 0)
	{
		/*Spawn a projectile.*/
		//DECLARE a FActorSpawnParameters called SpawnParams
		FActorSpawnParameters SpawnParams;
		//SET the SpawnParams.Instigator to the Owner of this Actor, by Calling GetOwner() and Casting it as a APawn. This Actor is ATTACHED to the Character, making the Character the owner of this Actor
		SpawnParams.Instigator = Cast<APawn>(GetOwner());

		//DECLARE a FTransform called Transform and SET it to the return value of Muzzle->GetComponentToWorld()
		FTransform Transform = Muzzle->GetComponentToWorld();
		//SPAWN the Projectile, by calling SpawnActor() on the World and pass in (ProjectileType, Transform.GetLocation(), Transform.GetRotation().Rotator(), SpawnParams)
		GetWorld()->SpawnActor(ProjectileType, &Transform, SpawnParams);

		/* Broadcast a weapon fired event.*/
		//CALL Broadcast() on the OnWeaponFired event
		OnWeaponFired.Broadcast();

		/*Decrement the ammo count.*/
		//SUBTRACT 1 from current Ammo
		CurrentAmmo--;
	}
	//ENDIF
}

void AWeaponBase::ClearFireTimer()
{
	//TODO Lab3 AWeaponBase::ClearFireTimer():
	/* Clear the timer after a delay set in ReleaseTrigger() function.*/
	//Un-Comment the line below
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
}

FVector AWeaponBase::GetMuzzleLocation() const
{
	/* Return the world location of the 'Muzzle' component.
	 'Muzzle' is assumed to be a pointer to a component (e.g., a mesh or a particle system) attached to an actor (such as a weapon).
	 'GetComponentToWorld()' retrieves a transform that represents the component's position, rotation, and scale in the world space.
	 'GetLocation()' then extracts the location vector (position in world space) from this transform.
	 The combined call effectively returns the current world space location of the 'Muzzle' component, which can be used for positioning projectiles, effects, or for aiming calculations.*/
	return Muzzle->GetComponentToWorld().GetLocation();
}