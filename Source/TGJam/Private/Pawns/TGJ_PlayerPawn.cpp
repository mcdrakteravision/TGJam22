// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/TGJ_PlayerPawn.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayAbilitySystem/TGJ_AttributeSet.h"
#include "GameplayAbilitySystem/TGJ_GameplayAbility.h"

//=================================================================================================================
// Sets default values
ATGJ_PlayerPawn::ATGJ_PlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->ReplicationMode = EGameplayEffectReplicationMode::Full;
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UTGJ_AttributeSet>(TEXT("AttributSet"));

	bWereAbilitiesGiven = false;
	bWereEffectsGiven = false;
	bGASInputsBound = false;
}

//=================================================================================================================
// Called when the game starts or when spawned
void ATGJ_PlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

//=================================================================================================================
void ATGJ_PlayerPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(IsValid(AbilitySystemComponent))
	{
		SetupStartingAbilities();
		SetupStartingEffects();

		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

//=================================================================================================================
void ATGJ_PlayerPawn::SetupStartingAbilities()
{
	if(!HasAuthority() || !IsValid(AbilitySystemComponent) || StartingAbilities.Num() <= 0 || bWereAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UTGJ_GameplayAbility>& StartupAbility : StartingAbilities)
	{
		if (IsValid(StartupAbility))
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(StartupAbility, 1.f, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}

	bWereAbilitiesGiven = true;
}

//=================================================================================================================
void ATGJ_PlayerPawn::SetupStartingEffects()
{
	if(!HasAuthority() || !IsValid(AbilitySystemComponent) || StartingEffects.Num() <= 0 || bWereEffectsGiven)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (const TSubclassOf<UGameplayEffect> CurrentGameplayEffect : StartingEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(CurrentGameplayEffect, 1.f, EffectContext);
		if (NewHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}

	bWereEffectsGiven = true;
}

//=================================================================================================================
void ATGJ_PlayerPawn::SetupGASInputs()
{
	if(!IsValid(AbilitySystemComponent) || !IsValid(InputComponent) || bGASInputsBound)
	{
		return;
	}

	AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("Confirm"), FString("Cancel"), FString("ETGJ_AbilityInputID"), static_cast<int32>(ETGJ_AbilityInput::Confirm), static_cast<int32>(ETGJ_AbilityInput::Cancel)));
}

//=================================================================================================================
// Called every frame
void ATGJ_PlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//=================================================================================================================
// Called to bind functionality to input
void ATGJ_PlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupGASInputs();
}

