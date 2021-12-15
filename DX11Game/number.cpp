// ���l�\�� [number.cpp]
#include "number.h"
#include "Texture.h"
#include "polygon.h"
// �}�N����`
#define PATH_NUMBERTEXTURE L"data/texture/number000.png"
#define NUMBER_COUNT_X 10
#define NUMBER_COUNT_Y 1
// �O���[�o���ϐ�
static ID3D11ShaderResourceView* g_pTexture;
// ������
HRESULT InitNumber()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_NUMBERTEXTURE, &g_pTexture);
	return hr;
}
// �I������
void UninitNumber()
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}
// �`��
	void DrawNumber(XMFLOAT2 vPos, // �\���ʒu(����)
	unsigned uNumber, // �\�����l
	int nWidth, // �\������
	float fSizeX,
	float fSizeY
	)
	{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(NUMBER_SIZE_X, NUMBER_SIZE_Y);
	SetPolygonTexture(g_pTexture);
	SetPolygonFrameSize(1.0f / NUMBER_COUNT_X,
		1.0f / NUMBER_COUNT_Y);
	// �@
	vPos.x += (nWidth - 0.5f) * NUMBER_SIZE_X;
	vPos.y -= NUMBER_SIZE_Y * 0.5f;
	// �D
	for (; nWidth > 0; --nWidth) 
	{
		// �A
		unsigned n = uNumber % 10;
		SetPolygonPos(vPos.x, vPos.y);
		SetPolygonUV
		(
			(n % NUMBER_COUNT_X) / (float)NUMBER_COUNT_X,
			(n / NUMBER_COUNT_X) / (float)NUMBER_COUNT_Y
		);
		DrawPolygon(pDC);
		// �B
		uNumber /= 10;
		// �C
		vPos.x -= NUMBER_SIZE_X;
	}
	// ���ɖ߂�
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
}