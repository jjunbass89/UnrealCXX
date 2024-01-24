// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/UCGameInterface.h"
#include "UCGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCXX_API AUCGameMode : public AGameModeBase, public IUCGameInterface
{
	GENERATED_BODY()

public:
	AUCGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	EGameMode CurrentGameMode;
};
