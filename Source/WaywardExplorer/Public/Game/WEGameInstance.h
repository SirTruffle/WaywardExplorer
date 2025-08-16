// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interface/SaveActorInterface.h"
#include "WEGameInstance.generated.h"


UCLASS()
class WAYWARDEXPLORER_API UWEGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	TMap<FGuid, FSaveActorData> SaveableActorData;
	UPROPERTY()
	class UWESaveGame* SaveGameObject = nullptr;;
	FString SaveGameName = TEXT("DEFAULT");
	FName CurrentlyLoadedLevel = "NONE";

	UWEGameInstance();

	void CreateSaveSlot();
	void GatherActorData();
	void LoadGame();


public:
	UFUNCTION(BlueprintCallable)
	void AddActorData(const FGuid ActorID, FSaveActorData ActorData);
	UFUNCTION(BlueprintCallable)
	FSaveActorData GetActorData(const FGuid& ActorID);

	UFUNCTION(BlueprintCallable)
	void DEV_SaveGame();
	UFUNCTION(BlueprintCallable)
	void DEV_LoadGame();
};
