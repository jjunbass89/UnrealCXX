// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UCCharacterBase.h"

// Sets default values
AUCCharacterBase::AUCCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUCCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUCCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUCCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

