//=============================================================================
//
// モデル処理 [model.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "camera.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	ENEMY_AIRPLANE		"data/MODEL/airplane000.x"	// 読み込むモデル名
#define	VALUE_MOVE_ENEMY	(0.50f)						// 移動速度
#define	RATE_MOVE_ENEMY		(0.20f)						// 移動慣性係数
#define	VALUE_ROTATE_ENEMY	(D3DX_PI * 0.05f)			// 回転速度
#define	RATE_ROTATE_ENEMY	(0.20f)						// 回転慣性係数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureEnemy;			// テクスチャ読み込み場所
LPD3DXMESH			g_pD3DXMeshEnemy;			// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER		g_pD3DXBuffMatEnemy;		// メッシュのマテリアル情報を格納
DWORD				g_nNumMatEnemy;				// 属性情報の総数

D3DXMATRIX			g_mtxWorldEnemy;			// ワールドマトリックス
D3DXVECTOR3			g_posEnemy;					// 現在の位置
D3DXVECTOR3			g_moveEnemy;				// 移動量
D3DXVECTOR3			g_rotEnemy;					// 現在の向き
D3DXVECTOR3			g_rotDestEnemy;				// 目的の向き

int					g_nIdxShadow;				// 影ID
float				g_fSizeShadow;				// 影のサイズ
D3DXCOLOR			g_colShadow;				// 影の色

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_pD3DTextureEnemy = NULL;
	g_pD3DXMeshEnemy = NULL;
	g_pD3DXBuffMatEnemy = NULL;

	g_posEnemy = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	g_moveEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotDestEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Xファイルの読み込み
	if(FAILED(D3DXLoadMeshFromX(ENEMY_AIRPLANE,
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&g_pD3DXBuffMatEnemy,
								NULL,
								&g_nNumMatEnemy,
								&g_pD3DXMeshEnemy)))
	{
		return E_FAIL;
	}

#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_FILENAME,		// ファイルの名前
								&g_pD3DTextureEnemy);	// 読み込むメモリー
#endif

	

	g_fSizeShadow = 25.0f;
	g_colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if(g_pD3DTextureEnemy != NULL)
	{// テクスチャの開放
		g_pD3DTextureEnemy->Release();
		g_pD3DTextureEnemy = NULL;
	}

	if(g_pD3DXMeshEnemy != NULL)
	{// メッシュの開放
		g_pD3DXMeshEnemy->Release();
		g_pD3DXMeshEnemy = NULL;
	}

	if(g_pD3DXBuffMatEnemy != NULL)
	{// マテリアルの開放
		g_pD3DXBuffMatEnemy->Release();
		g_pD3DXBuffMatEnemy = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	D3DXVECTOR3 rotCamera;
	float fDiffRotY;

	// カメラの向き取得
	rotCamera = GetRotCamera();

	if(GetKeyboardPress(DIK_LEFT))
	{
		if(GetKeyboardPress(DIK_UP))
		{// 左前移動
			g_moveEnemy.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_ENEMY;
			g_moveEnemy.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_ENEMY;

			g_rotDestEnemy.y = rotCamera.y + D3DX_PI * 0.75f;
		}
		else if(GetKeyboardPress(DIK_DOWN))
		{// 左後移動
			g_moveEnemy.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_ENEMY;
			g_moveEnemy.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_ENEMY;

			g_rotDestEnemy.y = rotCamera.y + D3DX_PI * 0.25f;
		}
		else
		{// 左移動
			g_moveEnemy.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_ENEMY;
			g_moveEnemy.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_ENEMY;

			g_rotDestEnemy.y = rotCamera.y + D3DX_PI * 0.50f;
		}
	}
	else if(GetKeyboardPress(DIK_RIGHT))
	{
		if(GetKeyboardPress(DIK_UP))
		{// 右前移動
			g_moveEnemy.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_ENEMY;
			g_moveEnemy.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_ENEMY;

			g_rotDestEnemy.y = rotCamera.y - D3DX_PI * 0.75f;
		}
		else if(GetKeyboardPress(DIK_DOWN))
		{// 右後移動
			g_moveEnemy.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_ENEMY;
			g_moveEnemy.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_ENEMY;

			g_rotDestEnemy.y = rotCamera.y - D3DX_PI * 0.25f;
		}
		else
		{// 右移動
			g_moveEnemy.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_ENEMY;
			g_moveEnemy.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_ENEMY;

			g_rotDestEnemy.y = rotCamera.y - D3DX_PI * 0.50f;
		}
	}
	else if(GetKeyboardPress(DIK_UP))
	{// 前移動
		g_moveEnemy.x -= sinf(D3DX_PI + rotCamera.y) * VALUE_MOVE_ENEMY;
		g_moveEnemy.z -= cosf(D3DX_PI + rotCamera.y) * VALUE_MOVE_ENEMY;

		g_rotDestEnemy.y = D3DX_PI + rotCamera.y;
	}
	else if(GetKeyboardPress(DIK_DOWN))
	{// 後移動
		g_moveEnemy.x -= sinf(rotCamera.y) * VALUE_MOVE_ENEMY;
		g_moveEnemy.z -= cosf(rotCamera.y) * VALUE_MOVE_ENEMY;

		g_rotDestEnemy.y = rotCamera.y;
	}

	if(GetKeyboardPress(DIK_LSHIFT))
	{// 左回転
		g_rotDestEnemy.y -= VALUE_ROTATE_ENEMY;
		if(g_rotDestEnemy.y < -D3DX_PI)
		{
			g_rotDestEnemy.y += D3DX_PI * 2.0f;
		}
	}
	if(GetKeyboardPress(DIK_RSHIFT))
	{// 右回転
		g_rotDestEnemy.y += VALUE_ROTATE_ENEMY;
		if(g_rotDestEnemy.y > D3DX_PI)
		{
			g_rotDestEnemy.y -= D3DX_PI * 2.0f;
		}
	}

	if(GetKeyboardPress(DIK_I))
	{// 上昇
		g_moveEnemy.y += VALUE_MOVE_ENEMY * 0.5f;

		if(g_posEnemy.y < 75.0f)
		{
			g_fSizeShadow += 0.2f;
			g_colShadow.a -= 0.004f;
		}
	}
	if(GetKeyboardPress(DIK_K))
	{// 下降
		g_moveEnemy.y -= VALUE_MOVE_ENEMY * 0.5f;

		if(g_posEnemy.y > 5.0f)
		{
			g_fSizeShadow -= 0.2f;
			g_colShadow.a += 0.004f;
		}
	}

	// 目的の角度までの差分
	fDiffRotY = g_rotDestEnemy.y - g_rotEnemy.y;
	if(fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if(fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// 目的の角度まで慣性をかける
	g_rotEnemy.y += fDiffRotY * RATE_ROTATE_ENEMY;
	if(g_rotEnemy.y > D3DX_PI)
	{
		g_rotEnemy.y -= D3DX_PI * 2.0f;
	}
	if(g_rotEnemy.y < -D3DX_PI)
	{
		g_rotEnemy.y += D3DX_PI * 2.0f;
	}

	/// 位置移動
	g_posEnemy.x += g_moveEnemy.x;
	g_posEnemy.y += g_moveEnemy.y;
	if(g_posEnemy.y < 5.0f)
	{
		g_posEnemy.y = 5.0f;
	}
	if(g_posEnemy.y > 75.0f)
	{
		g_posEnemy.y = 75.0f;
	}
	g_posEnemy.z += g_moveEnemy.z;

	// 移動量に慣性をかける
	g_moveEnemy.x += (0.0f - g_moveEnemy.x) * RATE_MOVE_ENEMY;
	g_moveEnemy.y += (0.0f - g_moveEnemy.y) * RATE_MOVE_ENEMY;
	g_moveEnemy.z += (0.0f - g_moveEnemy.z) * RATE_MOVE_ENEMY;

	if(g_posEnemy.x < -310.0f)
	{
		g_posEnemy.x = -310.0f;
	}
	if(g_posEnemy.x > 310.0f)
	{
		g_posEnemy.x = 310.0f;
	}
	if(g_posEnemy.z < -310.0f)
	{
		g_posEnemy.z = -310.0f;
	}
	if(g_posEnemy.z > 310.0f)
	{
		g_posEnemy.z = 310.0f;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldEnemy);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotEnemy.y, g_rotEnemy.x, g_rotEnemy.z);
	D3DXMatrixMultiply(&g_mtxWorldEnemy, &g_mtxWorldEnemy, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_posEnemy.x, g_posEnemy.y, g_posEnemy.z);
	D3DXMatrixMultiply(&g_mtxWorldEnemy, &g_mtxWorldEnemy, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEnemy);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatEnemy->GetBufferPointer();

	for(int nCntMat = 0; nCntMat < (int)g_nNumMatEnemy; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureEnemy);

		// 描画
		g_pD3DXMeshEnemy->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 位置取得
//=============================================================================
D3DXVECTOR3 GetPositionEnemy(void)
{
	return g_posEnemy;
}

//=============================================================================
// 向き取得
//=============================================================================
D3DXVECTOR3 GetRotationEnemy(void)
{
	return g_rotEnemy;
}

//=============================================================================
// 目的の向き取得
//=============================================================================
D3DXVECTOR3 GetRotationDestEnemy(void)
{
	return g_rotDestEnemy;
}
