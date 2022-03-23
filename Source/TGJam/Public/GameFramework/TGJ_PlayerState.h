// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "TGJ_PlayerState.generated.h"

class UAbilitySystemComponent;
class UTGJ_AttributeSet;

/**
 * 
 */
UCLASS()
class TGJAM_API ATGJ_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	ATGJ_PlayerState();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UTGJ_AttributeSet* AttributeSet;

public:
	/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };

	UTGJ_AttributeSet* GetAttributeSet() const { return AttributeSet; };
};
