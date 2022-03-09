// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/TGJ_PlayerPawn.h"

// Sets default values
ATGJ_PlayerPawn::ATGJ_PlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATGJ_PlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATGJ_PlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATGJ_PlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

