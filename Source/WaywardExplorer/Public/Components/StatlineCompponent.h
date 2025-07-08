// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatlineCompponent.generated.h"

UENUM(BlueprintType)
enum class ECorestat : uint8
{
	CS_Health UMETA(DisplayName = "Health"),
	CS_Stamina UMETA(DisplayName = "Stamina"),
	CS_Mana UMETA(DisplayName = "Mana"),
	CS_Favor UMETA(DisplayName = "Favor")
};

USTRUCT(BlueprintType)
struct FCoreStat
{
	GENERATED_USTRUCT_BODY()


public:

	FCoreStat();
	FCoreStat(const float& current, const float& max, const float& tick)
	{
		Current = current;
		Max = max;
		PerSecondTick = tick;
		
	}
	void TickStat(const float& DeltaTime)
	{
		Current = FMath::Clamp(Current + (PerSecondTick * DeltaTime), 0, Max);
	}

	void Adjust(const float& Amount)
	{
		Current = FMath::Clamp(Current + Amount, 0, Max);
	}

	float Percentile() const
	{
		return (Current / 100);
	}

	void AdjustTick(const float& Amount)
	{
		PerSecondTick = Amount;
	}

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Current = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Max = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float PerSecondTick = 1;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WAYWARDEXPLORER_API UStatlineCompponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatlineCompponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Core Stats")
	float GetStatPercentile(const ECorestat Stat) const;

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Stats", meta = (AllowPrivateAccess = "true"))
	FCoreStat Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Stats", meta = (AllowPrivateAccess = "true"))
	FCoreStat Stamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Stats", meta = (AllowPrivateAccess = "true"))
	FCoreStat Mana;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Stats", meta = (AllowPrivateAccess = "true"))
	FCoreStat Favor = FCoreStat(100, 100, -0.01);

	void TickStats(const float& DeltaTime);
};
