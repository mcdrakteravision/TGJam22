// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TGJ_ProxyCharacter.generated.h"

UCLASS()
class TGJAM_API ATGJ_ProxyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATGJ_ProxyCharacter();

	void UpdateControllerLocationToGo(const FVector& LocationToGo) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
