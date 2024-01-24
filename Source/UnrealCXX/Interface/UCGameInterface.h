// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UCGameInterface.generated.h"

UENUM(BlueprintType)
enum class EGameMode : uint8
{
	DEFAULT = 0,
	CHAOS_DUNGEON,
	GUARDIAN_RAID,
	TOWER,
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUCGameInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALCXX_API IUCGameInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//virtual void OnGameModeChanged(EGameMode NewGameMode) = 0;
};
