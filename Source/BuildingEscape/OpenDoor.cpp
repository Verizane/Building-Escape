// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Grabber.h"

#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing Pressure Plate"), *GetOwner()->GetName())
	}

	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s failed getting Owner"), *GetOwner()->GetName())
	}

	/*
	FString ObjectName = Owner->GetName();
	FString ObjectRotation = Owner->GetActorRotation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s is rotated at %s"), *ObjectName, *ObjectRotation);
	*/
	
}


void UOpenDoor::CloseDoor()
{
	if (!Owner) { return; }
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



	// Poll the Trigger Volume

	if (GetTotalMassOfActorsOnPlate() > TriggerMass) // TODO make into a parameter
	{
		OnOpen.Broadcast();
	}
	else  
	{	
		OnClose.Broadcast();
	}

	// Check if it's time to close the door
	
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float TotalMass = 0.f;


	if (PressurePlate == nullptr) { return TotalMass; }

	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);



	// Iterate through them adding their masses
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName())
	}
	return TotalMass;
}