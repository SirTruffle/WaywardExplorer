// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatlineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


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
	if (bIsSprinting && IsValidSprinting())
	{
		Stamina.TickStat(0 - (DeltaTime * SprintCostMultiplier));
		if (Stamina.GetCurrent() <= 0.0)
		{
			SetSprinting(false);
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
	OwningCharMovementComp->MaxWalkSpeed = IsSprinting ? SprintSpeed : WalkSpeed;
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
		Mana.TickStat(0 - (DeltaTime * ManaCostMultiplier));
		if (Mana.GetCurrent() <= 0.0)
		{
			SetChanneling(false);
		}
		return;
	}
	Mana.TickStat(DeltaTime);
}
bool UStatlineComponent::IsValidChanneling() const
{
	return true;
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
