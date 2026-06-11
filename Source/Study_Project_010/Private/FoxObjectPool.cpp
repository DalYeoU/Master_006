#include "FoxObjectPool.h"
#include "FoxObjectPoolSubsystem.h"

void AFoxObjectPool::BeginPlay()
{
	Super::BeginPlay();

	UFoxObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UFoxObjectPoolSubsystem>();
	if (PoolSubsystem)
	{
		PoolSubsystem->InitializePool(PooledObjectData, this);
	}
}

void AFoxObjectPool::Broadcast_PoolerCleanup()
{
	if (UFoxObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UFoxObjectPoolSubsystem>())
	{
		PoolSubsystem->Broadcast_PoolerCleanup();
	}
}

AActor* AFoxObjectPool::GetPooledActor(FString Name)
{
	if (UFoxObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UFoxObjectPoolSubsystem>())
	{
		return PoolSubsystem->GetPooledActor(Name);
	}
	return nullptr;
}

void AFoxObjectPool::RecyclePooledObject(UPooledObject* PoolCompRef)
{
	if (UFoxObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UFoxObjectPoolSubsystem>())
	{
		PoolSubsystem->RecyclePooledObject(PoolCompRef);
	}
}

void AFoxObjectPool::RecycleActor(AActor* PooledActor)
{
	if (UFoxObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UFoxObjectPoolSubsystem>())
	{
		PoolSubsystem->RecycleActor(PooledActor);
	}
}