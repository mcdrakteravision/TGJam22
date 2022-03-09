// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/TGJ_PlayerController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Net/UnrealNetwork.h"

//=================================================================================================================
ATGJ_PlayerController::ATGJ_PlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

//=================================================================================================================
void ATGJ_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// keep updating the destination every tick while desired
	// TODO Find the optimal way to replicate Cursor's movement, I hate Server RPCs on Tick
	if (bMoveToCursor)
	{
		MoveToMouseCursor();
	}
}

//=================================================================================================================
void ATGJ_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ATGJ_PlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ATGJ_PlayerController::OnSetDestinationReleased);
}

//=================================================================================================================
void ATGJ_PlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}

//=================================================================================================================
void ATGJ_PlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	const APawn* MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			Server_SetLastCursorLocation(DestLocation);
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

//=================================================================================================================
void ATGJ_PlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToCursor = true;
}

//=================================================================================================================
void ATGJ_PlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToCursor = false;
}

//=================================================================================================================
void ATGJ_PlayerController::Server_SetLastCursorLocation_Implementation(const FVector& LastCursorLocation)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, LastCursorLocation);
}
