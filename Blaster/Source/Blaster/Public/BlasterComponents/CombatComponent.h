#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

/* ÃÑ »ç°Å¸® */
#define TRACE_LENGTH 80000.f

class AWeapon;
class ABlasterCharacter;
class USphereComponent;
class ABlasterMainController;
class ABlasterHUD;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	friend class ABlasterCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void EquipWeapon(AWeapon* WeaponToEquip);

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnRep_EquippedWeapon();
	
	/* Aiming */

	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	/* Fire */

	UFUNCTION()
	void FireButtonPressed(bool bPressed);

	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	/* Trace */
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

protected:

	TObjectPtr<ABlasterCharacter> Character;
	TObjectPtr<ABlasterMainController> Controller;
	TObjectPtr<ABlasterHUD> HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	TObjectPtr<AWeapon> EquippedWeapon;
		
	UPROPERTY(Replicated)
	uint8 bAiming : 1;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;
	
	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;

	FVector HitTarget;


	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomedFOV = 30.f;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);


	float CrossHairVelocityFactor;
	float CrossHairInAirFactor;
};
