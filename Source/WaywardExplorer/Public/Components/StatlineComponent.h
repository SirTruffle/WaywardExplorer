#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/SaveActorInterface.h"
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

	FString GetSaveString()
	{
		FString Ret = FString::SanitizeFloat(Current);
		Ret += "|";
		Ret += FString::SanitizeFloat(Max);
		Ret += "|";
		Ret += FString::SanitizeFloat(PerSecondTick);
		return Ret;
	}

	void UpdateFromSaveString(TArray<FString> Parts)
	{
		if (Parts.Num() < 3)
		{
			//TODO: Log error about invalid save string
			return;
		}
		Current = FCString::Atof(*Parts[0]);
		Max = FCString::Atof(*Parts[1]);
		PerSecondTick = FCString::Atof(*Parts[2]);
	}

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, meta = (AllowPrivateAccess = "true"))
	float Current = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, meta = (AllowPrivateAccess = "true"))
	float Max = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, meta = (AllowPrivateAccess = "true"))
	float PerSecondTick = 1;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WAYWARDEXPLORER_API UStatlineComponent : public UActorComponent, public ISaveActorInterface
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
	void SetSneaking(const bool& IsSneaking);

	UFUNCTION(BlueprintCallable)
	bool Canjump() const;

	UFUNCTION(BlueprintCallable)
	void HasJumped();

	UFUNCTION(BlueprintCallable)
	bool CanChannel() const;

	UFUNCTION(BlueprintCallable)
	void SetChanneling(const bool& IsChanneling);

	virtual FSaveComponentData GetComponentSaveData_Implementation();
	void SetComponentSaveData_Implementation(FSaveComponentData Data);

protected:
	virtual void BeginPlay() override;

private:
	class UCharacterMovementComponent* OwningCharMovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category = "Core Stats", meta = (AllowPrivateAccess = "true"))
	FCoreStat Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category = "Core Stats", meta = (AllowPrivateAccess = "true"))
	FCoreStat Stamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category = "Core Stats", meta = (AllowPrivateAccess = "true"))
	FCoreStat Mana;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category = "Core Stats", meta = (AllowPrivateAccess = "true"))
	FCoreStat Favor = FCoreStat(100, 100, -0.01);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Core Stats", meta = (AllowPrivateAccess = "true"))
	float SecTriggerStamineExhaustion = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Stats", meta = (AllowPrivateAccess = "true"))
	float CurrentStaminaExhaustion = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	float SprintCostMultiplier = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 325;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 650;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	float SneakSpeed = 75;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	bool bIsSneaking = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	float JumpCost = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	bool bIsChanneling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	float ManaCostMultiplier = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	float ManaUsedThisChannel = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	float ChannelStrength = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	float ChannelMultiplier = 1.0f; // You can adjust this dynamically later

	void TickStats(const float& DeltaTime);
	void TickStamina(const float& DeltaTime);
	bool IsValidSprinting() const;
	void TickMana(const float& DeltaTime);
	bool IsValidChanneling() const;
	void FinalizeChannelStrength();

};
