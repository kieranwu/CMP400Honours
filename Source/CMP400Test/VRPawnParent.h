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

	//List of pre-set gestures
	UPROPERTY( BlueprintReadWrite)
		TArray<FPreSetSpellGesture> PreSetGestureList;

	//Array of transform used to store the points generated from the left hand
	UPROPERTY( BlueprintReadWrite)
		TArray<FTransform> LeftGesturePositions;

	//Array of transform used to store the points generated from the Right hand
	UPROPERTY( BlueprintReadWrite)
		TArray<FTransform> RightGesturePositions;

	//Array of transform used to store the points generated from the user and exposed to retreive the points from editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTransform> ExposedGesturePositions;

	//Distance the controller needs to travel before being sampled
	UPROPERTY( BlueprintReadWrite)
		float SampleDistance = 7;

	//Bool to check if gesture is currently recording a point
	UPROPERTY( BlueprintReadWrite)
		bool RecordingLeft;

	UPROPERTY(BlueprintReadWrite)
		bool RecordingRight;

	//Bool to check if recording has started
	UPROPERTY(BlueprintReadWrite)
		bool StartRecordingLeft;

	UPROPERTY( BlueprintReadWrite)
		bool StartRecordingRight;

	//Bool to check if finding a gesture rather than creating one
	UPROPERTY( BlueprintReadWrite)
		bool FindLeft;

	UPROPERTY( BlueprintReadWrite)
		bool FindRight;

	//Player head mounted display Yaw world rotation
	UPROPERTY( BlueprintReadWrite)
		float PlayerYaw;



	//Function to calculate the direction of 4 vectors and compare them to each other 
	UFUNCTION(BlueprintCallable)
		bool CompareDirection(FVector Point1, FVector Point2, FVector PointA, FVector PointB,bool AFlipped, bool BFlipped);

	//Function handles finding the length of a gesture
	UFUNCTION(BlueprintCallable)
		float FindGestureDistance(TArray <FTransform> Points);

	//Function to compare the points created by the user to the points in the pre set gesture list
	UFUNCTION(BlueprintCallable)
		bool GestureRecognise(TArray<FTransform>Points,TArray<FTransform>TargetPoints,float &GestureAccuracy);

	//Function used to move points back to the center of the world to allow for recognition
	UFUNCTION(BlueprintCallable)
		TArray<FTransform> MoveGesturePointsBack(TArray<FTransform>RecordedPoints, float YawRotation);

	//Function used to record the motion controller to generate points for the gesture
	UFUNCTION(BlueprintCallable)
		void RecordGesturePositions(UCameraComponent* CameraClass, UMotionControllerComponent* ControllerClass, bool isLeft, bool GestureFind, TArray<FTransform> RecordingPositions, bool RecordStart, bool SetRecord);

	//Function to start the recognition process
	UFUNCTION(BlueprintCallable)
		bool BeginGestureRecognise(int &SpellIndex, bool isLeft, TArray<FTransform> RecordedPositions, bool GestureFind);

	//Function used to adjust points to the height of the player
	UFUNCTION(BlueprintCallable)
		FVector AdjustToPlayerHeight(FVector Input, bool isLeft, UCameraComponent* CameraClass);

	//Function to start recording the players gesture
	UFUNCTION(BlueprintCallable)
		void BeginGestureRecord(bool isLeft, UCameraComponent* CameraClass);

	//Function ends the gesture recording and starts recognition
	UFUNCTION(BlueprintCallable)
		FString EndGestureRecord(bool isLeft, int& SpellIndex);

};
