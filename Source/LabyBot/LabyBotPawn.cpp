// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyBotPawn.h"
#include "LabyBotProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "DrawDebugHelpers.h"

#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, String)

const FName ALabyBotPawn::MoveForwardBinding("MoveForward");
const FName ALabyBotPawn::MoveRightBinding("MoveRight");
const FName ALabyBotPawn::FireForwardBinding("FireForward");
const FName ALabyBotPawn::FireRightBinding("FireRight");

ALabyBotPawn::ALabyBotPawn()
{	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

	//Set Materials
	MaterialOne = CreateDefaultSubobject<UMaterialInterface>("MaterialOne");
	MaterialTwo = CreateDefaultSubobject<UMaterialInterface>("MaterialTwo");
	MaterialThree = CreateDefaultSubobject<UMaterialInterface>("MaterialThree");
	MaterialFour = CreateDefaultSubobject<UMaterialInterface>("MaterialFour");
	MaterialFive = CreateDefaultSubobject<UMaterialInterface>("MaterialFive");

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	// Movement
	MoveSpeed = 500.0f;

	currentDirectionPawn = DirectionPawn::Up;
}

void ALabyBotPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
}

void ALabyBotPawn::Tick(float DeltaSeconds)
{
	if (!Started) return;
	
	Raycast();

	UpdateMaterial();

	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	//FString ForwardValueStr = FString::SanitizeFloat(ForwardValue);
	//UE_LOG(LogTemp, Warning, TEXT("Tick"));
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	//const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
	
	FVector MoveDirection;

	switch (currentDirectionPawn) {
		case DirectionPawn::Up:
			MoveDirection = FVector(1.f, 0.f, 0.f).GetClampedToMaxSize(1.0f);
			break;
		case DirectionPawn::Down:
			MoveDirection = FVector(-1.f, 0.f, 0.f).GetClampedToMaxSize(1.0f);
			break;
		case DirectionPawn::Right:
			MoveDirection = FVector(0.f, 1.f, 0.f).GetClampedToMaxSize(1.0f);
			break;
		case DirectionPawn::Left:
			MoveDirection = FVector(0.f, -1.f, 0.f).GetClampedToMaxSize(1.0f);
			break;
		default:
			MoveDirection = FVector(1.f, 0.f, 0.f).GetClampedToMaxSize(1.0f);
			break;
	}
	if (BatteryLeft <= 0) {
		MoveDirection = FVector(0.f, 0.f, 0.f).GetClampedToMaxSize(1.0f);
	}

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = Movement.Rotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);
		
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}
}

void ALabyBotPawn::Raycast() {
	FHitResult OutHit;

	FVector Start = ShipMeshComponent->GetComponentLocation();
	FVector ForwardVector = ShipMeshComponent->GetForwardVector();
	FVector EndForward = Start + (ForwardVector * 200.f);
	FVector LeftVector = ShipMeshComponent->GetRightVector();
	FVector EndLeft = Start + (LeftVector * -250.f);
	FVector RightVector = ShipMeshComponent->GetRightVector();
	FVector EndRight = Start + (RightVector * 250.f);

	FCollisionQueryParams CollisionParms;
	CollisionParms.AddIgnoredActor(this->GetOwner());

	//DrawDebugLine(GetWorld(), Start, EndForward, FColor::Green, false, 1, 0, 1);
	//DrawDebugLine(GetWorld(), Start, EndLeft, FColor::Blue, false, 1, 0, 1);
	//DrawDebugLine(GetWorld(), Start, EndRight, FColor::Red, false, 1, 0, 1);

	bool isHitForward = GetWorld()->LineTraceSingleByChannel(OutHit, Start, EndForward, ECC_Visibility, CollisionParms);
	bool isHitLeft = GetWorld()->LineTraceSingleByChannel(OutHit, Start, EndLeft, ECC_Visibility, CollisionParms);
	//bool isHitLeft = false;
	//bool isHitRight = GetWorld()->LineTraceSingleByChannel(OutHit, Start, EndRight, ECC_Visibility, CollisionParms);
	bool isHitRight = false;

	if (isHitForward) {
		//OutHit.GetActor()->Destroy();
		switch (currentDirectionPawn) {
		case DirectionPawn::Up:
			currentDirectionPawn = DirectionPawn::Left;
			if (isHitLeft) {
				currentDirectionPawn = DirectionPawn::Right;
			}
			if (isHitRight) {
				currentDirectionPawn = DirectionPawn::Down;
			}
			break;
		case DirectionPawn::Down:
			currentDirectionPawn = DirectionPawn::Right;
			if (isHitLeft) {
				currentDirectionPawn = DirectionPawn::Left;
			}
			if (isHitRight) {
				currentDirectionPawn = DirectionPawn::Up;
			}
			break;
		case DirectionPawn::Right:
			currentDirectionPawn = DirectionPawn::Up;
			if (isHitLeft) {
				currentDirectionPawn = DirectionPawn::Down;
			}
			if (isHitRight) {
				currentDirectionPawn = DirectionPawn::Left;
			}
			break;
		case DirectionPawn::Left:
			currentDirectionPawn = DirectionPawn::Down;
			if (isHitLeft) {
				currentDirectionPawn = DirectionPawn::Up;
			}
			if (isHitRight) {
				currentDirectionPawn = DirectionPawn::Right;
			}
			break;
		default:
			break;
		}
	}
}

void ALabyBotPawn::InitBattery() {
	BatteryLeft = MaxBattery;
	GetWorldTimerManager().SetTimer(TimeHandle_Battery, this, &ALabyBotPawn::UpdateBattery, 2.0f, true, 1.0f);
}

void ALabyBotPawn::UpdateBattery() {
	BatteryLeft--;

	if (BatteryLeft <= 0){
		//PrintString(FString::Printf(TEXT("No More Battery")));
		GetWorldTimerManager().ClearTimer(TimeHandle_Battery);
		PlayerEnd.Broadcast(false);
	}

}

void ALabyBotPawn::UpdateMaterial() {
	if (BatteryLeft > 7) {
		ShipMeshComponent->SetMaterial(0, MaterialOne);
		return;
	}
	if (BatteryLeft > 5) {
		ShipMeshComponent->SetMaterial(0, MaterialTwo);
		return;
	}
	if (BatteryLeft > 2) {
		ShipMeshComponent->SetMaterial(0, MaterialThree);
		return;
	}
	if (BatteryLeft > 0) {
		ShipMeshComponent->SetMaterial(0, MaterialFour);
		return;
	}
	ShipMeshComponent->SetMaterial(0, MaterialFive);

}

void ALabyBotPawn::SetDirectionPawn(DirectionPawn direction) {
	currentDirectionPawn = direction;
}

void ALabyBotPawn::Goal() {
	GetWorldTimerManager().ClearTimer(TimeHandle_Battery);
	PlayerEnd.Broadcast(true);
	MoveSpeed = 0.f;
}

int32 ALabyBotPawn::GetCurrentBattery()
{
	return BatteryLeft;
}
