// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/TGJ_PlayerState.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySystem/TGJ_AttributeSet.h"

//=================================================================================================================
ATGJ_PlayerState::ATGJ_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->ReplicationMode = EGameplayEffectReplicationMode::Full;
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UTGJ_AttributeSet>(TEXT("AttributSet"));
}
