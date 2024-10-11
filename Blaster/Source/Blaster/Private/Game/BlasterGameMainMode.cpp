// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BlasterGameMainMode.h"
#include "Character/BlasterCharacter.h"
#include "Controller/BlasterMainController.h"

void ABlasterGameMainMode::PlayerEliminated(ABlasterCharacter* ElimCharacater, ABlasterMainController* VictimController, ABlasterMainController* AttackerController)
{
	if (ElimCharacater)
	{
		ElimCharacater->Elim();
	}
}
