// 数値表示 [number.cpp]
#include "number.h"
#include "Texture.h"
#include "polygon.h"
// マクロ定義
#define PATH_NUMBERTEXTURE L"data/texture/number000.png"
#define NUMBER_COUNT_X 10
#define NUMBER_COUNT_Y 1
// グローバル変数
static ID3D11ShaderResourceView* g_pTexture;
// 初期化
HRESULT InitNumber()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_NUMBERTEXTURE, &g_pTexture);
	return hr;
}
// 終了処理
void UninitNumber()
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}
// 描画
	void DrawNumber(XMFLOAT2 vPos, // 表示位置(左上)
	unsigned uNumber, // 表示数値
	int nWidth, // 表示桁数
	float fSizeX,
	float fSizeY
	)
	{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(NUMBER_SIZE_X, NUMBER_SIZE_Y);
	SetPolygonTexture(g_pTexture);
	SetPolygonFrameSize(1.0f / NUMBER_COUNT_X,
		1.0f / NUMBER_COUNT_Y);
	// ①
	vPos.x += (nWidth - 0.5f) * NUMBER_SIZE_X;
	vPos.y -= NUMBER_SIZE_Y * 0.5f;
	// ⑤
	for (; nWidth > 0; --nWidth) 
	{
		// ②
		unsigned n = uNumber % 10;
		SetPolygonPos(vPos.x, vPos.y);
		SetPolygonUV
		(
			(n % NUMBER_COUNT_X) / (float)NUMBER_COUNT_X,
			(n / NUMBER_COUNT_X) / (float)NUMBER_COUNT_Y
		);
		DrawPolygon(pDC);
		// ③
		uNumber /= 10;
		// ④
		vPos.x -= NUMBER_SIZE_X;
	}
	// 元に戻す
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
}