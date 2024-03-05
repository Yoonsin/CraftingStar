// Fill out your copyright notice in the Description page of Project Settings.


#include "AmingHUD.h"
#include "Engine/Canvas.h"

void AAmingHUD::DrawHUD()
{
    Super::DrawHUD();

    if ( CrosshairTexture )
    {
        // 캔버스 중심을 찾습니다.
        FVector2D Center(Canvas->ClipX * 0.5f , Canvas->ClipY * 0.5f);

        // 텍스처 중심이 캔버스 중심에 맞도록 텍스처의 크기 절반 만큼 오프셋을 줍니다.
        FVector2D CrossHairDrawPosition(Center.X - ( CrosshairTexture->GetSurfaceWidth() * 0.5f ) , Center.Y - ( CrosshairTexture->GetSurfaceHeight() * 0.5f ));

        // 중심점에 조준선을 그립니다.
        FCanvasTileItem TileItem(CrossHairDrawPosition , CrosshairTexture->Resource , FLinearColor::White);
        TileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(TileItem);
    }
}