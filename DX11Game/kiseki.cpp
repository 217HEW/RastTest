//=============================================================================
//
// 煙処理 [kiseki.cpp]
// Author : HIROHIKO HAMAYA
//	kisekiの数値設定でエフェクト作れるかも
//
//=============================================================================
#include "kiseki.h"
#include "Texture.h"
#include "mesh.h"
#include "shadow.h"
#include "Camera.h"
#include "Light.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_KISEKI		L"data/texture/Particle01.tga"	// 読み込むテクスチャファイル名
#define	KISEKI_SIZE_X		(150.0f)							// ビルボードの幅
#define	KISEKI_SIZE_Y		(150.0f)							// ビルボードの高さ

#define	MAX_KISEKI			(512)							// ビルボード最大数

#define MATERIAL_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define MATERIAL_AMBIENT		XMFLOAT4(0,0,0,1)
#define MATERIAL_SPECULAR		XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE		XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER			(1.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct TKiseki 
{
	XMFLOAT4X4	mtxWorld;	// ワールドマトリックス
	XMFLOAT3	pos;		// 位置
	XMFLOAT3	vel;		// 速度
	XMFLOAT4	col;		// 色
	XMFLOAT2	size;		// 大きさ
	float		fAlpha;		// アルファ値
	bool		use;		// 使用しているかどうか
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexKiseki(ID3D11Device* pDevice);
void SetVertexKiseki(int nIdxBullet, XMFLOAT2 size);
void SetColorKiseki(int nIdxKiseki, XMFLOAT4 col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MESH			g_mesh;
static MATERIAL		g_material;
static TKiseki		g_kiseki[MAX_KISEKI];	// 軌跡ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitKiseki(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	ZeroMemory(&g_mesh, sizeof(MESH));

	// 頂点情報の作成
	MakeVertexKiseki(pDevice);

	// テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice,				// デバイスへのポインタ
							   TEXTURE_KISEKI,		// ファイル名
							   &g_mesh.pTexture);	// 読み込むメモリ
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	// マテリアルの設定
	g_material.Diffuse = MATERIAL_DIFFUSE;
	g_material.Ambient = MATERIAL_AMBIENT;
	g_material.Specular = MATERIAL_SPECULAR;
	g_material.Emissive = MATERIAL_EMISSIVE;
	g_material.Power = MATERIAL_POWER;
	g_mesh.pMaterial = &g_material;

	TKiseki* pKiseki = g_kiseki;
	for (int i = 0; i < MAX_KISEKI; ++i, ++pKiseki) 
	{
		pKiseki->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		pKiseki->col = XMFLOAT4(0.1f, 0.0f, 0.0f, 1.0f);
		pKiseki->size = XMFLOAT2(KISEKI_SIZE_X, KISEKI_SIZE_Y);
		pKiseki->fAlpha = 1.0f;
		pKiseki->use = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitKiseki(void)
{
	// メッシュの開放
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateKiseki(void)
{
	TKiseki* pKiseki = g_kiseki;
	for (int i = 0; i < MAX_KISEKI; ++i, ++pKiseki) 
	{
		if (pKiseki->use)
		{
			// 移動
			pKiseki->pos.x += pKiseki->vel.x;
			pKiseki->pos.y += pKiseki->vel.y;
			pKiseki->pos.z += pKiseki->vel.z;

			// 頂点座標の設定
			pKiseki->size.x += 0.1f;
			pKiseki->size.y += 0.1f;
			SetVertexKiseki(i, pKiseki->size);

			// 頂点カラーの設定
			pKiseki->fAlpha -= 0.002f;
			if (pKiseki->fAlpha < 0.0f) 
			{
				pKiseki->fAlpha = 0.0f;
				pKiseki->use = false;
				continue;
			}
			//色変更
			SetColorKiseki(i, XMFLOAT4(1.0f, 0.6f, 0.0f, pKiseki->fAlpha));
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawKiseki(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// ビューマトリックスを取得
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();

	CLight::Get()->SetDisable();
	//SetBlendState(BS_SUBTRACTION);	// 減算合成
	SetBlendState(BS_ALPHABLEND);		// 加算合成
	SetZWrite(false);	// 半透明描画はZバッファを更新しない(Zチェックは行う)
	TKiseki* pKiseki = g_kiseki;
	for (int i = 0; i < MAX_KISEKI; ++i, ++pKiseki) 
	{
		if (pKiseki->use) 
		{
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(&pKiseki->mtxWorld, mtxWorld);

			pKiseki->mtxWorld._11 = mtxView._11;
			pKiseki->mtxWorld._12 = mtxView._21;
			pKiseki->mtxWorld._13 = mtxView._31;
			pKiseki->mtxWorld._21 = mtxView._12;
			pKiseki->mtxWorld._22 = mtxView._22;
			pKiseki->mtxWorld._23 = mtxView._32;
			pKiseki->mtxWorld._31 = mtxView._13;
			pKiseki->mtxWorld._32 = mtxView._23;
			pKiseki->mtxWorld._33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(&pKiseki->mtxWorld);

			// スケールを反映
			mtxScale = XMMatrixScaling(pKiseki->size.x, pKiseki->size.y, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(pKiseki->pos.x, pKiseki->pos.y, pKiseki->pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			XMStoreFloat4x4(&pKiseki->mtxWorld, mtxWorld);
			g_mesh.mtxWorld = pKiseki->mtxWorld;

			// カラーの設定
			g_material.Diffuse = pKiseki->col;

			// メッシュの描画
			DrawMesh(pDeviceContext, &g_mesh);
		}
	}
	SetZWrite(true);
	SetBlendState(BS_NONE);
	CLight::Get()->SetEnable();
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexKiseki(ID3D11Device* pDevice)
{
	// オブジェクトの頂点配列を生成
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	// 頂点配列の中身を埋める
	VERTEX_3D* pVtx = pVertexWk;

	// 頂点座標の設定
	pVtx[0].vtx = XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);
	pVtx[1].vtx = XMFLOAT3(-1.0f / 2,  1.0f / 2, 0.0f);
	pVtx[2].vtx = XMFLOAT3( 1.0f / 2, -1.0f / 2, 0.0f);
	pVtx[3].vtx = XMFLOAT3( 1.0f / 2,  1.0f / 2, 0.0f);

	// 法線の設定
	pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// 反射光の設定
	pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = XMFLOAT2(0.0f, 1.0f);
	pVtx[1].tex = XMFLOAT2(0.0f, 0.0f);
	pVtx[2].tex = XMFLOAT2(1.0f, 1.0f);
	pVtx[3].tex = XMFLOAT2(1.0f, 0.0f);

	// インデックス配列を生成
	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[g_mesh.nNumIndex];

	// インデックス配列の中身を埋める
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// 頂点バッファ生成
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	// 一時配列の解放
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexKiseki(int nIdxKiseki, XMFLOAT2 size)
{
	if (nIdxKiseki >= 0 && nIdxKiseki < MAX_KISEKI) {
		TKiseki* pKiseki = &g_kiseki[nIdxKiseki];
		pKiseki->size = size;
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorKiseki(int nIdxKiseki, XMFLOAT4 col)
{
	if (nIdxKiseki >= 0 && nIdxKiseki < MAX_KISEKI) {
		g_kiseki[nIdxKiseki].col = col;
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetKiseki(XMFLOAT3 pos, XMFLOAT2 size)
{
	int nIdxKiseki = -1;

	TKiseki* pKiseki = g_kiseki;
	for (int i = 0; i < MAX_KISEKI; ++i, ++pKiseki) {
		if (!pKiseki->use) 
		{
			pKiseki->pos = pos;
			XMMATRIX mRot = XMMatrixRotationY(XMConvertToRadians((float)(rand() % 360)));
			XMVECTOR vAxis = XMVector3TransformNormal(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), mRot);
			XMStoreFloat3(&pKiseki->vel, XMVector3TransformNormal(XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f), XMMatrixRotationAxis(vAxis, XMConvertToRadians(10))));
			pKiseki->col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			pKiseki->fAlpha = 0.2f;
			pKiseki->use = true;

			// 頂点座標の設定
			SetVertexKiseki(i, size);

			nIdxKiseki = i;
			break;
		}
	}

	return nIdxKiseki;
}
