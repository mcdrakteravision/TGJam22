// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TGJ_PlayerController.generated.h"

UCLASS()
class TGJAM_API ATGJ_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATGJ_PlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	UPROPERTY(BlueprintReadOnly)
	uint8 bMoveToCursor : 1;

	// Begin PlayerController interface
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	UFUNCTION(BlueprintCallable)
	void MoveToMouseCursor();
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	UFUNCTION(Server, Unreliable)
	void Server_SetLastCursorLocation(const FVector& LastCursorLocation);
};
