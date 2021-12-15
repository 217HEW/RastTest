// 画面遷移 [Life.cpp]
#include "Texture.h"
#include "life.h"
#include "polygon.h"

// マクロ定義
#define PATH_LIFEFRAMETEXTURE L"data/texture/frame_life.png"
#define LIFEFRAME_WIDTH 200.0f
#define LIFEFRAME_HEIGHT 80.0f
#define LIFEFRAME_POS_X (SCREEN_WIDTH-LIFEFRAME_WIDTH)*-0.5f
#define LIFEFRAME_POS_Y (SCREEN_HEIGHT-LIFEFRAME_HEIGHT)*0.5f

#define PATH_LIFETEXTURE L"data/texture/hart.tga"
#define MAX_LIFE 3
#define LIFE_SIZE_X 40.0f
#define LIFE_SIZE_Y 40.0f
#define LIFE_POS_X LIFEFRAME_POS_X-LIFE_SIZE_X*(MAX_LIFE*0.5f-0.5f)
#define LIFE_POS_Y LIFEFRAME_POS_Y-6.0f

#define LIFE_COUNT_X	3
#define LIFE_COUNT_Y	4

#define TEX_FRAME 0
#define TEX_LIFE 1
#define MAX_TEXTURE 2

#define LIFE_START 3
//構造体定義
struct ANIM_PAT
{
	int nFrame;		//表示枠No.(-1で終了)
	int nCount;		//表示フレーム数
};

struct LIFE
{//敵情報
	XMFLOAT2 vPos;
	int nAnimFrame;	//表示枠No.
	int nFrameCount;		//表示フレーム数
	int nAnimPat;			//パターンNo.
	int nDir;				//方向(0:下,1:左,2:右)
};

// グローバル変数
static ANIM_PAT g_animPat[4][5] =
{
	//左側がアニメーションカウント
	//右側がフレームカウント
		{{ 0, 5}, { 0, 2}, { 0, 5}, { 0, 2}, {-1, -1}},
		{{ 4, 5}, { 4, 2}, { 4, 5}, { 4, 2}, {-1, -1}},
		{{ 8, 5}, { 8, 2}, {8, 5}, { 8, 2}, {-1, -1}},
		{{12, 5}, {12, 2}, {12, 5}, {12, 2}, {-1, -1}},

};

static ID3D11ShaderResourceView* g_pTexture[MAX_TEXTURE];
static int g_nLife;



// 初期化
HRESULT InitLife() 
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	LPCWSTR pszTexFName[] = { PATH_LIFEFRAMETEXTURE, PATH_LIFETEXTURE };
	for (int i = 0; i < MAX_TEXTURE; ++i) 
	{
		hr = CreateTextureFromFile(pDevice, pszTexFName[i], &g_pTexture[i]);
		if (FAILED(hr)) 
		{
			return hr;
		}
	}
	// 変数初期化
	g_nLife = LIFE_START;
	return hr;
}

// 終了処理
void UninitLife()
{
	// テクスチャ解放
	for (int i = 0; i < MAX_TEXTURE; ++i) 
	{
		SAFE_RELEASE(g_pTexture[i]);
	}
}

void UpdateLife()
{

}

// 描画
void DrawLife() 
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	// テクスチャと同色に設定
	SetPolygonColor(0.929412f, 0.152941f, 0.564706f);

	// ライフ枠表示
	SetPolygonSize(LIFEFRAME_WIDTH, LIFEFRAME_HEIGHT);
	SetPolygonPos(LIFEFRAME_POS_X, LIFEFRAME_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_FRAME]);
	DrawPolygon(pDC);

	// 元に戻す
	SetPolygonColor(1.0f, 1.0f, 1.0f);

	// ライフ表示
	SetPolygonSize(LIFE_SIZE_X, LIFE_SIZE_Y);
	SetPolygonTexture(g_pTexture[TEX_LIFE]);

	//Lifeをアニメーションさせるには、UV,FramSizeが必要
	for(int i = 0; i < g_nLife; ++i) 
	{
		SetPolygonPos(LIFE_POS_X + i * LIFE_SIZE_X, LIFE_POS_Y);
		DrawPolygon(pDC);
	}



}
// 切替
// ライフ加算
void AddLife(int nLife)
{
	g_nLife += nLife;
	if (g_nLife > MAX_LIFE) 
	{
		g_nLife = MAX_LIFE;
	}
}
// ライフ減算
void DelLife(int nLife)
{
	g_nLife -= nLife;
	if (g_nLife < 0) 
	{
		g_nLife = 0;
	}
}
// ライフ取得
int GetLife() 
{
	return g_nLife;
}