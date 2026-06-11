#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PooledObject.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STUDY_PROJECT_010_API UPooledObject : public UActorComponent
{
	GENERATED_BODY()

public:	

	void Init(class UFoxObjectPoolSubsystem* Owner, FString InPoolName);

	UFUNCTION(BlueprintCallable)
	void RecycleSelf();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bIsPoolActive;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Object Pool")
	FString PoolName;

private:

	TObjectPtr<class UFoxObjectPoolSubsystem> ObjectPool;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
};