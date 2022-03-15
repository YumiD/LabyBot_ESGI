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

const FName ALabyBotPawn::MoveForwardBinding("MoveForward");
const FName ALabyBotPawn::MoveRightBinding("MoveRight");
const FName ALabyBotPawn::FireForwardBinding("FireForward");
const FName ALabyBotPawn::FireRightBinding("FireRight");

enum DirectionPawn { Up, Down, Right, Left };
DirectionPawn currentDirectionPawn;

ALabyBotPawn::ALabyBotPawn()
{	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

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
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;

	currentDirectionPawn = Up;
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
	Raycast();

	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	//FString ForwardValueStr = FString::SanitizeFloat(ForwardValue);
	//UE_LOG(LogTemp, Warning, TEXT("Tick"));
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	//const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
	
	FVector MoveDirection;

	switch (currentDirectionPawn) {
		case Up:
			MoveDirection = FVector(1.f, 0.f, 0.f).GetClampedToMaxSize(1.0f);
			break;
		case Down:
			MoveDirection = FVector(-1.f, 0.f, 0.f).GetClampedToMaxSize(1.0f);
			break;
		case Right:
			MoveDirection = FVector(0.f, 1.f, 0.f).GetClampedToMaxSize(1.0f);
			break;
		case Left:
			MoveDirection = FVector(0.f, -1.f, 0.f).GetClampedToMaxSize(1.0f);
			break;
		default:
			MoveDirection = FVector(1.f, 0.f, 0.f).GetClampedToMaxSize(1.0f);
			break;
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
	
	// Create fire direction vector
	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);

	// Try and fire a shot
	FireShot(FireDirection);
}

void ALabyBotPawn::Raycast() {
	FHitResult OutHit;

	FVector Start = ShipMeshComponent->GetComponentLocation();
	FVector ForwardVector = ShipMeshComponent->GetForwardVector();
	FVector End = Start + (ForwardVector * 100.f);

	FCollisionQueryParams CollisionParms;
	CollisionParms.AddIgnoredActor(this->GetOwner());

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParms);

	UE_LOG(LogTemp, Warning, TEXT("RAYCAST"), TEXT(" RAYCAST2"));
	if (isHit) {
		//OutHit.GetActor()->Destroy();
		switch (currentDirectionPawn) {
		case Up:
			currentDirectionPawn = Right;
			break;
		case Down:
			currentDirectionPawn = Left;
			break;
		case Right:
			currentDirectionPawn = Down;
			break;
		case Left:
			currentDirectionPawn = Up;
			break;
		default:
			break;
		}
	}

}

void ALabyBotPawn::FireShot(FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				// spawn the projectile
				World->SpawnActor<ALabyBotProjectile>(SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ALabyBotPawn::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
}

void ALabyBotPawn::ShotTimerExpired()
{
	bCanFire = true;
}

