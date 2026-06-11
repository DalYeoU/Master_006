#include "FoxSingleObjectPool.h"
#include "PooledObject.h"
#include "FoxObjectPoolSubsystem.h"

void UFoxSingleObjectPool::Initialize(const FPooledObjectData& InPoolData, AActor* InPoolRoot)
{
	PoolData = InPoolData;
	PoolRoot = InPoolRoot;
	PooledObjects.Empty();

	for (int ObjectIndex = 0; ObjectIndex < PoolData.PoolSize; ObjectIndex++)
	{
		if (UPooledObject* NewActorComp = SpawnNewActor())
		{
			PooledObjects.Add(NewActorComp);
		}
	}
}

AActor* UFoxSingleObjectPool::GetPooledActor()
{
	UPooledObject* TargetComp = GetAvailableObject();

	return TargetComp ? ActivatePooledActor(TargetComp) : nullptr;
}

void UFoxSingleObjectPool::RecyclePooledObject(UPooledObject* PoolCompRef)
{
	if (!PoolCompRef) return;

	PoolCompRef->bIsPoolActive = false;
	
	AActor* ReturningActor = PoolCompRef->GetOwner();
	if (ReturningActor)
	{
		ReturningActor->SetActorHiddenInGame(true);
		ReturningActor->SetActorEnableCollision(false);
		ReturningActor->SetActorTickEnabled(false);

		if (PoolRoot.IsValid())
		{
			ReturningActor->AttachToActor(PoolRoot.Get(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

void UFoxSingleObjectPool::RecycleActor(AActor* PooledActor)
{
	if (!PooledActor) return;

	if (UPooledObject* PoolCompRef = Cast<UPooledObject>(PooledActor->GetComponentByClass(UPooledObject::StaticClass())))
	{
		RecyclePooledObject(PoolCompRef);
	}
}

void UFoxSingleObjectPool::CleanupAll()
{
	for (int i = 0; i < PooledObjects.Num(); i++)
	{
		if (PooledObjects[i] && PooledObjects[i]->bIsPoolActive)
		{
			RecyclePooledObject(PooledObjects[i]);
		}
	}
}

void UFoxSingleObjectPool::RegenItem(int PositionIndex)
{
	if (UPooledObject* NewActorComp = SpawnNewActor())
	{
		PooledObjects.Insert(NewActorComp, PositionIndex);
	}
}

UPooledObject* UFoxSingleObjectPool::SpawnNewActor()
{
	UFoxObjectPoolSubsystem* PoolSubsystem = Cast<UFoxObjectPoolSubsystem>(GetOuter());
	if (!PoolSubsystem || !GetWorld()) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = FName(FString::Printf(TEXT("%s"), *PoolData.ActorName));
	SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor(PoolData.ActorTemplate, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
	if (SpawnedActor)
	{
		SpawnedActor->SetActorLabel(SpawnedActor->GetName());

		UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);
		if (PoolComp)
		{
			PoolComp->RegisterComponent();
			SpawnedActor->AddInstanceComponent(PoolComp);
			
			PoolComp->Init(PoolSubsystem, PoolData.ActorName);

			SpawnedActor->SetActorHiddenInGame(true);
			SpawnedActor->SetActorEnableCollision(false);
			SpawnedActor->SetActorTickEnabled(false);

			if (PoolRoot.IsValid())
			{
				SpawnedActor->AttachToActor(PoolRoot.Get(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			}

			return PoolComp;
		}
	}

	return nullptr;
}

UPooledObject* UFoxSingleObjectPool::GetAvailableObject()
{
	int PooledObjectCount = PooledObjects.Num();

	for (int i = 0; i < PooledObjectCount; i++)
	{
		if (PooledObjects[i] != nullptr)
		{
			if (!PooledObjects[i]->bIsPoolActive)
			{
				return PooledObjects[i];
			}
		}
		else
		{
			RegenItem(i);
			return PooledObjects[i];
		}
	}

	if (PoolData.CanGrow)
	{
		if (UPooledObject* NewActorComp = SpawnNewActor())
		{
			PooledObjects.Add(NewActorComp);
			return NewActorComp;
		}
	}

	return nullptr;
}

AActor* UFoxSingleObjectPool::ActivatePooledActor(UPooledObject* PoolComp)
{
	if (!PoolComp) return nullptr;

	PoolComp->bIsPoolActive = true;

	AActor* TargetActor = PoolComp->GetOwner();
	if (TargetActor)
	{
		TargetActor->SetActorHiddenInGame(false);
		TargetActor->SetActorEnableCollision(true);
		TargetActor->SetActorTickEnabled(true);
		
		TargetActor->AttachToActor(nullptr, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	return TargetActor;
}
