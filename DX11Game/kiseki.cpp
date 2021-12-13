//=============================================================================
//
// ������ [kiseki.cpp]
// Author : HIROHIKO HAMAYA
//	kiseki�̐��l�ݒ�ŃG�t�F�N�g���邩��
//
//=============================================================================
#include "kiseki.h"
#include "Texture.h"
#include "mesh.h"
#include "shadow.h"
#include "Camera.h"
#include "Light.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_KISEKI		L"data/texture/Particle01.tga"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	KISEKI_SIZE_X		(150.0f)							// �r���{�[�h�̕�
#define	KISEKI_SIZE_Y		(150.0f)							// �r���{�[�h�̍���

#define	MAX_KISEKI			(512)							// �r���{�[�h�ő吔

#define MATERIAL_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define MATERIAL_AMBIENT		XMFLOAT4(0,0,0,1)
#define MATERIAL_SPECULAR		XMFLOAT4(0,0,0,1)
#define MATERIAL_EMISSIVE		XMFLOAT4(0,0,0,1)
#define MATERIAL_POWER			(1.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct TKiseki 
{
	XMFLOAT4X4	mtxWorld;	// ���[���h�}�g���b�N�X
	XMFLOAT3	pos;		// �ʒu
	XMFLOAT3	vel;		// ���x
	XMFLOAT4	col;		// �F
	XMFLOAT2	size;		// �傫��
	float		fAlpha;		// �A���t�@�l
	bool		use;		// �g�p���Ă��邩�ǂ���
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexKiseki(ID3D11Device* pDevice);
void SetVertexKiseki(int nIdxBullet, XMFLOAT2 size);
void SetColorKiseki(int nIdxKiseki, XMFLOAT4 col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH			g_mesh;
static MATERIAL		g_material;
static TKiseki		g_kiseki[MAX_KISEKI];	// �O�Ճ��[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitKiseki(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	ZeroMemory(&g_mesh, sizeof(MESH));

	// ���_���̍쐬
	MakeVertexKiseki(pDevice);

	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
							   TEXTURE_KISEKI,		// �t�@�C����
							   &g_mesh.pTexture);	// �ǂݍ��ރ�����
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	// �}�e���A���̐ݒ�
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
// �I������
//=============================================================================
void UninitKiseki(void)
{
	// ���b�V���̊J��
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateKiseki(void)
{
	TKiseki* pKiseki = g_kiseki;
	for (int i = 0; i < MAX_KISEKI; ++i, ++pKiseki) 
	{
		if (pKiseki->use)
		{
			// �ړ�
			pKiseki->pos.x += pKiseki->vel.x;
			pKiseki->pos.y += pKiseki->vel.y;
			pKiseki->pos.z += pKiseki->vel.z;

			// ���_���W�̐ݒ�
			pKiseki->size.x += 0.1f;
			pKiseki->size.y += 0.1f;
			SetVertexKiseki(i, pKiseki->size);

			// ���_�J���[�̐ݒ�
			pKiseki->fAlpha -= 0.002f;
			if (pKiseki->fAlpha < 0.0f) 
			{
				pKiseki->fAlpha = 0.0f;
				pKiseki->use = false;
				continue;
			}
			//�F�ύX
			SetColorKiseki(i, XMFLOAT4(1.0f, 0.6f, 0.0f, pKiseki->fAlpha));
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawKiseki(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	// �r���[�}�g���b�N�X���擾
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();

	CLight::Get()->SetDisable();
	//SetBlendState(BS_SUBTRACTION);	// ���Z����
	SetBlendState(BS_ALPHABLEND);		// ���Z����
	SetZWrite(false);	// �������`���Z�o�b�t�@���X�V���Ȃ�(Z�`�F�b�N�͍s��)
	TKiseki* pKiseki = g_kiseki;
	for (int i = 0; i < MAX_KISEKI; ++i, ++pKiseki) 
	{
		if (pKiseki->use) 
		{
			// ���[���h�}�g���b�N�X�̏�����
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

			// �X�P�[���𔽉f
			mtxScale = XMMatrixScaling(pKiseki->size.x, pKiseki->size.y, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(pKiseki->pos.x, pKiseki->pos.y, pKiseki->pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			XMStoreFloat4x4(&pKiseki->mtxWorld, mtxWorld);
			g_mesh.mtxWorld = pKiseki->mtxWorld;

			// �J���[�̐ݒ�
			g_material.Diffuse = pKiseki->col;

			// ���b�V���̕`��
			DrawMesh(pDeviceContext, &g_mesh);
		}
	}
	SetZWrite(true);
	SetBlendState(BS_NONE);
	CLight::Get()->SetEnable();
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexKiseki(ID3D11Device* pDevice)
{
	// �I�u�W�F�N�g�̒��_�z��𐶐�
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	// ���_�z��̒��g�𖄂߂�
	VERTEX_3D* pVtx = pVertexWk;

	// ���_���W�̐ݒ�
	pVtx[0].vtx = XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);
	pVtx[1].vtx = XMFLOAT3(-1.0f / 2,  1.0f / 2, 0.0f);
	pVtx[2].vtx = XMFLOAT3( 1.0f / 2, -1.0f / 2, 0.0f);
	pVtx[3].vtx = XMFLOAT3( 1.0f / 2,  1.0f / 2, 0.0f);

	// �@���̐ݒ�
	pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// ���ˌ��̐ݒ�
	pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = XMFLOAT2(0.0f, 1.0f);
	pVtx[1].tex = XMFLOAT2(0.0f, 0.0f);
	pVtx[2].tex = XMFLOAT2(1.0f, 1.0f);
	pVtx[3].tex = XMFLOAT2(1.0f, 0.0f);

	// �C���f�b�N�X�z��𐶐�
	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[g_mesh.nNumIndex];

	// �C���f�b�N�X�z��̒��g�𖄂߂�
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// ���_�o�b�t�@����
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	// �ꎞ�z��̉��
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexKiseki(int nIdxKiseki, XMFLOAT2 size)
{
	if (nIdxKiseki >= 0 && nIdxKiseki < MAX_KISEKI) {
		TKiseki* pKiseki = &g_kiseki[nIdxKiseki];
		pKiseki->size = size;
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorKiseki(int nIdxKiseki, XMFLOAT4 col)
{
	if (nIdxKiseki >= 0 && nIdxKiseki < MAX_KISEKI) {
		g_kiseki[nIdxKiseki].col = col;
	}
}

//=============================================================================
// ���_���̍쐬
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

			// ���_���W�̐ݒ�
			SetVertexKiseki(i, size);

			nIdxKiseki = i;
			break;
		}
	}

	return nIdxKiseki;
}
