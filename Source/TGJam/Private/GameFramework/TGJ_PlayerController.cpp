// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/TGJ_PlayerController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/TGJ_ProxyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

//=================================================================================================================
ATGJ_PlayerController::ATGJ_PlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	MouseMovementInterpolationSpeed = 10.f;
}

//=================================================================================================================
void ATGJ_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if(HasAuthority())
	{
		SetupProxyCharacterSpawning();
	}
}

//=================================================================================================================
void ATGJ_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateLocationAccordingToProxy(DeltaTime);
}

//=================================================================================================================
void ATGJ_PlayerController::GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATGJ_PlayerController, ProxyCharacterReference);
}

//=================================================================================================================
void ATGJ_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ATGJ_PlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ATGJ_PlayerController::OnSetDestinationReleased);
}

//=================================================================================================================
void ATGJ_PlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToCursor = true;

	if(!MouseMovementTimerHandle.IsValid())
	{
		GetWorldTimerManager().SetTimer(MouseMovementTimerHandle, this, &ATGJ_PlayerController::OnMouseMovedWhilePressed, 0.01f, true);
	}
}

//=================================================================================================================
void ATGJ_PlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToCursor = false;

	if(MouseMovementTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(MouseMovementTimerHandle);
	}
}

//=================================================================================================================
void ATGJ_PlayerController::SetupProxyCharacterSpawning()
{
	if(!IsValid(ProxyCharacterSubclass) || !IsValid(GetPawn()))
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = GetPawn();
	ProxyCharacterReference = GetWorld()->SpawnActor<ATGJ_ProxyCharacter>(ProxyCharacterSubclass, GetPawn()->GetActorLocation(), GetPawn()->GetActorRotation(), SpawnParameters);
}

//=================================================================================================================
void ATGJ_PlayerController::OnMouseMovedWhilePressed()
{
	if(!IsValid(GetPawn()))
	{
		return;
	}

	FHitResult Hit;
	if (!GetHitResultUnderCursor(ECC_Visibility, false, Hit))
	{
		return;
	}

	float const Distance = FVector::Dist(Hit.Location, GetPawn()->GetActorLocation());
	// We need to issue move command only if far enough in order for walk animation to play correctly
	if ((Distance > 120.0f))
	{
		Server_SetLastCursorLocation(Hit.Location);
	}
}

//=================================================================================================================
void ATGJ_PlayerController::UpdateLocationAccordingToProxy(const float DeltaTime)
{
	if(!IsValid(ProxyCharacterReference) || !IsValid(GetPawn()))
	{
		return;
	}

	const FVector InterpolatedLocation = UKismetMathLibrary::VInterpTo(GetPawn()->GetActorLocation(), ProxyCharacterReference->GetActorLocation(), DeltaTime, MouseMovementInterpolationSpeed);
	GetPawn()->SetActorLocation(InterpolatedLocation);
}

//=================================================================================================================
void ATGJ_PlayerController::Server_SetLastCursorLocation_Implementation(const FVector& LastCursorLocation)
{
	if(!IsValid(ProxyCharacterReference))
	{
		return;
	}

	ProxyCharacterReference->UpdateControllerLocationToGo(LastCursorLocation);
}
