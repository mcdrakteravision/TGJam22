// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/TGJ_AIProxyController.h"

//=================================================================================================================
void ATGJ_AIProxyController::SetLocationToMove(const FVector& LocationToMove)
{
	MoveToLocation(LocationToMove);
}
