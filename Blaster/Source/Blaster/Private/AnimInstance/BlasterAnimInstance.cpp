// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/BlasterAnimInstance.h"
#include "Character/BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
}

void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (IsValid(BlasterCharacter) == true)
	{
		Velocity = BlasterCharacter->GetVelocity();
		Velocity.Z = 0.f;
		CurrentSpeed = Velocity.Size();
		bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling();
		bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
		bWeaponEquipped = BlasterCharacter->IsWeaponEquipped();
	}

}
