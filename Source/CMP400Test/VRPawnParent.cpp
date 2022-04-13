// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawnParent.h"
#include "Kismet/KismetMathLibrary.h"

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

bool AVRPawnParent::GestureRecognise(TArray<FTransform> Points, TArray<FTransform> TargetPoints, float &GestureAccuracy )
{
	int ArrayLength = TargetPoints.Num();
	int PNum,TNum;
	bool PFlip,TFlip;

	if (Points.Num() > 2)
	{
		for (int i = 0; i < Points.Num(); i++)
		{
			for (int j = 0; j < TargetPoints.Num(); j++)
			{
				if (UKismetMathLibrary::Vector_Distance(TargetPoints[j].GetLocation(), Points[i].GetLocation()) <= 15)
				{
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

					if (CompareDirection(Points[i].GetLocation(), Points[i + PNum].GetLocation(), TargetPoints[j].GetLocation(), TargetPoints[j + TNum].GetLocation(), PFlip, TFlip))
					{
						GestureAccuracy++;
						break;
					}
				}

			}

		}
		float a = FindGestureDistance(Points);
		float b = FindGestureDistance(TargetPoints);
		if (UKismetMathLibrary::NearlyEqual_FloatFloat(a, b, 25))
		{
			GestureAccuracy = (GestureAccuracy / Points.Num()) * 100;
			if (GestureAccuracy >= 80)
			{
				FString TheFloatStr = FString::SanitizeFloat(GestureAccuracy);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TheFloatStr);
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Gesture Length Mismatch!"));
			GestureAccuracy = 0;
			return false;
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Gesture too Small!"));
		GestureAccuracy = 0;
		return false;
	}
}

