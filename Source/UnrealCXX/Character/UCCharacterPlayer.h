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

public:
	virtual void AttackHitCheck() override;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ZoomIn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ZoomOut;

// Combo Action Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUCComboActionData> ComboActionData;

	bool bClickLeftMouse;

	void InputLeftMouseButtonPressed();

	void InputLeftMouseButtonReleased();

	void ComboActionBegin();

	void SetComboCheckTimer();

	void ComboCheck();

	void RotationToCursor();

	bool bRunCombo = false;

	int32 CurrentCombo = 0;

	FTimerHandle ComboTimerHandle;

	float AttackSpeedRate = 1.0f;

	bool bNewDestinationSet;

	FVector NewDestination;

	void UpdateCircleRing(const FVector Destination);

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

	void Move();

	bool bClickRightMouse;

	void InputRightMouseButtonPressed();

	void InputRightMouseButtonReleased();

	void SetNewDestination(const FVector Destination);

	// Zoom Section
protected:
	static const float MaxTargetArmLength;

	static const float MinTargetArmLength;

	static const float MaxSocketOffset;

	static const float MinSocketOffset;

	float ZoomFactor;

	bool bIsZoomingIn;

	bool bIsZoomingOut;

	void InputMouseWheelUp();

	void InputMouseWheelDown();
};
