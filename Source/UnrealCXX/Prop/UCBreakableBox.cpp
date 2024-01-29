// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/UCBreakableBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/UCCollision.h"
#include "UI/UCHpBarWidget.h"
#include "CharacterStat/UCCharacterStatComponent.h"
#include "UI/UCWidgetComponent.h"

// Sets default values
AUCBreakableBox::AUCBreakableBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_UCCAPSULE);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/UnrealCXX/Prop/SM_Env_Breakables_Box.SM_Env_Breakables_Box'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	// Stat Component 
	Stat = CreateDefaultSubobject<UUCCharacterStatComponent>(TEXT("Stat"));

	// Widget Component 
	HpBar = CreateDefaultSubobject<UUCWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(Mesh);
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UnrealCXX/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(80.0f, 8.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AUCBreakableBox::SetupCharacterWidget(UUCUserWidget* InUserWidget)
{
	UUCHpBarWidget* HpBarWidget = Cast<UUCHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetMaxHp());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UUCHpBarWidget::UpdateHpBar);
	}
}

void AUCBreakableBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AUCBreakableBox::SetDead);
}

float AUCBreakableBox::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}

void AUCBreakableBox::SetDead()
{
	SetActorEnableCollision(false);
	HpBar->SetHiddenInGame(true);
	Destroy();
}

