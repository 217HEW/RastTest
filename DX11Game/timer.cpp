// ��ʑJ�� [Timer.cpp]
#include "Texture.h"
#include "polygon.h"
#include "number.h"
#include "timer.h"

// �}�N����`
#define PATH_TIMERFRAMETEXTURE L"data/texture/frame_timer.png"
#define TIMERFRAME_WIDTH 140.0f
#define TIMERFRAME_HEIGHT 80.0f
#define TIMERFRAME_POS_X 0.0f
#define TIMERFRAME_POS_Y (SCREEN_HEIGHT-TIMERFRAME_HEIGHT)*0.5f

#define TIMER_CHRCNT 3
#define TIMER_POS_X TIMERFRAME_POS_X-TIMER_CHRCNT*NUMBER_SIZE_X*0.5f
#define TIMER_POS_Y TIMERFRAME_POS_Y+16.0f

#define TIMER_START 30

// �O���[�o���ϐ�
static ID3D11ShaderResourceView* g_pTexture;
static int g_nTimer;
// ������
HRESULT InitTimer() 
{
	HRESULT hr = S_OK;

	ID3D11Device* pDevice = GetDevice();
	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_TIMERFRAMETEXTURE, &g_pTexture);
	if (FAILED(hr)) 
	{
		return hr;
	}
	// �ϐ�������
	g_nTimer = TIMER_START * 60 + 59;
	return hr;
}
// �I������
void UninitTimer() 
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}
// �X�V
void UpdateTimer() 
{
	if (g_nTimer > 0)
	{
		--g_nTimer;	}

}
// �`��
void DrawTimer() 
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	// �����Ԃɐݒ�
	if (g_nTimer < 10 * 60)
		SetPolygonColor(1.0f, 0.0f, 0.0f);//�c��10�b�ŐԂ��Ȃ�
	else
		SetPolygonColor(1.0f, 1.0f, 1.0f);
	// �^�C�}�[�g�\��
	SetPolygonSize(TIMERFRAME_WIDTH, TIMERFRAME_HEIGHT);
	SetPolygonPos(TIMERFRAME_POS_X, TIMERFRAME_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);	// �^�C�}�[�\��
	DrawNumber(XMFLOAT2(TIMER_POS_X, TIMER_POS_Y),(unsigned)(g_nTimer / 60), TIMER_CHRCNT);
	// ���ɖ߂�
	SetPolygonColor(1.0f, 1.0f, 1.0f);
}
// �^�C�}�[���Z
void AddTimer(int nSec)
{
	g_nTimer += nSec * 60;
}

// �^�C�}�[�擾
int GetTimer() 
{
	return g_nTimer / 60;}