//=============================================================================
//
// �J�������� [camera.cpp]
// Author : 
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CAM_POS_P_X			(0.0f)					// �J�����̎��_�����ʒu(X���W)
#define	CAM_POS_P_Y			(10.0f)				// �J�����̎��_�����ʒu(Y���W)
#define	CAM_POS_P_Z			(-10.0f)				// �J�����̎��_�����ʒu(Z���W)
#define	CAM_POS_R_X			(0.0f)					// �J�����̒����_�����ʒu(X���W)
#define	CAM_POS_R_Y			(0.0f)					// �J�����̒����_�����ʒu(Y���W)
#define	CAM_POS_R_Z			(0.0f)					// �J�����̒����_�����ʒu(Z���W)
#define	VIEW_ANGLE			(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��
#define	VIEW_NEAR_Z			(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z			(1000.0f)										// �r���[���ʂ�FarZ�l
#define	VALUE_MOVE_CAMERA	(2.0f)											// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.02f)								// �J�����̉�]��
#define	VALUE_MOVE			(5.0f/2)				// �ړ���
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)		// ��]��
#define	CHASE_HEIGHT_P		(30.0f)				// �ǐՎ��̎��_�̍���
#define	CHASE_HEIGHT_R		(14.0f)					// �ǐՎ��̒����_�̍���
#define	RATE_CHASE_CAMERA_P	(0.35f)					// �J�����̎��_�ւ̕␳�W��
#define	RATE_CHASE_CAMERA_R	(0.45f)					// �J�����̒����_�ւ̕␳�W��
#define	INTERVAL_CAMERA_R	(4.5f)					// ���f���̎����̐�܂ł̋���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DXVECTOR3		g_posCameraEye;				// �J�����̎��_
D3DXVECTOR3		g_posCameraAt;				// �J�����̒����_
D3DXVECTOR3		g_vecCameraUp;				// �J�����̏����
D3DXVECTOR3		g_rotCamera;				// �J�����̉�]
D3DXVECTOR3		g_posCameraPDest;			// �J�����̎��_�̖ړI�ʒu
float			g_fLengthIntervalCamera;	// �J�����̎��_�ƒ����_�̋���
float			g_fLengthIntervalPlayer;	// �v���C���[�ƒ����_�̋���
D3DXMATRIX		g_mtxView;					// �r���[�}�g���b�N�X
D3DXMATRIX		g_mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
float           g_camLength;
D3DXVECTOR2     prevPoint;

D3DXVECTOR3 DefaultForward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
D3DXVECTOR3 DefaultRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
D3DXVECTOR3 camForward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
D3DXVECTOR3 camRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

D3DXVECTOR3		g_posCameraRDest;			// �J�����̒����_�̖ړI�ʒu
//=============================================================================
// �J�����̏���������
//=============================================================================
HRESULT InitCamera(void)
{
	g_posCameraEye = D3DXVECTOR3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);
	g_posCameraAt  = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_X, CAM_POS_R_X);
	g_vecCameraUp  = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_posCameraPDest = D3DXVECTOR3(0.0f, 10.0f, -20.0f);
	g_posCameraRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_camLength = 30;
	float fVecX,fVecZ;
	fVecX = g_posCameraEye.x - g_posCameraAt.x;
	fVecZ = g_posCameraEye.z - g_posCameraAt.z;
	g_fLengthIntervalCamera = sqrtf(fVecX * fVecX + fVecZ * fVecZ);
	g_fLengthIntervalPlayer = INTERVAL_CAMERA_R;
	return S_OK;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	D3DXVECTOR3 posPlayer;
	D3DXVECTOR3 rotPlayer;
	D3DXVECTOR3 movePlayer;
	float fLength;

	// ���f���̈ʒu�擾
	posPlayer = GetPlayerPos();

	// ���f���̖ړI�̌����擾
	rotPlayer = GetPlayerRot();

	// ���f���̈ړ��ʎ擾
	movePlayer = GetPlayerMove();

	//// ���_�̖ړI�ʒu
	//fLength = sqrtf(movePlayer.x * movePlayer.x + movePlayer.z * movePlayer.z) * 6.0f;
	//g_posCameraPDest.x = posPlayer.x - sinf(g_rotCamera.y) * g_fLengthIntervalCamera - sinf(rotPlayer.y) * fLength;
	//g_posCameraPDest.y = posPlayer.y + CHASE_HEIGHT_P;
	//g_posCameraPDest.z = posPlayer.z - cosf(g_rotCamera.y) * g_fLengthIntervalCamera - cosf(rotPlayer.y) * fLength;

	//// �����_�̖ړI�ʒu
	fLength = g_fLengthIntervalPlayer + sqrtf(movePlayer.x * movePlayer.x + movePlayer.z * movePlayer.z) * 6.0f;
	//g_posCameraRDest.x = posPlayer.x - sinf(rotPlayer.y) * fLength;
	//g_posCameraRDest.y = posPlayer.y - CHASE_HEIGHT_R;
	//g_posCameraRDest.z = posPlayer.z - cosf(rotPlayer.y) * fLength;

	// ���_�̕␳
	g_posCameraEye.x = posPlayer.x - sinf(0)* CHASE_HEIGHT_R;
	g_posCameraEye.y = posPlayer.y + CHASE_HEIGHT_R;
	g_posCameraEye.z = posPlayer.z - cosf(0)* CHASE_HEIGHT_R;

	// �����_�̕␳
	g_posCameraAt.x = posPlayer.x;
	g_posCameraAt.y = posPlayer.y;
	g_posCameraAt.z = posPlayer.z;
	

	

}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_mtxView, 
						&g_posCameraEye,		// �J�����̎��_
						&g_posCameraAt,		// �J�����̒����_
						&g_vecCameraUp);		// �J�����̏����

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);


	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
								VIEW_ANGLE,			// �r���[���ʂ̎���p
								VIEW_ASPECT,		// �r���[���ʂ̃A�X�y�N�g��
								VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l
								VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);
}

//=============================================================================
// �J�����̌����̎擾
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	return g_rotCamera;
}
D3DXMATRIX  GetMtxView() {
	return g_mtxView;
}

