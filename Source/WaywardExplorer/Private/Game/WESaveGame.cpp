// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WESaveGame.h"

void UWESaveGame::SetSavedActorData(TMap<FGuid, FSaveActorData> Data)
{
	SaveableActorData = Data;
}

TMap<FGuid, FSaveActorData> UWESaveGame::GetSavedActorData()
{
	return SaveableActorData;
}

void UWESaveGame::SetCurrentLevel(const FName Level)
{
	CurrentlyLoadedLevel = Level;
}

FName UWESaveGame::GetCurrentLevel()
{
	return CurrentlyLoadedLevel;
}
