// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TGJ_ProxyCharacter.h"

#include "Pawns/TGJ_PlayerPawn.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/TGJ_AIProxyController.h"

//=================================================================================================================
// Sets default values
ATGJ_ProxyCharacter::ATGJ_ProxyCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

//=================================================================================================================
void ATGJ_ProxyCharacter::UpdateControllerLocationToGo(const FVector& LocationToGo) const
{
	ATGJ_AIProxyController* AIProxyController = Cast<ATGJ_AIProxyController>(GetController());
	if(!IsValid(AIProxyController))
	{
		return;
	}

	AIProxyController->SetLocationToMove(LocationToGo);
}

//=================================================================================================================
// Called every frame
void ATGJ_ProxyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
