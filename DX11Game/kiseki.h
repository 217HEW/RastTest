//=============================================================================
//
// ������ [Kiseki.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitKiseki(void);
void UninitKiseki(void);
void UpdateKiseki(void);
void DrawKiseki(void);

int SetKiseki(XMFLOAT3 pos, XMFLOAT2 size);
