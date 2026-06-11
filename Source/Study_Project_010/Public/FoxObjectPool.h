#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledObjectData.h"
#include "FoxObjectPool.generated.h"

UCLASS()
class STUDY_PROJECT_010_API AFoxObjectPool : public AActor
{
	GENERATED_BODY()

protected:
	// 레벨 시작 시 서브시스템에 풀링 레시피 데이터 및 자기 자신을 루트 부모로 전달해 초기화합니다.
	virtual void BeginPlay() override;

public:	
	// 전체 풀 회수 요청을 서브시스템으로 전달합니다.
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void Broadcast_PoolerCleanup();

	// 서브시스템을 통해 풀 액터를 빌려옵니다 (블루프린트 하위 호환용 래퍼 함수).
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	AActor* GetPooledActor(FString Name);

	// 서브시스템을 통해 풀링 컴포넌트 기준 반납을 요청합니다.
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void RecyclePooledObject(class UPooledObject* PoolCompRef);

	// 서브시스템을 통해 액터 기준 반납을 요청합니다.
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void RecycleActor(AActor* PooledActor);

protected:
	// 에디터 디테일 패널에서 설정할 풀링 데이터 정보 (레시피)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pool")
	TArray<FPooledObjectData> PooledObjectData;
};