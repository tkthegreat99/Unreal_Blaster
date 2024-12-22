// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/BlasterAnimInstance.h"
#include "Character/BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Weapon.h"

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

		// ���� �ӵ� ���� ��� (2D ������ ũ��)
		CurrentSpeed = Velocity.Size();

		// ĳ���Ͱ� ���߿� ���ִ��� Ȯ��
		bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling();

		// ĳ���Ͱ� ���� ������ Ȯ�� (���� ���ӵ� ������ ũ�Ⱑ 0���� ū�� Ȯ��)
		bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

		// ĳ���Ͱ� ���⸦ �����ϰ� �ִ��� Ȯ��
		bWeaponEquipped = BlasterCharacter->IsWeaponEquipped();

		// ������ ���� ���� ����
		EquippedWeapon = BlasterCharacter->GetEquippedWeapon();

		// ĳ���Ͱ� ��ũ���� �ִ��� Ȯ��
		bIsCrouched = BlasterCharacter->bIsCrouched;

		// ĳ���Ͱ� ���� �������� Ȯ��
		bIsAiming = BlasterCharacter->IsAiming();

		// ĳ���Ͱ� ȸ�� ������ (In-Place Turn) ���� ����
		TurningInPlace = BlasterCharacter->GetTurningInPlace();

		bRotateRootBone = BlasterCharacter->ShouldRotateRootBone();

		bEliminated = BlasterCharacter->IsEliminated();
		// ĳ������ ����(Aim) ȸ�� ���� ������
		FRotator AimRotation = BlasterCharacter->GetBaseAimRotation();

		// ĳ������ �̵� �������κ��� ȸ�� ���� ����
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BlasterCharacter->GetVelocity());

		// �̵� ����� ���� ���� ���� ȸ�� ���̸� ��� (Normalized Delta Rotator)
		FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);

		// DeltaRotation�� ���������� �����Ͽ� �ε巴�� ��ȭ��Ŵ
		DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);

		YawOffset = DeltaRotation.Yaw;

		// ���� �������� ĳ���� ȸ�� �� ����
		CharacterRotationLastFrame = CharacterRotation;
		// ���� �������� ĳ���� ȸ�� �� ����
		CharacterRotation = BlasterCharacter->GetActorRotation();

		// ���� �����Ӱ� ���� ������ ������ ȸ�� ���� ���
		const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);

		// ȸ�� ���̸� ������� ��ǥ Lean �� ��� (Yaw �� ��ȭ���� DeltaTime���� ����)
		const float Target = Delta.Yaw / DeltaTime;

		// Lean ���� ���������� �����Ͽ� �ε巴�� ��ȭ��Ŵ
		const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);

		Lean = FMath::Clamp(Interp, -90.f, 90.f);

		AO_Yaw = BlasterCharacter->GetAO_Yaw();
		AO_Pitch = BlasterCharacter->GetAO_Pitch();

		

		/*   ĳ������ �޼��� �ٿ��־� ���� �޽ÿ� ���� �׻� �˸��� ���� �ϵ��� ����*/
		if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && BlasterCharacter->GetMesh())
		{
			
			LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);

			// ���� ��ġ�� ȸ���� �����ϱ� ���� �ӽ� ���� ����
			FVector OutPosition;
			FRotator OutRotation;

			BlasterCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);

			LeftHandTransform.SetLocation(OutPosition);
			LeftHandTransform.SetRotation(FQuat(OutRotation));
		}
	}
}