// ��ʑJ�� [timer.h]
#pragma once
#include "main.h"

// �֐��v���g�^�C�v
HRESULT InitTimer(); // ������
void UninitTimer(); // �I������
void UpdateTimer(); // �X�V
void DrawTimer(); // �`��

void AddTimer(int nSec = 30);// �ؑ�_�g���ĂȂ�
int GetTimer();