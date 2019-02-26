//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "ResultScore.h"
#include "score.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULT_SCORE		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_FRAME_RESULT_SCORE	"data/TEXTURE/frame_score.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	RESULT_SCORE_SIZE_X		(35.0f)							// �X�R�A�̐����̕�
#define	RESULT_SCORE_SIZE_Y		(50.0f)							// �X�R�A�̐����̍���
#define	RESULT_SCORE_INTERVAL_X	(0.0f)							// �X�R�A�̐����̕\���Ԋu

#define	NUM_PLACE			(8)								// �X�R�A�̌���

#define	RESULT_SCORE_POS_X			(SCREEN_WIDTH/2 - ((RESULT_SCORE_INTERVAL_X + RESULT_SCORE_SIZE_X) * NUM_PLACE/2) + 10.0f)	// �X�R�A�̕\����ʒu�w���W
#define	RESULT_SCORE_POS_Y			(SCREEN_HEIGHT/2 + RESULT_SCORE_SIZE_Y)																	// �X�R�A�̕\����ʒu�x���W

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexResultScore(LPDIRECT3DDEVICE9 pDevice);
void SetTextureResultScore(int idx, int number);

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResultScore[2] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResultScore = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXVECTOR3				g_posResultScore;						// �ʒu
D3DXVECTOR3				g_rotResultScore;						// ����

int						g_ResultScore;						// �X�R�A

														//=============================================================================
														// ����������
														//=============================================================================
HRESULT InitResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posResultScore = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotResultScore = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �X�R�A�̏�����
	g_ResultScore = 0;

	// ���_���̍쐬
	MakeVertexResultScore(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_RESULT_SCORE,			// �t�@�C���̖��O
		&g_pD3DTextureResultScore[0]);	// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FRAME_RESULT_SCORE,		// �t�@�C���̖��O
		&g_pD3DTextureResultScore[1]);	// �ǂݍ��ރ������[

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResultScore(void)
{
	if (g_pD3DTextureResultScore[0] != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResultScore[0]->Release();
		g_pD3DTextureResultScore[0] = NULL;
	}
	if (g_pD3DTextureResultScore[1] != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResultScore[1]->Release();
		g_pD3DTextureResultScore[1] = NULL;
	}

	if (g_pD3DVtxBuffResultScore != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffResultScore->Release();
		g_pD3DVtxBuffResultScore = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResultScore(void)
{
	g_ResultScore = GetScore();
	for (int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++)
	{
		int number;

		number = (g_ResultScore % (int)(powf(10.0f, (float)(NUM_PLACE - nCntPlace)))) / (int)(powf(10.0f, (float)(NUM_PLACE - nCntPlace - 1)));
		SetTextureResultScore(nCntPlace, number);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pD3DVtxBuffResultScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureResultScore[0]);

	// �|���S���̕`��
	for (int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), NUM_POLYGON);
	}

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureResultScore[1]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (NUM_PLACE * 4), NUM_POLYGON);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexResultScore(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * NUM_PLACE + 4),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,								// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,									// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,								// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffResultScore,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))											// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + nCntPlace * RESULT_SCORE_SIZE_X + RESULT_SCORE_INTERVAL_X, RESULT_SCORE_POS_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + nCntPlace * (RESULT_SCORE_INTERVAL_X + RESULT_SCORE_SIZE_X) + RESULT_SCORE_SIZE_X, RESULT_SCORE_POS_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + nCntPlace * RESULT_SCORE_SIZE_X + RESULT_SCORE_INTERVAL_X, RESULT_SCORE_POS_Y + RESULT_SCORE_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + nCntPlace * (RESULT_SCORE_INTERVAL_X + RESULT_SCORE_SIZE_X) + RESULT_SCORE_SIZE_X, RESULT_SCORE_POS_Y + RESULT_SCORE_SIZE_Y, 0.0f);

			// rhw�̐ݒ�
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X - 15, RESULT_SCORE_POS_Y - 25, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + (RESULT_SCORE_INTERVAL_X + RESULT_SCORE_SIZE_X) * NUM_PLACE + 15, RESULT_SCORE_POS_Y - 25, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X - 15, RESULT_SCORE_POS_Y + 55, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + (RESULT_SCORE_INTERVAL_X + RESULT_SCORE_SIZE_X) * NUM_PLACE + 15, RESULT_SCORE_POS_Y + 55, 0.0f);

			// rhw�̐ݒ�
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffResultScore->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureResultScore(int idx, int number)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (idx * 4);

	// ���_���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffResultScore->Unlock();
}

//=============================================================================
// �X�R�A�̕ύX
//=============================================================================
void ChangeResultScore(int value)
{
	g_ResultScore += value;
	if (g_ResultScore < 0)
	{
		g_ResultScore = 0;
	}
	else if (g_ResultScore >= (int)(powf(10.0f, (float)(NUM_PLACE + 1))))
	{
		g_ResultScore = (int)(powf(10.0f, (float)(NUM_PLACE + 1))) - 1;
	}
}

