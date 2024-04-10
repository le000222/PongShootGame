
#pragma once

#include <GameFramework/Actor.h>
#include "ItemBase.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFiredSignature);

UENUM(BlueprintType)
enum class EWeaponPosition
{
	Option1 UMETA(DisplayName = "Blue"),
	Option2 UMETA(DisplayName = "Red"),
};

UCLASS()
class NGOCCOLORSHOOT_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaximumAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int CurrentAmmo;

	UPROPERTY(EditDefaultsOnly)
	class UClass* ProjectileType;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponFiredSignature OnWeaponFired;

	UPROPERTY(EditAnywhere, Category = "Weapon Position")
	EWeaponPosition PositionWeapon;

public:
	void Attach(class APawn* Character);
	void Detach();

	void PullTrigger();
	void ReleaseTrigger();

	FVector GetMuzzleLocation() const;

protected:
	AWeaponBase();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Fire();

private:
	void ClearFireTimer();

	FTimerHandle FireTimer;

	class USphereComponent* CollsionSphere;
	class UPrimitiveComponent* Primitive;
	class UArrowComponent* Muzzle;
};