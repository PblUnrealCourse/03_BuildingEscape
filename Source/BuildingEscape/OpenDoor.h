// Copyright Corsino 2017

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

/// Make this class asign bluprint events
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(BluePrintAssignable)
	FDoorEvent OnOpen;
	
	UPROPERTY(BluePrintAssignable)
	FDoorEvent OnClose;

private:
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float TriggerMass = 30.f;

	// The owning door
	AActor* Owner = nullptr; // The owning door

	// Return total mass in kg
	float GetTotalMassOfActorsOnPlate();
};
