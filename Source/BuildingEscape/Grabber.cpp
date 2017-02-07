// Copyright Corsino 2017

#include "BuildingEscape.h"
#include "Grabber.h"

// Only used to mark parameters passed by reference
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Look for attached Physics Handle
	FindPhysicsHandleComponet();

	// Look for attached Input Component
	SetupInputComponent();
}

// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponet()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		// Physics handle is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Component PhysicsHandle not found in: %s"), *GetOuter()->GetName());
	}
}

// Setup (assumed) attached input component
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		// Input Component is found
		UE_LOG(LogTemp, Warning, TEXT("Input Component found"));

		/// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input Component not found in: %s"), *GetOuter()->GetName());
	}
}

// Ray-cast and grab what's in reach
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));

	/// Line trace and see if we reach any actors with physics body collision channel set
	GetFirstPhysicsBodyInReach();

	/// If we hit somethin then attach a physics handle
	// TODO attach physics handle
}

// Call when grab is released
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Released"));
	// TODO realease physics handle
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// if the physics handle is attached
		// move the object that we're holding
	
}

// Return hit for first physics body in reach
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Get the player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTracedEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

	/// Setup query parameters
	/// Parameters: None, Dont use complex trace, ignore the user actor
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-trace (AKA Ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTracedEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"),
			*ActorHit->GetName()
		);
	}

	return FHitResult();
}