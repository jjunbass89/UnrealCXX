// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UCChaosDungeonController.h"
#include "Character/UCCharacterPlayer.h"
#include "Interface/UCChaosDungeonGameInterface.h"

void AUCChaosDungeonController::K2_Revival()
{
	AUCCharacterPlayer* player = Cast<AUCCharacterPlayer>(GetPawn());
	if (player)
	{
		player->Revival();
	}
}

int32 AUCChaosDungeonController::K2_GetStage()
{
	IUCChaosDungeonGameInterface* ChaosDungeonGameInterface = Cast<IUCChaosDungeonGameInterface>(GetGameInstance());
	if (ChaosDungeonGameInterface)
	{
		return ChaosDungeonGameInterface->GetChaosDungeonStage();
	}

	return int32();
}

void AUCChaosDungeonController::GameScoreChanged(int32 NewScore)
{
	K2_OnScoreChanged(NewScore);
}

void AUCChaosDungeonController::GameClear()
{
	K2_OnGameClear();
}

void AUCChaosDungeonController::GameOver()
{
	K2_OnGameOver();
}

void AUCChaosDungeonController::EnterPortal()
{
	K2_OnEnterPortal();
}