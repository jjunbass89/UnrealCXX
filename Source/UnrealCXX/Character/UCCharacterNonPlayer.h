// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UCCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "UCCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS(config=UnrealCXX)
class UNREALCXX_API AUCCharacterNonPlayer : public AUCCharacterBase
{
	GENERATED_BODY()
	
public:
	AUCCharacterNonPlayer();

protected:
	virtual void PostInitializeComponents() override;

protected:
	void SetDead() override;	
	void NPCMeshLoadCompleted();

	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;
	
	TSharedPtr<FStreamableHandle> NPCMeshHandle;
};
