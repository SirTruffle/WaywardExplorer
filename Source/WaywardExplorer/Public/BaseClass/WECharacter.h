// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WECharacter.generated.h"


UCLASS()
class WAYWARDEXPLORER_API AWECharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	class UStatlineComponent* Statline;

public:
	// Sets default values for this character's properties
	AWECharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool CanJump() const;
	void HasJumped();

	bool CanSprint() const;
	void SetSprint(const bool& IsSprinting);

	bool CanChannel() const;
	void SetChanneling(const bool& IsChanneling);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
