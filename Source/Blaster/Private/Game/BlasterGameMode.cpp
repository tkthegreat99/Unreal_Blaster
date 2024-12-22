// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BlasterGameMode.h"
#include "Controller/BlasterController.h"
#include "Character/BlasterCharacter.h"

ABlasterGameMode::ABlasterGameMode()
{
	PlayerControllerClass = ABlasterController::StaticClass();
	DefaultPawnClass = ABlasterCharacter::StaticClass();
}
