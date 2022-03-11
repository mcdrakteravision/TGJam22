// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTGJam, Log, All);

UENUM(BlueprintType)
enum class ETGJ_AbilityInput : uint8
{
	None					UMETA(DisplayName = "None"),
	Dash					UMETA(DisplayName = "Dash"),
	Confirm					UMETA(DisplayName = "Confirm"),
	Cancel					UMETA(DisplayName = "Cancel"),
	Ability1				UMETA(DisplayName = "Ability1"),
	Ability2				UMETA(DisplayName = "Ability2")
};