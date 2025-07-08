#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatlineComponent.generated.h"

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

	float GetCurrent() const
	{
		return Current;
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
class WAYWARDEXPLORER_API UStatlineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatlineComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMovementCompReference(UCharacterMovementComponent* Comp);

	UFUNCTION(BlueprintCallable)
	float GetStatPercentile(const ECorestat Stat) const;

	UFUNCTION(BlueprintCallable)
	bool CanSprint() const;

	UFUNCTION(BlueprintCallable)
	void SetSprinting(const bool& IsSprinting);

	UFUNCTION(BlueprintCallable)
	bool Canjump() const;

	UFUNCTION(BlueprintCallable)
	void HasJumped();

	UFUNCTION(BlueprintCallable)
	bool CanChannel() const;

	UFUNCTION(BlueprintCallable)
	void SetChanneling(const bool& IsChanneling);

protected:
	virtual void BeginPlay() override;

private:
	class UCharacterMovementComponent* OwningCharMovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Stats", meta = (AllowPrivateAccess = "true"))
	FCoreStat Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Stats", meta = (AllowPrivateAccess = "true"))
	FCoreStat Stamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Stats", meta = (AllowPrivateAccess = "true"))
	FCoreStat Mana;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Stats", meta = (AllowPrivateAccess = "true"))
	FCoreStat Favor = FCoreStat(100, 100, -0.01);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	float SprintCostMultiplier = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 325;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 650;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	float JumpCost = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	bool bIsChanneling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	float ManaCostMultiplier = 2;

	void TickStats(const float& DeltaTime);
	void TickStamina(const float& DeltaTime);
	bool IsValidSprinting() const;
	void TickMana(const float& DeltaTime);
	bool IsValidChanneling() const;

};
