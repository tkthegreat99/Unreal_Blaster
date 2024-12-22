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

		// 현재 속도 값을 계산 (2D 벡터의 크기)
		CurrentSpeed = Velocity.Size();

		// 캐릭터가 공중에 떠있는지 확인
		bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling();

		// 캐릭터가 가속 중인지 확인 (현재 가속도 벡터의 크기가 0보다 큰지 확인)
		bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

		// 캐릭터가 무기를 장착하고 있는지 확인
		bWeaponEquipped = BlasterCharacter->IsWeaponEquipped();

		// 장착된 무기 정보 저장
		EquippedWeapon = BlasterCharacter->GetEquippedWeapon();

		// 캐릭터가 웅크리고 있는지 확인
		bIsCrouched = BlasterCharacter->bIsCrouched;

		// 캐릭터가 조준 상태인지 확인
		bIsAiming = BlasterCharacter->IsAiming();

		// 캐릭터가 회전 중인지 (In-Place Turn) 상태 저장
		TurningInPlace = BlasterCharacter->GetTurningInPlace();

		bRotateRootBone = BlasterCharacter->ShouldRotateRootBone();

		bEliminated = BlasterCharacter->IsEliminated();
		// 캐릭터의 에임(Aim) 회전 값을 가져옴
		FRotator AimRotation = BlasterCharacter->GetBaseAimRotation();

		// 캐릭터의 이동 방향으로부터 회전 값을 생성
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BlasterCharacter->GetVelocity());

		// 이동 방향과 에임 방향 간의 회전 차이를 계산 (Normalized Delta Rotator)
		FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);

		// DeltaRotation을 점진적으로 보간하여 부드럽게 변화시킴
		DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);

		YawOffset = DeltaRotation.Yaw;

		// 이전 프레임의 캐릭터 회전 값 저장
		CharacterRotationLastFrame = CharacterRotation;
		// 현재 프레임의 캐릭터 회전 값 저장
		CharacterRotation = BlasterCharacter->GetActorRotation();

		// 이전 프레임과 현재 프레임 사이의 회전 차이 계산
		const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);

		// 회전 차이를 기반으로 목표 Lean 값 계산 (Yaw 값 변화량을 DeltaTime으로 나눔)
		const float Target = Delta.Yaw / DeltaTime;

		// Lean 값을 점진적으로 보간하여 부드럽게 변화시킴
		const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);

		Lean = FMath::Clamp(Interp, -90.f, 90.f);

		AO_Yaw = BlasterCharacter->GetAO_Yaw();
		AO_Pitch = BlasterCharacter->GetAO_Pitch();

		

		/*   캐릭터의 왼손을 붙여주어 무기 메시에 따라 항상 알맞은 손을 하도록 유도*/
		if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && BlasterCharacter->GetMesh())
		{
			
			LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);

			// 손의 위치와 회전을 조정하기 위한 임시 변수 선언
			FVector OutPosition;
			FRotator OutRotation;

			BlasterCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);

			LeftHandTransform.SetLocation(OutPosition);
			LeftHandTransform.SetRotation(FQuat(OutRotation));
		}
	}
}