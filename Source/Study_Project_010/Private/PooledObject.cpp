#include "PooledObject.h"
#include "FoxObjectPoolSubsystem.h"

void UPooledObject::Init(UFoxObjectPoolSubsystem* Owner, FString InPoolName)
{
	bIsPoolActive = false;
	ObjectPool = Owner;
	PoolName = InPoolName;
}

void UPooledObject::RecycleSelf()
{
	ObjectPool->RecyclePooledObject(this);
}

void UPooledObject::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	ObjectPool->OnPoolerCleanup.RemoveDynamic(this, &UPooledObject::RecycleSelf);
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}