// BlasterCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BlasterTypes/TurningInPlace.h"
#include "Interface/InteractWithCrossHairsInterface.h"
#include "BlasterCharacter.generated.h"

class USpringArmComponent;
class UInputMappingContext;
class UCameraComponent;
class UBInputConfig;
class UWidgetComponent;
class AWeapon;
class UCombatComponent;
class UAnimMontage;
class ABlasterMainController;

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrossHairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:

	virtual void PostInitializeComponents() override;

	/* 캐릭터가 오버래핑하고 있는 무기 Set*/
	void SetOverlappingWeapon(AWeapon* Weapon);

	/* 현재 무기를 착용하고 있는지*/
	bool IsWeaponEquipped();

	/* 현재 착용중인 무기 Get */
	AWeapon* GetEquippedWeapon();

	/* 현재 Aim 하고 있는지*/
	bool IsAiming();

	virtual void OnRep_ReplicatedMovement() override;
	
	void PlayFireMontage(bool bAiming);
	void PlayHitReactMontage();

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	void UpdateHUDHealth();
protected:
	
	virtual void BeginPlay() override;

	
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	/* Server RPC - 무기 장착 버튼 눌렀을 때 (E) */
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	/* 현재 캐릭터가 Overlapping 하고 있는 Weapon */
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<AWeapon> OverlappingWeapon;

protected:

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UInputMappingContext> PlayerCharacterInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UBInputConfig> PlayerCharacterInputConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float ForwardInputValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float RightInputValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UWidgetComponent> OverheadWidget;

	/* CombatComponent */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCombatComponent> Combat;

	/* 발사 Montage */
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	/* 제자리 회전 */
	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	void HideCameraIfCharacterClose();
	
	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;

	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float CalculateSpeed();

	/* Player Health */

	UPROPERTY(EditAnywhere, Category = "PlayerState")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "PlayerState")
	float Health = 100.f;

	UFUNCTION()
	void OnRep_Health();

	TObjectPtr<ABlasterMainController> BlasterPlayerController;

private:

	/* Input */
	void InputMove(const FInputActionValue& InValue);
	void InputLook(const FInputActionValue& InValue);
	void InputEquip(const FInputActionValue& InValue);
	void InputCrouch(const FInputActionValue& InValue);
	void InputAimStart(const FInputActionValue& InValue);
	void InputAimEnd(const FInputActionValue& InValue);
	void InputFirePressed(const FInputActionValue& InValue);
	void InputFireReleased(const FInputActionValue& InValue);
	void AimOffset(float DeltaTime);
	void CalculateAO_Pitch();
	void SimProxiesTurn();
	virtual void Jump() override;
public:

	/* AimOffset을 위한 Yaw, Pitch Getter*/
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }

	/* Turning을 위해 ETurningPlace Getter */
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }

	

};
