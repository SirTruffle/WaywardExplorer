
// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WEGameInstance.h"
#include "Game/WESaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include <Serialization/ObjectAndNameAsStringProxyArchive.h>

UWEGameInstance::UWEGameInstance()
{

}

void UWEGameInstance::CreateSaveSlot()
{
	SaveGameObject = Cast<UWESaveGame>(UGameplayStatics::CreateSaveGameObject(UWESaveGame::StaticClass()));
}

void UWEGameInstance::GatherActorData()
{
	for(FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsValid(Actor) || !Actor->Implements<USaveActorInterface>())
		{
			continue;
		}
		ISaveActorInterface* Inter = Cast<ISaveActorInterface>(Actor);
		if (Inter == nullptr)
		{
			continue;
		}
		
		FGuid SAI = Inter->GetActorSaveID_Implementation();
		if(!SAI.IsValid())
		{
			continue;
		}

		FSaveActorData SAD = Inter->GetSaveData_Implementation();
		FMemoryWriter MemWriter(SAD.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);

		for (auto ActorComp : Actor->GetComponents())
		{
			/* Loop through each Actors component and checks if it implements the SaveActorInterface.
			* if it needs to be saved, it serializes the component data into a byte array and adds it to the SaveActorData.
			*/
			if(!ActorComp->Implements<USaveActorInterface>())
			{
				continue;
			}
			ISaveActorInterface* CompInter = Cast<ISaveActorInterface>(ActorComp);
			if(CompInter == nullptr)
			{
				continue;
			}
			FSaveComponentData SCD = CompInter->GetComponentSaveData_Implementation();
			FMemoryWriter CompMemWriter(SCD.ByteData);
			FObjectAndNameAsStringProxyArchive CAr(CompMemWriter, true);
			CAr.ArIsSaveGame = true;
			ActorComp->Serialize(CAr);
			SCD.ComponentClass = ActorComp->GetClass();

			SAD.ComponentData.Add(SCD);
		}

		
		SaveableActorData.Add(SAI, SAD);
	}
}

void UWEGameInstance::LoadGame()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveGameName, 0))
	{
		//TODO: add logging and error message about no save game existing
		return;
	}

	SaveableActorData.Empty();
	SaveGameObject = Cast <UWESaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameName, 0));
	SaveableActorData = SaveGameObject->GetSavedActorData();

	for (TTuple<FGuid, FSaveActorData> SAD : SaveableActorData)
	{
		if (SAD.Value.WasSpawned)
		{
			AActor* SpawnedActor = GetWorld()->SpawnActor(SAD.Value.ActorClass->StaticClass(), &SAD.Value.ActorTransform);
			ISaveActorInterface* Inter = Cast<ISaveActorInterface>(SpawnedActor);
			if (Inter == nullptr)
			{
				continue;
			}
			// Set actor GUID
		}
	}
	for (FActorIterator IT(GetWorld()); IT; ++IT)
	{
		AActor* Actor = *IT;
		if(!IsValid(Actor) || !Actor->Implements<USaveActorInterface>())
		{
			continue;
		}
		ISaveActorInterface* Inter = Cast<ISaveActorInterface>(Actor);
		if (Inter == nullptr)
		{
			continue;
		}
		FGuid SAI = Inter->GetActorSaveID_Implementation();
		if(!SaveableActorData.Find(SAI))
		{
			continue;
		}
		FSaveActorData SAD = SaveableActorData[SAI];
		Actor->SetActorTransform(SAD.ActorTransform);

		FMemoryReader MemReader(SAD.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);

		for (auto ActorComp : Actor->GetComponents())
		{
			if (!ActorComp->Implements<USaveActorInterface>())
			{
				continue;
			}
			ISaveActorInterface* CompInter = Cast<ISaveActorInterface>(ActorComp);
			if (CompInter == nullptr)
			{
				continue;
			}
			for (auto SCD : SAD.ComponentData)
			{
				/************************************************************************
				* NOT SAVE IF ACTOR CONTAINS 2 OF THE SAME COMPONENTS, THAT ARE SAVED!
				* THE 1ST COMPONENT RETURNED BY aTOR->gETcOMPPONENTS() WILL GET ALL DATA
				*
				* CAN BE FIXED WITH GUID ON THE COMPONENTS
				************************************************************************/
				if(SCD.ComponentClass != ActorComp->GetClass())
				{
					continue;
				}
				FMemoryReader CompMemReader(SCD.ByteData);
				FObjectAndNameAsStringProxyArchive CAr(CompMemReader, true);
				CAr.ArIsSaveGame = true;
				ActorComp->Serialize(CAr);
				if(SCD.RawData.IsEmpty())
				{
					break;
				}
				CompInter->SetComponentSaveData_Implementation(SCD);
				break;
			}
		}
	}
}

void UWEGameInstance::AddActorData(const FGuid ActorID, FSaveActorData ActorData)
{
	SaveableActorData.Add(ActorID, ActorData);
}

FSaveActorData UWEGameInstance::GetActorData(const FGuid& ActorID)
{
	return SaveableActorData[ActorID];
}

void UWEGameInstance::DEV_SaveGame()
{
	if (SaveGameObject == nullptr)
	{
		CreateSaveSlot();
	}
	GatherActorData();
	SaveGameObject->SetSavedActorData(SaveableActorData);
	UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameName, 0);
}

void UWEGameInstance::DEV_LoadGame()
{
	LoadGame();
}
