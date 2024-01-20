// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UCCharacterBase.h"
#include "UCCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCXX_API AUCCharacterNonPlayer : public AUCCharacterBase
{
	GENERATED_BODY()
	
public:
	AUCCharacterNonPlayer();

protected:
	void SetDead() override;	
};
