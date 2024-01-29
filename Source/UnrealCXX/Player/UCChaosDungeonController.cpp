// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UCChaosDungeonController.h"
#include "Character/UCCharacterPlayer.h"
#include "Interface/UCChaosDungeonGameInterface.h"
#include "Kismet/GameplayStatics.h"

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
	IUCChaosDungeonGameInterface* ChaosDungeonGameInterface = Cast<IUCChaosDungeonGameInterface>(GetGameInstance());
	if (ChaosDungeonGameInterface)
	{
		int32 stage = ChaosDungeonGameInterface->GetChaosDungeonStage();
		switch (stage)
		{
		case 0:
			UGameplayStatics::OpenLevel(this, "ChaosDungeon1");
			break;

		case 1:
			UGameplayStatics::OpenLevel(this, "ChaosDungeon2");
			break;
		}
	}
}