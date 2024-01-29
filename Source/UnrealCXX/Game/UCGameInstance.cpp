// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UCGameInstance.h"

void UUCGameInstance::UpdateChaosDungeonScore(int32 NewScore)
{
	ChaosDungeonScore = NewScore;
}

int32 UUCGameInstance::GetChaosDungeonScore()
{
	return ChaosDungeonScore;
}
