// ��ʑJ�� [life.h]
#pragma once
#include "main.h"

// �֐��v���g�^�C�v
HRESULT InitLife(); // ������
void UninitLife(); // �I������
void UpdateLife(); // �X�V
void DrawLife(); // �`��

void AddLife(int nLife = 1);
void DelLife(int nLife = 1);
int GetLife();