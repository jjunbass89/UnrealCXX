// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interface/UCChaosDungeonGameInterface.h"
#include "UCGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCXX_API UUCGameInstance : public UGameInstance, public IUCChaosDungeonGameInterface
{
	GENERATED_BODY()

public:
	virtual void UpdateChaosDungeonScore(int32 NewScore) override;

	virtual int32 GetChaosDungeonScore() override;

	virtual void InitializeChaosDungeonStage() override;

	virtual void IncreaseChaosDungeonStage() override;

	virtual int32 GetChaosDungeonStage() override;

	int32 ChaosDungeonScore = 0;
	
	int32 ChaosDungeonStage = -1;
};
