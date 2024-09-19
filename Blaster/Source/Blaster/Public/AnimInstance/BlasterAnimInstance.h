// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BlasterAnimInstance.generated.h"

class ABlasterCharacter;
/**
 * 
 */
UCLASS()
class BLASTER_API UBlasterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	
	UPROPERTY(BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	TObjectPtr<ABlasterCharacter> BlasterCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess))
	float CurrentSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess))
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess))
	uint8 bIsInAir : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess))
	uint8 bIsAccelerating : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess))
	uint8 bWeaponEquipped : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess))
	uint8 bIsCrouched : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess))
	uint8 bIsAiming : 1;
};
