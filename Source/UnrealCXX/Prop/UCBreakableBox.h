// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/UCCharacterWidgetInterface.h"
#include "UCBreakableBox.generated.h"

UCLASS()
class UNREALCXX_API AUCBreakableBox : public AActor, public IUCCharacterWidgetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUCBreakableBox();

	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> HpBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUCCharacterStatComponent> Stat;

	virtual void SetupCharacterWidget(class UUCUserWidget* InUserWidget) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetDead();
};
