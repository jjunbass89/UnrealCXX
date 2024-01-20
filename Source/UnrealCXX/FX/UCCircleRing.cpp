// Fill out your copyright notice in the Description page of Project Settings.


#include "FX/UCCircleRing.h"

// Sets default values
AUCCircleRing::AUCCircleRing()
{
	CircleRing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CircleRing"));
	RootComponent = CircleRing;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CircleRingRef(TEXT("/Script/Engine.StaticMesh'/Game/UnrealCXX/FX/CircleRing/SM_CircleRing.SM_CircleRing'"));
	if (CircleRingRef.Object)
	{
		CircleRing->SetStaticMesh(CircleRingRef.Object);
	}
	CircleRing->SetCollisionProfileName(TEXT("NoCollision"));
}
