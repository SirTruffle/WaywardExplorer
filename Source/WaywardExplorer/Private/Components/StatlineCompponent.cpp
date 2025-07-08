// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatlineCompponent.h"

// Sets default values for this component's properties
UStatlineCompponent::UStatlineCompponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatlineCompponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UStatlineCompponent::TickStats(const float& DeltaTime)
{
	Health.TickStat(DeltaTime);
	Stamina.TickStat(DeltaTime);
	Mana.TickStat(DeltaTime);
	Favor.TickStat(DeltaTime);
}


// Called every frame
void UStatlineCompponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TickType != ELevelTick::LEVELTICK_PauseTick)
	{
		TickStats(DeltaTime);
	}
}

float UStatlineCompponent::GetStatPercentile(const ECorestat Stat) const
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
