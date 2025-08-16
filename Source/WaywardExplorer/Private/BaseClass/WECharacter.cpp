// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClass/WECharacter.h"
#include "Components/StatlineComponent.h"

// Sets default values
AWECharacter::AWECharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Statline = CreateDefaultSubobject<UStatlineComponent>(TEXT("Statline"));
	Statline->SetMovementCompReference(GetCharacterMovement());

	SaveActorID = FGuid::NewGuid();

}

// Called when the game starts or when spawned
void AWECharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!SaveActorID.IsValid())
	{
		SaveActorID = FGuid::NewGuid();
	}
}

bool AWECharacter::CanJump() const
{
	return Statline->Canjump();
}

void AWECharacter::HasJumped()
{
	Statline->HasJumped();
	ACharacter::Jump();
}

bool AWECharacter::CanSprint() const
{
	return Statline->CanSprint();
}

void AWECharacter::SetSprint(const bool& IsSprinting)
{
	Statline->SetSprinting(IsSprinting);
}

bool AWECharacter::CanChannel() const
{
	return Statline->CanChannel();
}

void AWECharacter::SetChanneling(const bool& IsChanneling)
{
	Statline->SetChanneling(IsChanneling);
}

void AWECharacter::SetSneaking(const bool& IsSneaking)
{
	Statline->SetSneaking(IsSneaking);
}

// Called every frame
void AWECharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWECharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FGuid AWECharacter::GetActorSaveID_Implementation()
{
	return SaveActorID;
}

FSaveActorData AWECharacter::GetSaveData_Implementation()
{
	FSaveActorData Ret;
	Ret.ActorClass = this->GetClass();
	Ret.ActorTransform = this->GetTransform();
	Ret.WasSpawned = this->WasSpawned;

	return Ret;
}

