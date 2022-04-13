// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRPawnParent.generated.h"

USTRUCT(BlueprintType)
struct FPreSetSpellGesture
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTransform> GesturePoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SpellName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ReleaseCast;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ChargeSpeed;

};



UCLASS()
class CMP400TEST_API AVRPawnParent : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawnParent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FPreSetSpellGesture> PreSetGestureList;








	//
	UFUNCTION(BlueprintCallable)
		bool CompareDirection(FVector Point1, FVector Point2, FVector PointA, FVector PointB,bool AFlipped, bool BFlipped);

	UFUNCTION(BlueprintCallable)
		float FindGestureDistance(TArray <FTransform> Points);

	UFUNCTION(BlueprintCallable)
		bool GestureRecognise(TArray<FTransform>Points,TArray<FTransform>TargetPoints,float &GestureAccuracy);

};
