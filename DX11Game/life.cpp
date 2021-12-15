// ��ʑJ�� [Life.cpp]
#include "Texture.h"
#include "life.h"
#include "polygon.h"

// �}�N����`
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
//�\���̒�`
struct ANIM_PAT
{
	int nFrame;		//�\���gNo.(-1�ŏI��)
	int nCount;		//�\���t���[����
};

struct LIFE
{//�G���
	XMFLOAT2 vPos;
	int nAnimFrame;	//�\���gNo.
	int nFrameCount;		//�\���t���[����
	int nAnimPat;			//�p�^�[��No.
	int nDir;				//����(0:��,1:��,2:�E)
};

// �O���[�o���ϐ�
static ANIM_PAT g_animPat[4][5] =
{
	//�������A�j���[�V�����J�E���g
	//�E�����t���[���J�E���g
		{{ 0, 5}, { 0, 2}, { 0, 5}, { 0, 2}, {-1, -1}},
		{{ 4, 5}, { 4, 2}, { 4, 5}, { 4, 2}, {-1, -1}},
		{{ 8, 5}, { 8, 2}, {8, 5}, { 8, 2}, {-1, -1}},
		{{12, 5}, {12, 2}, {12, 5}, {12, 2}, {-1, -1}},

};

static ID3D11ShaderResourceView* g_pTexture[MAX_TEXTURE];
static int g_nLife;



// ������
HRESULT InitLife() 
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���Ǎ�
	LPCWSTR pszTexFName[] = { PATH_LIFEFRAMETEXTURE, PATH_LIFETEXTURE };
	for (int i = 0; i < MAX_TEXTURE; ++i) 
	{
		hr = CreateTextureFromFile(pDevice, pszTexFName[i], &g_pTexture[i]);
		if (FAILED(hr)) 
		{
			return hr;
		}
	}
	// �ϐ�������
	g_nLife = LIFE_START;
	return hr;
}

// �I������
void UninitLife()
{
	// �e�N�X�`�����
	for (int i = 0; i < MAX_TEXTURE; ++i) 
	{
		SAFE_RELEASE(g_pTexture[i]);
	}
}

void UpdateLife()
{

}

// �`��
void DrawLife() 
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	// �e�N�X�`���Ɠ��F�ɐݒ�
	SetPolygonColor(0.929412f, 0.152941f, 0.564706f);

	// ���C�t�g�\��
	SetPolygonSize(LIFEFRAME_WIDTH, LIFEFRAME_HEIGHT);
	SetPolygonPos(LIFEFRAME_POS_X, LIFEFRAME_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_FRAME]);
	DrawPolygon(pDC);

	// ���ɖ߂�
	SetPolygonColor(1.0f, 1.0f, 1.0f);

	// ���C�t�\��
	SetPolygonSize(LIFE_SIZE_X, LIFE_SIZE_Y);
	SetPolygonTexture(g_pTexture[TEX_LIFE]);

	//Life���A�j���[�V����������ɂ́AUV,FramSize���K�v
	for(int i = 0; i < g_nLife; ++i) 
	{
		SetPolygonPos(LIFE_POS_X + i * LIFE_SIZE_X, LIFE_POS_Y);
		DrawPolygon(pDC);
	}



}
// �ؑ�
// ���C�t���Z
void AddLife(int nLife)
{
	g_nLife += nLife;
	if (g_nLife > MAX_LIFE) 
	{
		g_nLife = MAX_LIFE;
	}
}
// ���C�t���Z
void DelLife(int nLife)
{
	g_nLife -= nLife;
	if (g_nLife < 0) 
	{
		g_nLife = 0;
	}
}
// ���C�t�擾
int GetLife() 
{
	return g_nLife;
}