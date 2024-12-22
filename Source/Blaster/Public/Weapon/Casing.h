// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Casing.generated.h"

class UStaticMeshComponent;
class USoundCue;

UCLASS()
class BLASTER_API ACasing : public AActor
{
	GENERATED_BODY()
	
public:	
	ACasing();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	/* ź�� �޽� */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CasingMesh;

	/* ź�� ���� ���� */
	UPROPERTY(EditAnywhere)
	float ShellEjectionImpulse;

	/* ź�� �Ҹ� */
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> ShellSound;

};
