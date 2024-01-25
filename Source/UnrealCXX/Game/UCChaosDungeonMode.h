// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/UCGameMode.h"
#include "Interface/UCChaosDungeonModeInterface.h"
#include "UCChaosDungeonMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCXX_API AUCChaosDungeonMode : public AUCGameMode, public IUCChaosDungeonModeInterface
{
	GENERATED_BODY()
	
public:
	AUCChaosDungeonMode();

	virtual void OnPlayerScoreChanged(int32 NewPlayerScore) override;
	virtual void OnPlayerDead() override;
	virtual bool IsGameCleared() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 ClearScore;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	int32 CurrentScore;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	uint8 bIsCleared : 1;	
};
