// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawnParent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AVRPawnParent::AVRPawnParent()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVRPawnParent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVRPawnParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRPawnParent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//Function to calculate the direction of 4 vectors and compare them to each other 
//Used to make sure that gesutre points are only successful in the comparison if they are travelling
//In the same direction
bool AVRPawnParent::CompareDirection(FVector Point1, FVector Point2, FVector PointA, FVector PointB, bool AFlipped, bool BFlipped)
{
	//Direction of the points
	FVector APointsDirection, BPointsDirection;

	//Check if points 1 and 2 are flipped and get unit direction
	if (AFlipped)
	{
		APointsDirection = UKismetMathLibrary::GetDirectionUnitVector(Point2, Point1);
	}
	else
	{
		APointsDirection = UKismetMathLibrary::GetDirectionUnitVector(Point1, Point2);
	}

	//Check if points a and b are flipped and get unit direction
	if (BFlipped)
	{
		BPointsDirection = UKismetMathLibrary::GetDirectionUnitVector(PointB, PointA);
	}
	else
	{
		BPointsDirection = UKismetMathLibrary::GetDirectionUnitVector(PointA, PointB);
	}

	//Return if they are rougly equal to each other
	return UKismetMathLibrary::EqualEqual_VectorVector(APointsDirection,BPointsDirection, 0.8);
}

//Function handles finding the length of a gesture
//Used to make sure that the length of a gesture is similar to the compared on
float AVRPawnParent::FindGestureDistance(TArray<FTransform> Points)
{
	float OverallLength = 0;

	//Loop for as many elements in the array
	for(int i = 0;i<Points.Num();i++)
	{
		//Make sure this is not the last element
		if (i < Points.Num()-1)
		{
			//Add to the overall length
			OverallLength += UKismetMathLibrary::Vector_Distance(Points[i].GetLocation(), Points[i + 1].GetLocation());
		}
	}

	//Return the length
	return OverallLength;
}

//Function to compare the points created by the user to the points in the pre set gesture list
bool AVRPawnParent::GestureRecognise(TArray<FTransform> Points, TArray<FTransform> TargetPoints, float &GestureAccuracy )
{
	int ArrayLength = TargetPoints.Num();
	int PNum,TNum;
	bool PFlip,TFlip;
	GestureAccuracy = 0;
	//Check if the number of points is long enough to be compared
	if (Points.Num() > 2)
	{
		//loop for each created point and each gesture point in that loop
		for (int i = 0; i < Points.Num(); i++)
		{
			for (int j = 0; j < TargetPoints.Num(); j++)
			{
				//Check if the locational distance between points are in range 
				if (UKismetMathLibrary::Vector_Distance(TargetPoints[j].GetLocation(), Points[i].GetLocation()) <= 15)
				{
					//Enable flip if this is at the last point of the gestures so it finds the angle backwards
					if (i < Points.Num() - 1)
					{
						PNum = 1;
						PFlip = false;
					}
					else
					{
						PNum = -1;
						PFlip = true;
					}

					if (j < TargetPoints.Num()-1)
					{
						TNum = 1;
						TFlip = false;
					}
					else
					{
						TNum = -1;
						TFlip = true;
					}

					//Check if the direction of the point is of a similar angle
					if (CompareDirection(Points[i].GetLocation(), Points[i + PNum].GetLocation(), TargetPoints[j].GetLocation(), TargetPoints[j + TNum].GetLocation(), PFlip, TFlip))
					{
						//Increase counter
						GestureAccuracy++;
						//FString TheFloatStr = FString::SanitizeFloat(GestureAccuracy);
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TheFloatStr);
						break;
					}
				}

			}

		}

		//Find the overall distance of both gestures
		float a = FindGestureDistance(Points);
		float b = FindGestureDistance(TargetPoints);

		//Check if the distance travelled in both gestures are nearly equal by a tolerance of 25 units
		if (UKismetMathLibrary::NearlyEqual_FloatFloat(a, b, 25))
		{
				//FString TheFloatStr = FString::SanitizeFloat(GestureAccuracy);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TheFloatStr);
			//Find the  accuracy of the gestures
			GestureAccuracy = (GestureAccuracy / Points.Num()) * 100;

			//Check if the  accuracy of the gesture is greater than 80
			if (GestureAccuracy >= 80)
			{
				//Print the accuracy and return true
				return true;
			}
			else
			{
				//Not accurate enough return false
				return false;
			}
		}
		else
		{
			//distance not correct 
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Gesture Length Mismatch!"));
			GestureAccuracy = 0;
			return false;
		}
	}
	else
	{
		//Gesture too small
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Gesture too Small!"));
		GestureAccuracy = 0;
		return false;
	}
}


//Function used to move points back to the center of the world to allow for recognition
TArray<FTransform> AVRPawnParent::MoveGesturePointsBack(TArray<FTransform> RecordedPoints, float YawRotation)
{
	TArray<FTransform> NewPoints;
	FTransform TransformTemp;

	//Loop for each point in the array
	for (FTransform points : RecordedPoints)
	{
		//Create default transform 
		TransformTemp.SetRotation( points.GetRotation());
		TransformTemp.SetScale3D(points.GetScale3D());

		//Add 350 to the Z of the point
		TransformTemp.SetLocation(UKismetMathLibrary::Add_VectorVector(points.GetLocation(), FVector(0, 0, 350)));

		//Set the yaw to the inverse player Yaw
		FRotator rot;
		rot = FRotator::ZeroRotator;
		rot.Yaw = YawRotation;

		//Rotate the point back to face the center
		TransformTemp.SetLocation(rot.RotateVector(TransformTemp.GetLocation()));

		//Add point to array
		NewPoints.Push(TransformTemp);
	}


	//Return array
	return NewPoints;
}


//Function used to record the motion controller to generate points for the gesture
void AVRPawnParent::RecordGesturePositions(UCameraComponent* CameraClass, UMotionControllerComponent* ControllerClass, bool isLeft, bool GestureFind, TArray<FTransform> RecordingPositions, bool RecordStart, bool SetRecord)
{
	//If player isnt performing a gesture then return
	if (!RecordStart)
	{
		return;
	}

	//Check if ready to record the next point
	if (SetRecord)
	{
		//Check if this is not the first point
		if (RecordingPositions.Num() > 0)
		{
			//Get the controller position relative to the user's head
			FVector RelativeControllerPosition = UKismetMathLibrary::Subtract_VectorVector(ControllerClass->GetComponentLocation(), CameraClass->GetComponentLocation());
			
			//Get the distance from the previous point
			float DistanceFromLast = UKismetMathLibrary::Vector_Distance(RelativeControllerPosition, RecordingPositions.Last().GetLocation());

			//If the distance is not far enough away return
			if (!(RecordStart && DistanceFromLast >= SampleDistance))
			{
				return;
			}

		}

		//set the correct variable for the correct hand
		if (isLeft)
		{
			RecordingLeft = false;
		}
		else
		{
			RecordingRight = false;
		}

		//Return here
		return;
	}
	//Not ready to record point
	else
	{
		//Set correct variable for the hand
		if (isLeft)
		{
			RecordingLeft = true;
		}
		else
		{
			RecordingRight = true;
		}

		//Get controller position relative to the headset
		FTransform NewPosition;
		FVector RelativeControllerPosition = UKismetMathLibrary::Subtract_VectorVector(ControllerClass->GetComponentLocation(), CameraClass->GetComponentLocation());
		NewPosition.SetLocation(RelativeControllerPosition);
		NewPosition.SetScale3D(FVector::OneVector);

		//Add to the array of points being generated
		if (GestureFind)
		{
			if (isLeft)
			{
				LeftGesturePositions.Add(NewPosition);
			}
			else
			{
				RightGesturePositions.Add(NewPosition);
			}
		}
		else
		{
			//Add to pre-set gesture 0 as this is the create gesture
			PreSetGestureList[0].GesturePoints.Add(NewPosition);
		}
		return;
	}

}


//Function to start the recognition process
bool AVRPawnParent::BeginGestureRecognise(int& SpellIndex, bool isLeft, TArray<FTransform> RecordedPositions, bool GestureFind)
{
	//if not finding a gesture return here
	if (!GestureFind)
	{
		ExposedGesturePositions = PreSetGestureList[0].GesturePoints;
		SpellIndex = 0;
		return false;

	}

	//local variables for returning later
	float GestureAccuracy = 0;
	int GestureIndex = 0;
	bool found = false;

	//If one the right hand flip the gesture to be on the left side
	if (!isLeft)
	{
		TArray<FTransform> TempArray = RecordedPositions;
		for (int i = 0; i < TempArray.Num(); i++)
		{
			FTransform NewPoint;
			FVector FlippedLocation = TempArray[i].GetLocation();
			FlippedLocation.Y = FlippedLocation.Y * -1;
			NewPoint.SetScale3D(FVector::OneVector);
			NewPoint.SetLocation(FlippedLocation);
			TempArray[i] = NewPoint;
		}

		RecordedPositions = TempArray;
	}

	float acc = 0;

	//Loop for the number of preset gestures
	for (int i =0;i<PreSetGestureList.Num();i++)
	{
		//Do gesture recognition and find the gesture with the highest accuracy
		if (GestureRecognise(RecordedPositions, PreSetGestureList[i].GesturePoints, acc))
		{
			if (acc > GestureAccuracy)
			{
				GestureAccuracy = acc;
				GestureIndex = i;
				found = true;
			}
		}

	}

	//Return the spell index
	SpellIndex = GestureIndex;
	return found;

}

//Function used to adjust points to the height of the player
FVector AVRPawnParent::AdjustToPlayerHeight(FVector Input, bool isLeft,UCameraComponent *CameraClass)
{
	//Flip the points if on the right hand
	int flip = 1;
	int adjustment = -15;
	if (!isLeft) { flip = -1; adjustment = 0; }

	FVector NewVector = Input;

	//Moves the points back down to 0
	NewVector = UKismetMathLibrary::Subtract_VectorVector(NewVector,FVector(0,0,350));

	//Add the player height
	NewVector = UKismetMathLibrary::Add_VectorVector(NewVector, FVector(0, 0, CameraClass->GetComponentLocation().Z));


	//Flip the Y if on the right
	NewVector.Y = NewVector.Y * flip;

	//Rotate the points to the player head rotation
	FRotator Rotation(0,CameraClass->GetComponentRotation().Yaw + adjustment ,0);
	NewVector = Rotation.RotateVector(NewVector);

	//Move the points to the player location
	NewVector = UKismetMathLibrary::Add_VectorVector(NewVector, FVector(CameraClass->GetComponentLocation().X, CameraClass->GetComponentLocation().Y, 0));
	return NewVector;
}

//Function to start recording the players gesture
void AVRPawnParent::BeginGestureRecord(bool isLeft, UCameraComponent* CameraClass)
{
	//Get the rotation of the player head , This needs to be inverse as it will be rotating points back to center
	PlayerYaw = CameraClass->GetComponentRotation().GetInverse().Yaw;

	//Set the variables for the correct hand and empty the array 
	if (isLeft)
	{
		FindLeft = true;
		RecordingLeft = true;
		LeftGesturePositions.Empty();
	}
	else
	{
		FindRight = true;
		RecordingRight = true;
		RightGesturePositions.Empty();
	}

}


//Function ends the gesture recording and starts recognition
FString AVRPawnParent::EndGestureRecord(bool isLeft, int &SpellIndex)
{
	TArray <FTransform> tempArray;

	//Set for correct hand
	if (isLeft)
	{
		RecordingLeft = true;
		StartRecordingLeft = true;
		tempArray = LeftGesturePositions;
	}
	else
	{
		RecordingRight = true;
		StartRecordingRight = true;
		tempArray = RightGesturePositions;
	}

	//Process the points for recogntions
	tempArray = MoveGesturePointsBack(tempArray, PlayerYaw);
	int SpellNum = 0;
	
	//Do recognition
	if (BeginGestureRecognise(SpellNum, isLeft, tempArray, true))
	{
		//Return the spell name
		SpellIndex = SpellNum;
		return (PreSetGestureList[SpellNum].SpellName);
	}
	else
	{
		SpellIndex = 0;
		return " ";
	}
}

