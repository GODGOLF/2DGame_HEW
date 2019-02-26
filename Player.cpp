//=============================================================================
//
// モデル処理 [Player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "camera.h"
#include "input.h"
#include "Item.h"
#include "score.h"
#include "sound.h"
#include <math.h>
#include <string>
#include "fade.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CHARACTER_BODY			"data/MODEL/cockroach.x"	// 読み込むモデルファイル名

#define	CHARACTER_TEXTURE			"data/MODEL/tex.jpg"	// 読み込むモデルファイル名

#define	PLAYER_RADIUS		(1.0f)						// 半径
#define	VALUE_MOVE_PLAYER	(0.155f)					// 移動速度
#define	RATE_MOVE_PLAYER	(0.025f)					// 移動慣性係数
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.025f)			// 回転速度
#define	RATE_ROTATE_PLAYER	(0.10f)						// 回転慣性係数
#define	VALUE_MOVE_BULLET	(7.5f)						// 弾の移動速度


#define	VALUE_MOVE_MODEL	(5.0f)					// 移動速度
#define	VALUE_ROTATE_MODEL	(D3DX_PI * 0.02f)		// 回転速度

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//****************************************************ฝฝฝฝ*************************
// グローバル変数
//*****************************************************************************

struct BODY {
	LPDIRECT3DTEXTURE9	g_pD3DTexturePlayer;		// テクスチャへのポインタ
	LPD3DXMESH			g_pD3DXMeshPlayer;			// メッシュ情報へのポインタ
	LPD3DXBUFFER		g_pD3DXBuffMatPlayer;		// マテリアル情報へのポインタ
	DWORD				g_nNumMatPlayer;			// マテリアル情報の数
};

PLAYER				g_player;					// プレイヤーワーク

BODY body[3];

D3DXMATRIX			g_mtxWorldPlayer;			// ワールドマトリックス


static int					g_nIdxShadow;				// 影ID
static float				g_fSizeShadow;				// 影のサイズ
static D3DXCOLOR			g_colShadow;				// 影の色

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	


	// Xファイルの読み込み
	if(FAILED(D3DXLoadMeshFromX(CHARACTER_BODY,
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&body[0].g_pD3DXBuffMatPlayer,
								NULL,
								&body[0].g_nNumMatPlayer,
								&body[0].g_pD3DXMeshPlayer)))
	{
		return E_FAIL;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,									// デバイスへのポインタ
		CHARACTER_TEXTURE,						// ファイルの名前
		&body[0].g_pD3DTexturePlayer);	// 読み込むメモリー

	//// 左腕
	//if (FAILED(D3DXLoadMeshFromX(CHARACTER_L_H,
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&body[1].g_pD3DXBuffMatPlayer,
	//	NULL,
	//	&body[1].g_nNumMatPlayer,
	//	&body[1].g_pD3DXMeshPlayer)))
	//{
	//	return E_FAIL;
	//}


#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_FILENAME,		// ファイルの名前
								&g_pD3DTexturePlayer);	// 読み込むメモリー
#endif

														// 影の生成
	
	g_fSizeShadow = 25.0f;
	g_colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	ResetPlayer();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	for (int i = 0; i < 3; i++) {
		if (body[i].g_pD3DTexturePlayer) {
			body[i].g_pD3DTexturePlayer->Release();
			body[i].g_pD3DTexturePlayer = 0;
		}
		if (body[i].g_pD3DXBuffMatPlayer) {
			body[i].g_pD3DXBuffMatPlayer->Release();
			body[i].g_pD3DXBuffMatPlayer = 0;
		}
		if (body[i].g_pD3DXMeshPlayer) {
			body[i].g_pD3DXMeshPlayer->Release();
			body[i].g_pD3DXBuffMatPlayer = 0;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	if (g_player.isDied) {
		return;
	}
	D3DXVECTOR3 rotCamera;
	float fDiffRotY;

	// カメラの向き取得
	rotCamera = GetRotCamera();

	D3DXVECTOR2 mouse;
	getMouseXY(mouse);
	OutputDebugString(("X: "+ std::to_string(mouse.x)+" "+ "Y: " + std::to_string(mouse.y)+"\n").c_str());
	if (GetKeyboardPress(DIK_A))
	{
		if (GetKeyboardPress(DIK_W))
		{// 左前移動
			g_player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y + D3DX_PI * 0.75f;
		}
		else if (GetKeyboardPress(DIK_S))
		{// 左後移動
			g_player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y + D3DX_PI * 0.25f;
		}
		else
		{// 左移動
			g_player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y + D3DX_PI * 0.50f;
		}
	}
	else if (GetKeyboardPress(DIK_D))
	{
		if (GetKeyboardPress(DIK_W))
		{// 右前移動
			g_player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y - D3DX_PI * 0.75f;
		}
		else if (GetKeyboardPress(DIK_S))
		{// 右後移動
			g_player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y - D3DX_PI * 0.25f;
		}
		else
		{// 右移動
			g_player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y - D3DX_PI * 0.50f;
		}
	}
	else if (GetKeyboardPress(DIK_W))
	{// 前移動
		g_player.move.x -= sinf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PLAYER;
		g_player.move.z -= cosf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PLAYER;

		g_player.rotDest.y = D3DX_PI + rotCamera.y;
	}
	else if (GetKeyboardPress(DIK_S))
	{// 後移動
		g_player.move.x -= sinf(rotCamera.y) * VALUE_MOVE_PLAYER;
		g_player.move.z -= cosf(rotCamera.y) * VALUE_MOVE_PLAYER;

		g_player.rotDest.y = rotCamera.y;
	}

	if (GetKeyboardPress(DIK_UP))
	{// 上昇
		g_player.move.y += VALUE_MOVE_PLAYER;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{// 下降
		g_player.move.y -= VALUE_MOVE_PLAYER;
	}

	if (GetKeyboardPress(DIK_LEFT))
	{// 左回転
		g_player.rotDest.y -= VALUE_ROTATE_PLAYER;
		if (g_player.rotDest.y < -D3DX_PI)
		{
			g_player.rotDest.y += D3DX_PI * 2.0f;
		}
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{// 右回転
		g_player.rotDest.y += VALUE_ROTATE_PLAYER;
		if (g_player.rotDest.y > D3DX_PI)
		{
			g_player.rotDest.y -= D3DX_PI * 2.0f;
		}
	}

	// 弾発射
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 move;

		pos.x = g_player.pos.x - sinf(g_player.rot.y) * 10.0f;
		pos.y = g_player.pos.y;
		pos.z = g_player.pos.z - cosf(g_player.rot.y) * 10.0f;

		move.x = -sinf(g_player.rot.y) * VALUE_MOVE_BULLET;
		move.y = 0.0f;
		move.z = -cosf(g_player.rot.y) * VALUE_MOVE_BULLET;

		

		// SE再生
		
	}

	// 目的の角度までの差分
	fDiffRotY = g_player.rotDest.y - g_player.rot.y;
	if (fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if (fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// 目的の角度まで慣性をかける
	g_player.rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if (g_player.rot.y > D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}
	if (g_player.rot.y < -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}

	/// 位置移動
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;
	g_player.pos.z += g_player.move.z;

	if (g_player.pos.x < -80.0f)
	{
		g_player.pos.x = -80.0f;
	}
	if (g_player.pos.x > 75.0f)
	{
		g_player.pos.x = 75.0f;
	}
	if (g_player.pos.y < 10.0f)
	{
		g_player.pos.y = 10.0f;
	}
	if (g_player.pos.y > 40.0f)
	{
		g_player.pos.y = 40.0f;
	}
	if (g_player.pos.z > 89.0f)
	{
		g_player.pos.z = 89.0f;
	}
	if (g_player.pos.z < -47.0f)
	{
		g_player.pos.z = -47.0f;
	}

	// 移動量に慣性をかける
	g_player.move.x += (0.0f - g_player.move.x) * RATE_MOVE_PLAYER;
	g_player.move.y += (0.0f - g_player.move.y) * RATE_MOVE_PLAYER;
	g_player.move.z += (0.0f - g_player.move.z) * RATE_MOVE_PLAYER;

	// 影の位置設定
	{
		//SetPositionShadow(g_player.nIdxShadow, D3DXVECTOR3(g_player.pos.x, 0.1f, g_player.pos.z));

		float fSizeX = 20.0f + (g_player.pos.y - 10.0f) * 0.05f;
		if (fSizeX < 20.0f)
		{
			fSizeX = 20.0f;
		}
		float fSizeY = 20.0f + (g_player.pos.y - 10.0f) * 0.05f;
		if (fSizeY < 20.0f)
		{
			fSizeY = 20.0f;
		}

		//SetVertexShadow(g_player.nIdxShadow, fSizeX, fSizeY);

		float colA = (200.0f - (g_player.pos.y - 10.0f)) / 400.0f;
		if (colA < 0.0f)
		{
			colA = 0.0f;
		}
		//SetColorShadow(g_player.nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, colA));
	}

	if ((g_player.move.x * g_player.move.x
		+ g_player.move.y * g_player.move.y
		+ g_player.move.z * g_player.move.z) > 1.0f)
	{
		D3DXVECTOR3 pos;

		pos.x = g_player.pos.x + sinf(g_player.rot.y) * 10.0f;
		pos.y = g_player.pos.y + 2.0f;
		pos.z = g_player.pos.z + cosf(g_player.rot.y) * 10.0f;

		// エフェクトの設定
		/*SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.85f, 0.05f, 0.65f, 0.50f), 14.0f, 14.0f, 20);
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.65f, 0.85f, 0.05f, 0.30f), 10.0f, 10.0f, 20);
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.45f, 0.45f, 0.05f, 0.15f), 5.0f, 5.0f, 20);*/

	}
	// アイテムとの当たり判定
	{
		ITEM *pItem;

		// アイテムを取得
		pItem = GetItem();
		for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
		{
			if (pItem->bUse == true)
			{
				float fLength;

				fLength = (g_player.pos.x - pItem->pos.x) * (g_player.pos.x - pItem->pos.x)
					+ (g_player.pos.y - pItem->pos.y) * (g_player.pos.y - pItem->pos.y)
					+ (g_player.pos.z - pItem->pos.z) * (g_player.pos.z - pItem->pos.z);
				if (fLength < (g_player.fRadius + pItem->fRadius) * (g_player.fRadius + pItem->fRadius))
				{
					// アイテム消去
					DeleteItem(nCntItem);

					// スコア加算
					ChangeScore(100);

					// SE再生
					PlaySound(SOUND_LABEL_SE_COIN);
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldPlayer);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPlayer);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)body[0].g_pD3DXBuffMatPlayer->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)body[0].g_nNumMatPlayer; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, body[0].g_pD3DTexturePlayer);

		// 描画
		body[0].g_pD3DXMeshPlayer->DrawSubset(nCntMat);
	}

	{// マテリアルをデフォルトに戻す
		D3DXMATERIAL mat;

		mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		pDevice->SetMaterial(&mat.MatD3D);
	}
}


D3DXVECTOR3 GetPlayerPos(void)
{
	return g_player.pos;
}
D3DXVECTOR3 GetPlayerRot(void) {
	return g_player.rot;
}
D3DXVECTOR3 GetPlayerMove(void) {
	return g_player.move;
}
void ResetPlayer() {
	g_player.pos = D3DXVECTOR3(0.0f, -15.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.fRadius = PLAYER_RADIUS;
	g_player.isDied = false;
}