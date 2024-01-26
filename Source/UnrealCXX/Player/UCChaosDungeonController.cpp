// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UCChaosDungeonController.h"
#include "Character/UCCharacterPlayer.h"

void AUCChaosDungeonController::K2_Revival()
{
	AUCCharacterPlayer* player = Cast<AUCCharacterPlayer>(GetPawn());
	if (player)
	{
		player->Revival();
	}
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