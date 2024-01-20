// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UCCircleRing.generated.h"

UCLASS()
class UNREALCXX_API AUCCircleRing : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUCCircleRing();
	
	// Move Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> CircleRing;;
};
