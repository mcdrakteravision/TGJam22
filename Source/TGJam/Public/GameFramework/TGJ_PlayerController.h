// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TGJ_PlayerController.generated.h"

class ATGJ_ProxyCharacter;
class ATGJ_AIProxyController;

UCLASS()
class TGJAM_API ATGJ_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATGJ_PlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint8 bMoveToCursor : 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	TSubclassOf<ATGJ_ProxyCharacter> ProxyCharacterSubclass;

	UPROPERTY()
	ATGJ_ProxyCharacter* ProxyCharacterReference;

	UPROPERTY()
	ATGJ_AIProxyController* ProxyControllerReference;

	FTimerHandle MouseMovementTimerHandle;

	// Begin PlayerController interface
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void SetupProxyCharacterSpawning();

	UFUNCTION()
	void OnMouseMovedWhilePressed();

	UFUNCTION(Server, Reliable)
	void Server_SetLastCursorLocation(const FVector& LastCursorLocation);
};
