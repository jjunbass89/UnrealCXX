// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UCCharacterBase.h"
#include "InputActionValue.h"
#include "UCCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCXX_API AUCCharacterPlayer : public AUCCharacterBase
{
	GENERATED_BODY()

public:
	AUCCharacterPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	void Move();

	bool bClickRightMouse;	

	void InputRightMouseButtonPressed();
	
	void InputRightMouseButtonReleased();

	void SetNewDestination(const FVector Destination);

	void Attack();

	bool bClickLeftMouse;

	void InputLeftMouseButtonPressed();

	void InputLeftMouseButtonReleased();

// Combo Action Section
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUCComboActionData> ComboActionData;

	void ProcessComboCommand();

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetComboCheckTimer();
	void ComboCheck();

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

// Item Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;	

// Move Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Move, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AUCCircleRing> CircleRingClass;

	UPROPERTY(VisibleAnywhere, Category = Move, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AUCCircleRing> CircleRing;

	bool bNewDestinationSet;

	FVector NewDestination;

	void UpdateCircleRing(const FVector Destination);
};
