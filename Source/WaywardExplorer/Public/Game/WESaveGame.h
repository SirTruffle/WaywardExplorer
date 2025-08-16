// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Interface/SaveActorInterface.h"
#include "WESaveGame.generated.h"


UCLASS()
class WAYWARDEXPLORER_API UWESaveGame : public USaveGame
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TMap<FGuid, FSaveActorData> SaveableActorData;
	UPROPERTY()
	FName CurrentlyLoadedLevel = "NONE";
public:
	void SetSavedActorData(TMap<FGuid, FSaveActorData> Data);
	TMap<FGuid, FSaveActorData> GetSavedActorData();
	void SetCurrentLevel(const FName Level);
	FName GetCurrentLevel();
};
