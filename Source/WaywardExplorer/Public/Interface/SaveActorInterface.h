// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveActorInterface.generated.h"

USTRUCT(BlueprintType)
struct FSaveComponentData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Data")
	TArray<uint8> ByteData; // Serialized data of the component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Data")
	UClass* ComponentClass; // Class of the component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Data")
	TArray<FString> RawData; // Raw data for the component, if needed

};
USTRUCT(BlueprintType)
struct FSaveActorData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Data")
	FTransform ActorTransform; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Data")
	TArray<uint8> ByteData; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Data")
	TArray<FSaveComponentData> ComponentData; // Serialized data of the components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Data")
	bool WasSpawned = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Data")
	UClass* ActorClass;
};


UINTERFACE(MinimalAPI)
class USaveActorInterface : public UInterface
{
	GENERATED_BODY()
};


class WAYWARDEXPLORER_API ISaveActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	FGuid GetActorSaveID(); // Blueprint version of the call
	virtual FGuid GetActorSaveID_Implementation(); //cpp version of the call

	UFUNCTION(BlueprintNativeEvent)
	FGuid GetActorSaveData(); // Blueprint version of the call
	virtual FSaveActorData GetSaveData_Implementation(); //cpp version of the call


	UFUNCTION(BlueprintNativeEvent)
	FSaveComponentData GetComponentSaveData();
	virtual FSaveComponentData GetComponentSaveData_Implementation(); //cpp version of the call

	UFUNCTION(BlueprintNativeEvent)
	void SetComponentSaveData(FSaveComponentData Data);
	virtual void SetComponentSaveData_Implementation(FSaveComponentData Data); //cpp version of the call

};
