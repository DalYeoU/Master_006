#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PooledObjectData.h"
#include "FoxObjectPoolSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPoolerCleanupSignature);

UCLASS()
class STUDY_PROJECT_010_API UFoxObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void InitializePool(const TArray<FPooledObjectData>& InInitializationData, AActor* PoolRootActor);

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void Broadcast_PoolerCleanup();

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	AActor* GetPooledActor(FString Name);

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void RecyclePooledObject(class UPooledObject* PoolCompRef);

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void RecycleActor(AActor* PooledActor);

public:
	UPROPERTY(BlueprintAssignable, Category = "Object Pool")
	FPoolerCleanupSignature OnPoolerCleanup;

protected:
	UPROPERTY()
	TMap<FString, TObjectPtr<class UFoxSingleObjectPool>> Pools;
};