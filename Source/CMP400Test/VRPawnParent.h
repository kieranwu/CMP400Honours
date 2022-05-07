// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
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
	UPROPERTY( BlueprintReadWrite)
		TArray<FPreSetSpellGesture> PreSetGestureList;

	UPROPERTY( BlueprintReadWrite)
		TArray<FTransform> LeftGesturePositions;

	UPROPERTY( BlueprintReadWrite)
		TArray<FTransform> RightGesturePositions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTransform> ExposedGesturePositions;

	UPROPERTY( BlueprintReadWrite)
		float SampleDistance = 7;

	UPROPERTY( BlueprintReadWrite)
		bool RecordingLeft;

	UPROPERTY(BlueprintReadWrite)
		bool RecordingRight;

	UPROPERTY(BlueprintReadWrite)
		bool StartRecordingLeft;

	UPROPERTY( BlueprintReadWrite)
		bool StartRecordingRight;

	UPROPERTY( BlueprintReadWrite)
		bool FindLeft;

	UPROPERTY( BlueprintReadWrite)
		bool FindRight;

	UPROPERTY( BlueprintReadWrite)
		float PlayerYaw;



	//
	UFUNCTION(BlueprintCallable)
		bool CompareDirection(FVector Point1, FVector Point2, FVector PointA, FVector PointB,bool AFlipped, bool BFlipped);

	UFUNCTION(BlueprintCallable)
		float FindGestureDistance(TArray <FTransform> Points);

	UFUNCTION(BlueprintCallable)
		bool GestureRecognise(TArray<FTransform>Points,TArray<FTransform>TargetPoints,float &GestureAccuracy);

	UFUNCTION(BlueprintCallable)
		TArray<FTransform> MoveGesturePointsBack(TArray<FTransform>RecordedPoints, float YawRotation);

	UFUNCTION(BlueprintCallable)
		void RecordGesturePositions(UCameraComponent* CameraClass, UMotionControllerComponent* ControllerClass, bool isLeft, bool GestureFind, TArray<FTransform> RecordingPositions, bool RecordStart, bool SetRecord);

	UFUNCTION(BlueprintCallable)
		bool BeginGestureRecognise(int &SpellIndex, bool isLeft, TArray<FTransform> RecordedPositions, bool GestureFind);

	UFUNCTION(BlueprintCallable)
		FVector AdjustToPlayerHeight(FVector Input, bool isLeft, UCameraComponent* CameraClass);

	UFUNCTION(BlueprintCallable)
		void BeginGestureRecord(bool isLeft, UCameraComponent* CameraClass);

	UFUNCTION(BlueprintCallable)
		FString EndGestureRecord(bool isLeft, int& SpellIndex);

};
