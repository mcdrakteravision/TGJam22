// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/TGJ_PlayerPawn.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/TGJ_PlayerState.h"
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

	ATGJ_PlayerState* MyPlayerState = GetPlayerState<ATGJ_PlayerState>();
	if(IsValid(MyPlayerState))
	{
		AbilitySystemComponent = MyPlayerState->GetAbilitySystemComponent();
		if(AbilitySystemComponent.IsValid())
		{
			SetupStartingAbilities();
			SetupStartingEffects();

			AbilitySystemComponent->InitAbilityActorInfo(MyPlayerState, this);
		}

		AttributeSet = MyPlayerState->GetAttributeSet();
	}
}

//=================================================================================================================
void ATGJ_PlayerPawn::SetupStartingAbilities()
{
	if(!HasAuthority() || !AbilitySystemComponent.IsValid() || StartingAbilities.Num() <= 0 || bWereAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UTGJ_GameplayAbility>& StartupAbility : StartingAbilities)
	{
		if (IsValid(StartupAbility))
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1.f, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}

	bWereAbilitiesGiven = true;
}

//=================================================================================================================
void ATGJ_PlayerPawn::SetupStartingEffects()
{
	if(!HasAuthority() || !AbilitySystemComponent.IsValid() || StartingEffects.Num() <= 0 || bWereEffectsGiven)
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
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	bWereEffectsGiven = true;
}

//=================================================================================================================
void ATGJ_PlayerPawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ATGJ_PlayerState* MyPlayerState = GetPlayerState<ATGJ_PlayerState>();
	if(IsValid(MyPlayerState) && IsValid(GetController()))
	{
		AbilitySystemComponent = MyPlayerState->GetAbilitySystemComponent();
		if(AbilitySystemComponent.IsValid())
		{
			AbilitySystemComponent->InitAbilityActorInfo(MyPlayerState, this);
			SetupGASInputs(GetController()->InputComponent);
		}

		AttributeSet = MyPlayerState->GetAttributeSet();
	}
}

//=================================================================================================================
// Called every frame
void ATGJ_PlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//=================================================================================================================
void ATGJ_PlayerPawn::SetupGASInputs(UInputComponent* InstigatorInputComponent)
{
	if(!AbilitySystemComponent.IsValid() || !IsValid(InstigatorInputComponent) || bGASInputsBound)
	{
		return;
	}

	AbilitySystemComponent->BindAbilityActivationToInputComponent(InstigatorInputComponent, FGameplayAbilityInputBinds(FString("Confirm"), FString("Cancel"), FString("ETGJ_AbilityInput"), static_cast<int32>(ETGJ_AbilityInput::Confirm), static_cast<int32>(ETGJ_AbilityInput::Cancel)));
}

