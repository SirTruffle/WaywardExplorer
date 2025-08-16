// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatlineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WEUtils.h"


UStatlineComponent::UStatlineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStatlineComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningCharMovementComp->MaxWalkSpeed = WalkSpeed;
}

void UStatlineComponent::TickStats(const float& DeltaTime)
{
	Health.TickStat(DeltaTime);
	TickStamina(DeltaTime);
	TickMana(DeltaTime);
	Favor.TickStat(DeltaTime);
}

void UStatlineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TickType != ELevelTick::LEVELTICK_PauseTick)
	{
		TickStats(DeltaTime);
	}
}

void UStatlineComponent::SetMovementCompReference(UCharacterMovementComponent* Comp)
{
	OwningCharMovementComp = Comp;
}

//------------------------------------------------------------------------------------------------------------------------------------
#pragma region sprinting
void UStatlineComponent::TickStamina(const float& DeltaTime)
{
	if (CurrentStaminaExhaustion > 0.0)
	{
		CurrentStaminaExhaustion -= DeltaTime;
		return;
	}

	if (bIsSprinting && IsValidSprinting())
	{
		Stamina.TickStat(0 - (DeltaTime * SprintCostMultiplier));
		if (Stamina.GetCurrent() <= 0.0)
		{
			SetSprinting(false);
			CurrentStaminaExhaustion = SecTriggerStamineExhaustion;
		}
		return;
	}
	
	Stamina.TickStat(DeltaTime);
}

bool UStatlineComponent::IsValidSprinting() const
{
	return OwningCharMovementComp->Velocity.Length() > WalkSpeed && !OwningCharMovementComp->IsFalling();
}

bool UStatlineComponent::CanSprint() const
{
	return Stamina.GetCurrent() > 0.0;
}

void UStatlineComponent::SetSprinting(const bool& IsSprinting)
{
	bIsSprinting = IsSprinting;
	if (bIsSneaking && bIsSprinting)
	{
		return; // Prevent sprinting while sneaking
	}
	bIsSneaking = false; // Ensure sneaking is disabled when sprinting
	OwningCharMovementComp->MaxWalkSpeed = IsSprinting ? SprintSpeed : WalkSpeed;
}

void UStatlineComponent::SetSneaking(const bool& IsSneaking)
{
	bIsSneaking = IsSneaking;
	if (bIsSprinting && bIsSneaking)
	{
		return; // Prevent sneaking while sprinting
	}
	bIsSprinting = false; // Ensure sprinting is disabled when sneaking
	OwningCharMovementComp->MaxWalkSpeed = IsSneaking ? SneakSpeed : WalkSpeed;
}
#pragma endregion
//------------------------------------------------------------------------------------------------------------------------------------
#pragma region jumping
bool UStatlineComponent::Canjump() const
{
	return Stamina.GetCurrent() >= JumpCost;
}

void UStatlineComponent::HasJumped()
{
	Stamina.Adjust(0 - JumpCost);
}
#pragma endregion
//------------------------------------------------------------------------------------------------------------------------------------
#pragma region channeling
void UStatlineComponent::TickMana(const float& DeltaTime)
{
	if (bIsChanneling && IsValidChanneling())
	{
		float ManaCost = DeltaTime * ManaCostMultiplier;
		Mana.TickStat(-ManaCost); // Drain mana
		ManaUsedThisChannel += ManaCost;

		if (Mana.GetCurrent() <= 0.0f)
		{
			SetChanneling(false);
			FinalizeChannelStrength(); // Finish up
		}
		return;
	}
	Mana.TickStat(DeltaTime);
}

void UStatlineComponent::FinalizeChannelStrength()
{
	ChannelStrength = ManaUsedThisChannel * ChannelMultiplier;
	ManaUsedThisChannel = 0.0f; // Reset for next time
}

bool UStatlineComponent::IsValidChanneling() const
{
	if (Mana.GetCurrent() > 0.0)
	{
		return true;
	}
	return false;
}

bool UStatlineComponent::CanChannel() const
{
	return Mana.GetCurrent() > 0.0;
}

void UStatlineComponent::SetChanneling(const bool& IsChanneling)
{
	bIsChanneling = IsChanneling;
}

#pragma endregion
//------------------------------------------------------------------------------------------------------------------------------------

FSaveComponentData UStatlineComponent::GetComponentSaveData_Implementation()
{
	FSaveComponentData Ret;
	Ret.ComponentClass = this->GetClass();		
	Ret.RawData.Add(Health.GetSaveString());	// Index 0
	Ret.RawData.Add(Stamina.GetSaveString());	// Index 1
	Ret.RawData.Add(Mana.GetSaveString());		// Index 2
	// Any additional rwadata add needs to be included in the SetComponentSaveData_Implementation function.

	return Ret;
}
void UStatlineComponent::SetComponentSaveData_Implementation(FSaveComponentData Data)
{
	TArray<FString> Parts;
	for (int i = 0; i < Data.RawData.Num(); i++)
	{
		Parts.Empty();
		Parts = ChopString(Data.RawData[i], '|');
		switch (i)
		{
		case 0:
			Health.UpdateFromSaveString(Parts);
			break;
		case 1:
			Stamina.UpdateFromSaveString(Parts);
			break;
		case 2:
			Mana.UpdateFromSaveString(Parts);
			break;
		default:
			// Log error
			break;
		}
	}
}

float UStatlineComponent::GetStatPercentile(const ECorestat Stat) const
{
	switch (Stat)
	{
	case ECorestat::CS_Health:
		return Health.Percentile();
	case ECorestat::CS_Stamina:
		return Stamina.Percentile();
	case ECorestat::CS_Mana:
		return Mana.Percentile();
	case ECorestat::CS_Favor:
		return Favor.Percentile();
	default:
		//TODO: Log invalid stat
		break;
	}
	return -1;
}

FCoreStat::FCoreStat()
{
}
