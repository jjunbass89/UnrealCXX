// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UCChaosDungeonController.h"


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