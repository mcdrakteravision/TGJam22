// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TGJam/TGJam.h"
#include "TGJ_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class TGJAM_API UTGJ_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UTGJ_GameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	ETGJ_AbilityInput AbilityID;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Properties")
	ETGJ_AbilityInput AbilityInputID;
};
