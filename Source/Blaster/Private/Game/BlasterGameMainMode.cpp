// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BlasterGameMainMode.h"
#include "Character/BlasterCharacter.h"
#include "Controller/BlasterMainController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "PlayerState/BlasterPlayerState.h"

void ABlasterGameMainMode::PlayerEliminated(ABlasterCharacter* ElimCharacater, ABlasterMainController* VictimController, ABlasterMainController* AttackerController)
{
	if (ElimCharacater)
	{
		ElimCharacater->Elim();
	}
}

void ABlasterGameMainMode::RequestRespawn(ABlasterCharacter* ElimCharacter, AController* ElimController)
{
	if (ElimCharacter)
	{
		ElimCharacter->Reset();
		ElimCharacter->Destroy();
	}
	if (ElimController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimController, PlayerStarts[Selection]);
	}
}
