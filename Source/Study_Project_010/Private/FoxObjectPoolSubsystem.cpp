#include "FoxObjectPoolSubsystem.h"
#include "FoxSingleObjectPool.h"
#include "PooledObject.h"

void UFoxObjectPoolSubsystem::InitializePool(const TArray<FPooledObjectData>& InInitializationData, AActor* PoolRootActor)
{
	Pools.Empty();

	for (const FPooledObjectData& Recipe : InInitializationData)
	{
		UFoxSingleObjectPool* SinglePool = NewObject<UFoxSingleObjectPool>(this);
		if (SinglePool)
		{
			SinglePool->Initialize(Recipe, PoolRootActor);

			Pools.Add(Recipe.ActorName, SinglePool);
		}
	}
}

void UFoxObjectPoolSubsystem::Broadcast_PoolerCleanup()
{
	for (auto& Elem : Pools)
	{
		if (Elem.Value)
		{
			Elem.Value->CleanupAll();
		}
	}
}

AActor* UFoxObjectPoolSubsystem::GetPooledActor(FString Name)
{
	TObjectPtr<UFoxSingleObjectPool>* FoundPool = Pools.Find(Name);
	if (FoundPool && *FoundPool)
	{
		return (*FoundPool)->GetPooledActor();
	}
	return nullptr;
}

void UFoxObjectPoolSubsystem::RecyclePooledObject(UPooledObject* PoolCompRef)
{
	if (!PoolCompRef) return;

	TObjectPtr<UFoxSingleObjectPool>* FoundPool = Pools.Find(PoolCompRef->PoolName);
	if (FoundPool && *FoundPool)
	{
		(*FoundPool)->RecyclePooledObject(PoolCompRef);
	}
}

void UFoxObjectPoolSubsystem::RecycleActor(AActor* PooledActor)
{
	if (!PooledActor) return;

	if (UPooledObject* PoolCompRef = Cast<UPooledObject>(PooledActor->GetComponentByClass(UPooledObject::StaticClass())))
	{
		RecyclePooledObject(PoolCompRef);
	}
}
