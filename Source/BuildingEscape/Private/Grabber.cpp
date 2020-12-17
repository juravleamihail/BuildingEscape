// Copyright Mihail Juravlea 2020


#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UGrabber::FindPhysicsHandleComponent()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty"));

	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(!physicsHandle)
	{		
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}

void UGrabber::SetupInputComponent()
{
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if(inputComponent)
	{
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName())
	}
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Setup query parameters
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());
	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(OUT hit, GetReachLineStart(), GetReachLineEnd(), 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), traceParameters);

	// See What we hit
	AActor* actorHit = hit.GetActor();
	if(actorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *actorHit->GetName())
	}

	return hit;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(physicsHandle && physicsHandle->GrabbedComponent)
	{
		physicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

void UGrabber::Grab()
{
	auto hit = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent* componentToGrab = hit.GetComponent();
	auto actorHit = hit.GetActor();

	if(actorHit && componentToGrab)
	{
		physicsHandle->GrabComponentAtLocation(componentToGrab, NAME_None, componentToGrab->GetOwner()->GetActorLocation());
	}
}

void UGrabber::Release()
{
	physicsHandle->ReleaseComponent();
}

FVector UGrabber::GetReachLineEnd()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);

	return playerViewPointLocation + playerViewPointRotation.Vector() * reach;
}

FVector UGrabber::GetReachLineStart()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);

	return playerViewPointLocation;
}

