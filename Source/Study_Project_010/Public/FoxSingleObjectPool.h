#pragma once

#include "CoreMinimal.h"
#include "PooledObjectData.h"
#include "FoxSingleObjectPool.generated.h"

UCLASS()
class STUDY_PROJECT_010_API UFoxSingleObjectPool : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(const FPooledObjectData& InPoolData, AActor* InPoolRoot);

	AActor* GetPooledActor();

	void RecyclePooledObject(class UPooledObject* PoolCompRef);

	void RecycleActor(AActor* PooledActor);

	void CleanupAll();

protected:
	UPROPERTY()
	FPooledObjectData PoolData;

	UPROPERTY()
	TWeakObjectPtr<AActor> PoolRoot;

	UPROPERTY()
	TArray<TObjectPtr<class UPooledObject>> PooledObjects;

private:
	class UPooledObject* SpawnNewActor();

	class UPooledObject* GetAvailableObject();

	AActor* ActivatePooledActor(class UPooledObject* PoolComp);

	void RegenItem(int PositionIndex);
};
