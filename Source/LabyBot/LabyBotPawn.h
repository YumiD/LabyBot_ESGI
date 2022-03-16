// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LabyBotPawn.generated.h"

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

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Actor Interface

	void Raycast();

	void UpdateMaterial();

	void InitBattery();
	void UpdateBattery();

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
	static const FName FireRightBinding;

private:

	virtual void BeginPlay() override;

	/* Handle for Battery */
	FTimerHandle TimeHandle_Battery;
	int32 BatteryLeft;


public:
	/** Returns ShipMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
	/** Returns CameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};

