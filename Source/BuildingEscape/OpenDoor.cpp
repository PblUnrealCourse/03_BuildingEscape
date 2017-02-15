// Copyright Corsino 2017

#include "BuildingEscape.h"
#include "OpenDoor.h"

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

	// Find the owning Actor
	Owner = GetOwner();

	if (!PressurePlate) 
	{
		UE_LOG(LogTemp, Error, TEXT("PressurePlate Component not found in: %s"), *GetOuter()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the Trigger Volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	
	if (!PressurePlate) { return TotalMass; }
	
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate trough them adding their masses
	for (const auto& Actor : OverlappingActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapping actor number: %i , name: %s"), OverlappingActors.Num(), *Actor->GetName());
	
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

