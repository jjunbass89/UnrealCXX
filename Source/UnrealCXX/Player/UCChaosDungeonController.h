// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/UCPlayerController.h"
#include "UCChaosDungeonController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCXX_API AUCChaosDungeonController : public AUCPlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnScoreChangedCpp"))
	void K2_OnScoreChanged(int32 NewScore);
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameClearCpp"))
	void K2_OnGameClear();
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();

	UFUNCTION(BlueprintCallable, Category = Game, Meta = (DisplayName = "RevivalCpp"))
	void K2_Revival();

	UFUNCTION(BlueprintCallable, Category = Game, Meta = (DisplayName = "GetStageCpp"))
	int32 K2_GetStage();

	void GameScoreChanged(int32 NewScore);
	void GameClear();
	void GameOver();	

	// Interaction section
public:
	UFUNCTION(BlueprintImplementableEvent, Category = Portal, Meta = (DisplayName = "OnEnterPortalCpp"))
	void K2_OnEnterPortal();
	
	void EnterPortal();
};
