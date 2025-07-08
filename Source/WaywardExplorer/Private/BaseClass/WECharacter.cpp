// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClass/WECharacter.h"

// Sets default values
AWECharacter::AWECharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWECharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

