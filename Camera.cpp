//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CAM_POS_P_X			(0.0f)					// カメラの視点初期位置(X座標)
#define	CAM_POS_P_Y			(10.0f)				// カメラの視点初期位置(Y座標)
#define	CAM_POS_P_Z			(-10.0f)				// カメラの視点初期位置(Z座標)
#define	CAM_POS_R_X			(0.0f)					// カメラの注視点初期位置(X座標)
#define	CAM_POS_R_Y			(0.0f)					// カメラの注視点初期位置(Y座標)
#define	CAM_POS_R_Z			(0.0f)					// カメラの注視点初期位置(Z座標)
#define	VIEW_ANGLE			(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比
#define	VIEW_NEAR_Z			(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z			(1000.0f)										// ビュー平面のFarZ値
#define	VALUE_MOVE_CAMERA	(2.0f)											// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.02f)								// カメラの回転量
#define	VALUE_MOVE			(5.0f/2)				// 移動量
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)		// 回転量
#define	CHASE_HEIGHT_P		(30.0f)				// 追跡時の視点の高さ
#define	CHASE_HEIGHT_R		(14.0f)					// 追跡時の注視点の高さ
#define	RATE_CHASE_CAMERA_P	(0.35f)					// カメラの視点への補正係数
#define	RATE_CHASE_CAMERA_R	(0.45f)					// カメラの注視点への補正係数
#define	INTERVAL_CAMERA_R	(4.5f)					// モデルの視線の先までの距離

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DXVECTOR3		g_posCameraEye;				// カメラの視点
D3DXVECTOR3		g_posCameraAt;				// カメラの注視点
D3DXVECTOR3		g_vecCameraUp;				// カメラの上方向
D3DXVECTOR3		g_rotCamera;				// カメラの回転
D3DXVECTOR3		g_posCameraPDest;			// カメラの視点の目的位置
float			g_fLengthIntervalCamera;	// カメラの視点と注視点の距離
float			g_fLengthIntervalPlayer;	// プレイヤーと注視点の距離
D3DXMATRIX		g_mtxView;					// ビューマトリックス
D3DXMATRIX		g_mtxProjection;			// プロジェクションマトリックス
float           g_camLength;
D3DXVECTOR2     prevPoint;

D3DXVECTOR3 DefaultForward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
D3DXVECTOR3 DefaultRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
D3DXVECTOR3 camForward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
D3DXVECTOR3 camRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

D3DXVECTOR3		g_posCameraRDest;			// カメラの注視点の目的位置
//=============================================================================
// カメラの初期化処理
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
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	D3DXVECTOR3 posPlayer;
	D3DXVECTOR3 rotPlayer;
	D3DXVECTOR3 movePlayer;
	float fLength;

	// モデルの位置取得
	posPlayer = GetPlayerPos();

	// モデルの目的の向き取得
	rotPlayer = GetPlayerRot();

	// モデルの移動量取得
	movePlayer = GetPlayerMove();

	//// 視点の目的位置
	//fLength = sqrtf(movePlayer.x * movePlayer.x + movePlayer.z * movePlayer.z) * 6.0f;
	//g_posCameraPDest.x = posPlayer.x - sinf(g_rotCamera.y) * g_fLengthIntervalCamera - sinf(rotPlayer.y) * fLength;
	//g_posCameraPDest.y = posPlayer.y + CHASE_HEIGHT_P;
	//g_posCameraPDest.z = posPlayer.z - cosf(g_rotCamera.y) * g_fLengthIntervalCamera - cosf(rotPlayer.y) * fLength;

	//// 注視点の目的位置
	fLength = g_fLengthIntervalPlayer + sqrtf(movePlayer.x * movePlayer.x + movePlayer.z * movePlayer.z) * 6.0f;
	//g_posCameraRDest.x = posPlayer.x - sinf(rotPlayer.y) * fLength;
	//g_posCameraRDest.y = posPlayer.y - CHASE_HEIGHT_R;
	//g_posCameraRDest.z = posPlayer.z - cosf(rotPlayer.y) * fLength;

	// 視点の補正
	g_posCameraEye.x = posPlayer.x - sinf(0)* CHASE_HEIGHT_R;
	g_posCameraEye.y = posPlayer.y + CHASE_HEIGHT_R;
	g_posCameraEye.z = posPlayer.z - cosf(0)* CHASE_HEIGHT_R;

	// 注視点の補正
	g_posCameraAt.x = posPlayer.x;
	g_posCameraAt.y = posPlayer.y;
	g_posCameraAt.z = posPlayer.z;
	

	

}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_mtxView, 
						&g_posCameraEye,		// カメラの視点
						&g_posCameraAt,		// カメラの注視点
						&g_vecCameraUp);		// カメラの上方向

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);


	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
								VIEW_ANGLE,			// ビュー平面の視野角
								VIEW_ASPECT,		// ビュー平面のアスペクト比
								VIEW_NEAR_Z,		// ビュー平面のNearZ値
								VIEW_FAR_Z);		// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);
}

//=============================================================================
// カメラの向きの取得
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	return g_rotCamera;
}
D3DXMATRIX  GetMtxView() {
	return g_mtxView;
}

