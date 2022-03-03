// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TGJ_Character.h"

// Sets default values
ATGJ_Character::ATGJ_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATGJ_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATGJ_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATGJ_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

