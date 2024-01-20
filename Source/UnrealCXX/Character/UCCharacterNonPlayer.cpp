// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UCCharacterNonPlayer.h"


AUCCharacterNonPlayer::AUCCharacterNonPlayer()
{
}

void AUCCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}
