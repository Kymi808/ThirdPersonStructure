#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SanityCorruptionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSanityChanged, float, NewSanity, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCorruptionChanged, float, NewCorruption, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSanityDepleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFullyCorrupted);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENDSDAWNRPG_API USanityCorruptionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USanityCorruptionComponent();

    UPROPERTY(BlueprintAssignable, Category = "Sanity")
    FOnSanityChanged OnSanityChanged;

    UPROPERTY(BlueprintAssignable, Category = "Corruption")
    FOnCorruptionChanged OnCorruptionChanged;

    UPROPERTY(BlueprintAssignable, Category = "Sanity")
    FOnSanityDepleted OnSanityDepleted;

    UPROPERTY(BlueprintAssignable, Category = "Corruption")
    FOnFullyCorrupted OnFullyCorrupted;

    UFUNCTION(BlueprintCallable, Category = "Sanity")
    void ModifySanity(float Delta);

    UFUNCTION(BlueprintCallable, Category = "Corruption")
    void ModifyCorruption(float Delta);

    UFUNCTION(BlueprintCallable, Category = "Sanity")
    void SetSanity(float NewValue);

    UFUNCTION(BlueprintCallable, Category = "Corruption")
    void SetCorruption(float NewValue);

    UFUNCTION(BlueprintPure, Category = "Sanity")
    FORCEINLINE float GetSanity() const { return Sanity; }

    UFUNCTION(BlueprintPure, Category = "Corruption")
    FORCEINLINE float GetCorruption() const { return Corruption; }

    UFUNCTION(BlueprintPure, Category = "Sanity")
    FORCEINLINE float GetMaxSanity() const { return MaxSanity; }

    UFUNCTION(BlueprintPure, Category = "Corruption")
    FORCEINLINE float GetMaxCorruption() const { return MaxCorruption; }

    UFUNCTION(BlueprintPure, Category = "Sanity")
    bool IsSanityLow() const;

    UFUNCTION(BlueprintPure, Category = "Corruption")
    bool IsCorruptionHigh() const;

    UFUNCTION(BlueprintPure, Category = "Sanity")
    float GetSanityPercent() const;

    UFUNCTION(BlueprintPure, Category = "Corruption")
    float GetCorruptionPercent() const;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sanity")
    float Sanity = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Corruption")
    float Corruption = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sanity")
    float MaxSanity = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Corruption")
    float MaxCorruption = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sanity")
    float LowSanityThreshold = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Corruption")
    float HighCorruptionThreshold = 75.0f;
};
