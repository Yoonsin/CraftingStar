// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AmingHUD.generated.h"

/**
 * 
 */
UCLASS()
class CRAFTINGSTAR_API AAmingHUD : public AHUD
{
	GENERATED_BODY()

protected:
    // 화면 중앙에 그려질 것입니다.
    UPROPERTY(EditDefaultsOnly)
    UTexture2D* CrosshairTexture;

public:
    // HUD 에 대한 주요 드로 콜입니다.
    virtual void DrawHUD() override;
};
