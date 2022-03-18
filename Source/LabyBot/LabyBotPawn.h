// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LabyBotPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnd, bool, isVictory);

UENUM(BlueprintType)
enum class DirectionPawn : uint8
{
	None UMETA(DisplayName = "None"),
	Left UMETA(DisplayName = "Left"),
	Up UMETA(DisplayName = "Up"),
	Right UMETA(DisplayName = "Right"),
	Down UMETA(DisplayName = "Down")
};

UCLASS(Blueprintable)
class ALabyBotPawn : public APawn
{
	GENERATED_BODY()

		/* The mesh component */
		UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ShipMeshComponent;

	/** The camera */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/* Materials */
	UPROPERTY(EditAnywhere, Category = "Materials")
		UMaterialInterface* MaterialOne;
	UPROPERTY(EditAnywhere, Category = "Materials")
		UMaterialInterface* MaterialTwo;
	UPROPERTY(EditAnywhere, Category = "Materials")
		UMaterialInterface* MaterialThree;
	UPROPERTY(EditAnywhere, Category = "Materials")
		UMaterialInterface* MaterialFour;
	UPROPERTY(EditAnywhere, Category = "Materials")
		UMaterialInterface* MaterialFive;

public:
	ALabyBotPawn();

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float MoveSpeed;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		int32 MaxBattery = 10;

	bool Started = false;
	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Actor Interface

	void Raycast();

	void UpdateMaterial();

	void InitBattery();
	void UpdateBattery();

	void SetDirectionPawn(DirectionPawn direction);

	void Goal();
	int32 GetCurrentBattery();

	UPROPERTY(BlueprintAssignable, Category = "Event")
		FOnEnd PlayerEnd;

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
	static const FName FireRightBinding;

private:
	/* Handle for Battery */
	FTimerHandle TimeHandle_Battery;
	int32 BatteryLeft;
	bool BatteryInitialized;

	DirectionPawn currentDirectionPawn;

public:
	/** Returns ShipMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
	/** Returns CameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};

