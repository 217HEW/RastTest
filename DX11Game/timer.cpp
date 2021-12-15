// 画面遷移 [Timer.cpp]
#include "Texture.h"
#include "polygon.h"
#include "number.h"
#include "timer.h"

// マクロ定義
#define PATH_TIMERFRAMETEXTURE L"data/texture/frame_timer.png"
#define TIMERFRAME_WIDTH 140.0f
#define TIMERFRAME_HEIGHT 80.0f
#define TIMERFRAME_POS_X 0.0f
#define TIMERFRAME_POS_Y (SCREEN_HEIGHT-TIMERFRAME_HEIGHT)*0.5f

#define TIMER_CHRCNT 3
#define TIMER_POS_X TIMERFRAME_POS_X-TIMER_CHRCNT*NUMBER_SIZE_X*0.5f
#define TIMER_POS_Y TIMERFRAME_POS_Y+16.0f

#define TIMER_START 30

// グローバル変数
static ID3D11ShaderResourceView* g_pTexture;
static int g_nTimer;
// 初期化
HRESULT InitTimer() 
{
	HRESULT hr = S_OK;

	ID3D11Device* pDevice = GetDevice();
	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_TIMERFRAMETEXTURE, &g_pTexture);
	if (FAILED(hr)) 
	{
		return hr;
	}
	// 変数初期化
	g_nTimer = TIMER_START * 60 + 59;
	return hr;
}
// 終了処理
void UninitTimer() 
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}
// 更新
void UpdateTimer() 
{
	if (g_nTimer > 0)
	{
		--g_nTimer;	}

}
// 描画
void DrawTimer() 
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	// 白か赤に設定
	if (g_nTimer < 10 * 60)
		SetPolygonColor(1.0f, 0.0f, 0.0f);//残り10秒で赤くなる
	else
		SetPolygonColor(1.0f, 1.0f, 1.0f);
	// タイマー枠表示
	SetPolygonSize(TIMERFRAME_WIDTH, TIMERFRAME_HEIGHT);
	SetPolygonPos(TIMERFRAME_POS_X, TIMERFRAME_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);	// タイマー表示
	DrawNumber(XMFLOAT2(TIMER_POS_X, TIMER_POS_Y),(unsigned)(g_nTimer / 60), TIMER_CHRCNT);
	// 元に戻す
	SetPolygonColor(1.0f, 1.0f, 1.0f);
}
// タイマー加算
void AddTimer(int nSec)
{
	g_nTimer += nSec * 60;
}

// タイマー取得
int GetTimer() 
{
	return g_nTimer / 60;}