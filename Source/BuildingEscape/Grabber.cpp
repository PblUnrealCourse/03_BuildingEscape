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
	if (PhysicsHandle == nullptr)
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
	/// Line trace and see if we reach any actors with physics body collision channel set
	auto HitResutl = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResutl.GetComponent(); // gets the mesh in our case
	auto ActorHit = HitResutl.GetActor();

	/// If we hit somethin then attach a physics handle
	if (ActorHit)
	{
		// Attach physics handle
		// TODO Lock rotation of the grabbed object
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation()
		);
	}
}

// Call when grab is released
void UGrabber::Release()
{
	// Realease physics handle
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(getReachLineTraceEnd());
	}
}

// Return hit for first physics body in reach
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Setup query parameters
	/// Parameters: None, Dont use complex trace, ignore the user actor
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-trace (AKA Ray-cast) out to reach distance
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		getReachLineTraceStart(),
		getReachLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	return HitResult;
}

// Returns current start of the reach line
const FVector UGrabber::getReachLineTraceStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

// Returns current end of the reach line
const FVector UGrabber::getReachLineTraceEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}
