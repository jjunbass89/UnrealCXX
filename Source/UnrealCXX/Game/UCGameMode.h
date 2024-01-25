// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UCGameMode.generated.h"

UENUM(BlueprintType)
enum class EGameMode : uint8
{
	DEFAULT = 0,
	CHAOS_DUNGEON,
	GUARDIAN_RAID,
	TOWER,
};

/**
 * 
 */
UCLASS()
class UNREALCXX_API AUCGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUCGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
	EGameMode CurrentGameMode;
};
