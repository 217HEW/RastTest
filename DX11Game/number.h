//背景描画[Number.h]
#pragma once
#include "main.h"

#define NUMBER_SIZE_X 32
#define NUMBER_SIZE_Y 32

//関数のプロトタイプ
HRESULT InitNumber();	//初期化
void UninitNumber();	//終了処理
void DrawNumber(XMFLOAT2 vPos,unsigned,int nWidth, float fSizeX = NUMBER_SIZE_X,float fSizeY = NUMBER_SIZE_Y);		//描画

