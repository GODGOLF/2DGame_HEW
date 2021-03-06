//=============================================================================
//
// ライト
//
//=============================================================================
#include "light.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	NUM_LIGHT		(2)					// ライトの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DLIGHT9 g_aLight[NUM_LIGHT];				// ライトのワーク

//=============================================================================
// ライトの初期化処理
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;


	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));

	// ライト0のタイプの設定
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	// ライト0の拡散光の設定
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト0の環境光の設定
	g_aLight[0].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライト0の方向の設定
	vecDir = D3DXVECTOR3(0.0f, -0.6f, -1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[0].Direction, &vecDir);

	// ライト0をレンダリングパイプラインに設定
	pDevice->SetLight(0, &g_aLight[0]);

	// ライト0を使用使用状態に
	pDevice->LightEnable(0, TRUE);



	//// D3DLIGHT9構造体を0でクリアする
	//ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));

	//// ライト0のタイプの設定
	//g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	//// ライト0の拡散光の設定
	//g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ライト0の鏡面反射光の設定
	//g_aLight[0].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ライト0の環境光の設定
	//g_aLight[0].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ライト0の方向の設定
	//vecDir = D3DXVECTOR3(0.0f, -1.0f, 1.0f);
	//D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[0].Direction, &vecDir);

	////g_aLight[0].Range = 1000.0f;

	//// ライト0をレンダリングパイプラインに設定
	//pDevice->SetLight(0, &g_aLight[0]);

	//// ライト0を使用使用状態に
	//pDevice->LightEnable(0, TRUE);


	//// D3DLIGHT9構造体を0でクリアする
	//ZeroMemory(&g_aLight[1], sizeof(D3DLIGHT9));

	//// ライト1のタイプの設定
	//g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	//// ライト1の拡散光の設定
	//g_aLight[1].Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	//// ライト1の環境光の設定
	//g_aLight[1].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ライト1の方向の設定
	//vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.8f);
	//D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir);

	//// ライト1をレンダリングパイプラインに設定
	//pDevice->SetLight(1, &g_aLight[1]);

	//// ライト1の設定
	//pDevice->LightEnable(1, TRUE);


	// マテリアルにアンビエントを設定する
	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl , sizeof( D3DMATERIAL9 ) );
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	pDevice->SetMaterial( &mtrl );
	

	// ライティングモードをON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//pDevice->SetRenderState(D3DRS_AMBIENT, 0x00444444);
	
}

//=============================================================================
// ライトの終了処理
//=============================================================================
void UninitLight(void)
{
}

//=============================================================================
// ライトの更新処理
//=============================================================================
void UpdateLight(void)
{
}
//=============================================================================
// ライトの描画処理
//=============================================================================
void DrawLight(void)
{
}
