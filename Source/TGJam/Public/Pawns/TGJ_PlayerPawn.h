// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Pawn.h"
#include "TGJ_PlayerPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USceneComponent;
class UAbilitySystemComponent;
class UTGJ_AttributeSet;
class UTGJ_GameplayAbility;
class UGameplayEffect;

UCLASS()
class TGJAM_API ATGJ_PlayerPawn : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATGJ_PlayerPawn();

protected:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USceneComponent* RootSceneComponent;

	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	TWeakObjectPtr<UTGJ_AttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayAbilities")
	TArray<TSubclassOf<UTGJ_GameplayAbility>> StartingAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayAbilities")
	TArray<TSubclassOf<UGameplayEffect>> StartingEffects;

	uint8 bWereAbilitiesGiven : 1;

	uint8 bWereEffectsGiven : 1;

	uint8 bGASInputsBound : 1;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Controller possession delegate */
	virtual void PossessedBy(AController* NewController) override;

	void SetupStartingAbilities();

	void SetupStartingEffects();

	virtual void OnRep_PlayerState() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent.Get(); };

	void SetupGASInputs();
};
