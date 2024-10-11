// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BlasterGameMainMode.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterGameMainMode : public AGameMode
{
	GENERATED_BODY()

public:

	virtual void PlayerEliminated(class ABlasterCharacter* ElimCharacater, class ABlasterMainController* VictimController, ABlasterMainController* AttackerController);
	virtual void RequestRespawn(class ABlasterCharacter* ElimCharacter, AController* ElimController);
};
