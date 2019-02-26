//=============================================================================
//
// �n�ʏ��� [terrain.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "terrain.h"
#include <vector>



//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DIRECTORY_TEXTURE       "data/TEXTURE/"
#define	TEXTURE_FIELD	"data/TEXTURE/field000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	MAP_FILE		 "data/MODEL/room.x"	// �ǂݍ��ރ��f���t�@�C����
#define TERRAIN_SIZE_X 20
#define TERRAIN_SIZE_Y 20
#define TERRAIN_WIDTH 800
#define TERRAIN_HEIGHT 800
#define TERRAIN_POS_X -500
#define TERRAIN_POS_Y -100




//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTerrain(LPDIRECT3DDEVICE9 pDevice);

void generateTerrain();

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTerrain = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTerrain = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
struct TextureInfo {
	char name[256];
	LPDIRECT3DTEXTURE9 g_pD3DTexturePlayer;
};
struct Map {
	std::vector<TextureInfo> textures;		// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			g_pD3DXMeshPlayer;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		g_pD3DXBuffMatPlayer;		// �}�e���A�����ւ̃|�C���^
	DWORD				g_nNumMatPlayer;// �}�e���A�����̐�
	std::vector<int> textureIndex;
};


D3DXMATRIX				g_mtxWorldTerrain;			// ���[���h�}�g���b�N�X
D3DXVECTOR3				g_posTerrain;					// ���݂̈ʒu
D3DXVECTOR3				g_rotTerrain;					// ���݂̌���
D3DXVECTOR3				g_scaleTerrain;					// ���݂̊g��
VERTEX_3D terrain[TERRAIN_WIDTH * TERRAIN_HEIGHT];
Map map;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitTerrain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HRESULT hr;

	// �ʒu�A�����̏����ݒ�
	g_posTerrain = D3DXVECTOR3(0.0f, 13.0f, 0.0f);
	g_rotTerrain = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_scaleTerrain = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// �e�N�X�`���̓ǂݍ���
	//hr = D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
	//	TEXTURE_FIELD,			// �t�@�C���̖��O
	//	&g_pD3DTextureTerrain);	// �ǂݍ��ރ������[

	//							// ���_���̍쐬
	hr = MakeVertexTerrain(pDevice);

	if (FAILED(D3DXLoadMeshFromX(MAP_FILE,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&map.g_pD3DXBuffMatPlayer,
		NULL,
		&map.g_nNumMatPlayer,
		&map.g_pD3DXMeshPlayer)))
	{
		return E_FAIL;
	}

	D3DXMATERIAL *pD3DXMat;
	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)map.g_pD3DXBuffMatPlayer->GetBufferPointer();


	map.textureIndex.resize((int)map.g_nNumMatPlayer);
	for (int nCntMat = 0; nCntMat < (int)map.g_nNumMatPlayer; nCntMat++)
	{
		if (pD3DXMat[nCntMat].pTextureFilename == NULL) {
			continue;
		}
		bool haveTexture = false;
		int index = -1;
		for (unsigned int i = 0; i < map.textures.size(); i++) {
			if (std::strcmp(pD3DXMat[nCntMat].pTextureFilename,map.textures[i].name) == 0) {
				haveTexture = true;
				index = i;
			}
		}
		
		if (haveTexture) {
			map.textureIndex[nCntMat] = index;
		}
		else {
			TextureInfo textureInfo;
			char result[100];
			std::strcpy(result, (LPSTR)DIRECTORY_TEXTURE);
			std::strcat(result, pD3DXMat[nCntMat].pTextureFilename);
			hr = D3DXCreateTextureFromFile(pDevice, result, &textureInfo.g_pD3DTexturePlayer);
			std::strcpy(textureInfo.name, pD3DXMat[nCntMat].pTextureFilename);
			map.textures.push_back(textureInfo);
			map.textureIndex[nCntMat] = map.textures.size()-1;
		}
	}
	
	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTerrain(void)
{
	if (g_pD3DTextureTerrain != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureTerrain->Release();
		g_pD3DTextureTerrain = NULL;
	}

	if (g_pD3DVtxBuffTerrain != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffTerrain->Release();
		g_pD3DVtxBuffTerrain = NULL;
	}
	for(unsigned int i = 0; i < map.textures.size(); i++) {
		if (map.textures[i].g_pD3DTexturePlayer != NULL) {
			map.textures[i].g_pD3DTexturePlayer->Release();
			map.textures[i].g_pD3DTexturePlayer = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTerrain(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTerrain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxScale;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldTerrain);
	//�g��𔽉f
	D3DXMatrixScaling(&mtxScale, g_scaleTerrain.x, g_scaleTerrain.y, g_scaleTerrain.z);
	D3DXMatrixMultiply(&g_mtxWorldTerrain, &g_mtxWorldTerrain, &mtxScale);
	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotTerrain.y, g_rotTerrain.x, g_rotTerrain.z);
	D3DXMatrixMultiply(&g_mtxWorldTerrain, &g_mtxWorldTerrain, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_posTerrain.x, g_posTerrain.y, g_posTerrain.z);
	D3DXMatrixMultiply(&g_mtxWorldTerrain, &g_mtxWorldTerrain, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldTerrain);

	D3DXMATERIAL *pD3DXMat;
	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)map.g_pD3DXBuffMatPlayer->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)map.g_nNumMatPlayer; nCntMat++)
	{
		pD3DXMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
		pD3DXMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pD3DXMat[nCntMat].MatD3D.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, map.textures[map.textureIndex[nCntMat]].g_pD3DTexturePlayer);

		// �`��
		map.g_pD3DXMeshPlayer->DrawSubset(nCntMat);
	}

	{// �}�e���A�����f�t�H���g�ɖ߂�
		D3DXMATERIAL mat;

		mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		pDevice->SetMaterial(&mat.MatD3D);
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTerrain(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffTerrain,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffTerrain->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
		pVtx[1].vtx = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
		pVtx[2].vtx = D3DXVECTOR3(-100.0f, 0.0f, -100.0f);
		pVtx[3].vtx = D3DXVECTOR3(100.0f, 0.0f, -100.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffTerrain->Unlock();
	}

	return S_OK;
}
void generateTerrain() {

	int x = TERRAIN_POS_X;
	int y = TERRAIN_POS_Y;

	for (int i = 0; i < TERRAIN_HEIGHT; i++) {
		for (int j = 0; j < TERRAIN_WIDTH; i++) {
			// ���_���W�̐ݒ�
			terrain[j].vtx = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
			terrain[j+1].vtx = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
			terrain[j+2].vtx = D3DXVECTOR3(-100.0f, 0.0f, -100.0f);
			terrain[j+3].vtx = D3DXVECTOR3(100.0f, 0.0f, -100.0f);

			// �@���x�N�g���̐ݒ�
			terrain[j + 0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			terrain[j + 1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			terrain[j + 2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			terrain[j + 3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


			// ���ˌ��̐ݒ�
			terrain[j + 0].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			terrain[j + 1].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			terrain[j + 2].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			terrain[j + 3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			terrain[j + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			terrain[j + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			terrain[j + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			terrain[j + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}
}
