//�w�i�`��[Number.h]
#pragma once
#include "main.h"

#define NUMBER_SIZE_X 32
#define NUMBER_SIZE_Y 32

//�֐��̃v���g�^�C�v
HRESULT InitNumber();	//������
void UninitNumber();	//�I������
void DrawNumber(XMFLOAT2 vPos,unsigned,int nWidth, float fSizeX = NUMBER_SIZE_X,float fSizeY = NUMBER_SIZE_Y);		//�`��

