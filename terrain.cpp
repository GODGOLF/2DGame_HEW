//=============================================================================
//
// 地面処理 [terrain.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "terrain.h"
#include <vector>



//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DIRECTORY_TEXTURE       "data/TEXTURE/"
#define	TEXTURE_FIELD	"data/TEXTURE/field000.jpg"	// 読み込むテクスチャファイル名
#define	MAP_FILE		 "data/MODEL/room.x"	// 読み込むモデルファイル名
#define TERRAIN_SIZE_X 20
#define TERRAIN_SIZE_Y 20
#define TERRAIN_WIDTH 800
#define TERRAIN_HEIGHT 800
#define TERRAIN_POS_X -500
#define TERRAIN_POS_Y -100




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTerrain(LPDIRECT3DDEVICE9 pDevice);

void generateTerrain();

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTerrain = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTerrain = NULL;	// 頂点バッファへのポインタ
struct TextureInfo {
	char name[256];
	LPDIRECT3DTEXTURE9 g_pD3DTexturePlayer;
};
struct Map {
	std::vector<TextureInfo> textures;		// テクスチャへのポインタ
	LPD3DXMESH			g_pD3DXMeshPlayer;			// メッシュ情報へのポインタ
	LPD3DXBUFFER		g_pD3DXBuffMatPlayer;		// マテリアル情報へのポインタ
	DWORD				g_nNumMatPlayer;// マテリアル情報の数
	std::vector<int> textureIndex;
};


D3DXMATRIX				g_mtxWorldTerrain;			// ワールドマトリックス
D3DXVECTOR3				g_posTerrain;					// 現在の位置
D3DXVECTOR3				g_rotTerrain;					// 現在の向き
D3DXVECTOR3				g_scaleTerrain;					// 現在の拡大
VERTEX_3D terrain[TERRAIN_WIDTH * TERRAIN_HEIGHT];
Map map;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTerrain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HRESULT hr;

	// 位置、向きの初期設定
	g_posTerrain = D3DXVECTOR3(0.0f, 13.0f, 0.0f);
	g_rotTerrain = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_scaleTerrain = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// テクスチャの読み込み
	//hr = D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
	//	TEXTURE_FIELD,			// ファイルの名前
	//	&g_pD3DTextureTerrain);	// 読み込むメモリー

	//							// 頂点情報の作成
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
	// マテリアル情報に対するポインタを取得
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
// 終了処理
//=============================================================================
void UninitTerrain(void)
{
	if (g_pD3DTextureTerrain != NULL)
	{// テクスチャの開放
		g_pD3DTextureTerrain->Release();
		g_pD3DTextureTerrain = NULL;
	}

	if (g_pD3DVtxBuffTerrain != NULL)
	{// 頂点バッファの開放
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
// 更新処理
//=============================================================================
void UpdateTerrain(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTerrain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxScale;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldTerrain);
	//拡大を反映
	D3DXMatrixScaling(&mtxScale, g_scaleTerrain.x, g_scaleTerrain.y, g_scaleTerrain.z);
	D3DXMatrixMultiply(&g_mtxWorldTerrain, &g_mtxWorldTerrain, &mtxScale);
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotTerrain.y, g_rotTerrain.x, g_rotTerrain.z);
	D3DXMatrixMultiply(&g_mtxWorldTerrain, &g_mtxWorldTerrain, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_posTerrain.x, g_posTerrain.y, g_posTerrain.z);
	D3DXMatrixMultiply(&g_mtxWorldTerrain, &g_mtxWorldTerrain, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldTerrain);

	D3DXMATERIAL *pD3DXMat;
	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)map.g_pD3DXBuffMatPlayer->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)map.g_nNumMatPlayer; nCntMat++)
	{
		pD3DXMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
		pD3DXMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		pD3DXMat[nCntMat].MatD3D.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, map.textures[map.textureIndex[nCntMat]].g_pD3DTexturePlayer);

		// 描画
		map.g_pD3DXMeshPlayer->DrawSubset(nCntMat);
	}

	{// マテリアルをデフォルトに戻す
		D3DXMATERIAL mat;

		mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		pDevice->SetMaterial(&mat.MatD3D);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTerrain(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffTerrain,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffTerrain->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
		pVtx[1].vtx = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
		pVtx[2].vtx = D3DXVECTOR3(-100.0f, 0.0f, -100.0f);
		pVtx[3].vtx = D3DXVECTOR3(100.0f, 0.0f, -100.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffTerrain->Unlock();
	}

	return S_OK;
}
void generateTerrain() {

	int x = TERRAIN_POS_X;
	int y = TERRAIN_POS_Y;

	for (int i = 0; i < TERRAIN_HEIGHT; i++) {
		for (int j = 0; j < TERRAIN_WIDTH; i++) {
			// 頂点座標の設定
			terrain[j].vtx = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
			terrain[j+1].vtx = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
			terrain[j+2].vtx = D3DXVECTOR3(-100.0f, 0.0f, -100.0f);
			terrain[j+3].vtx = D3DXVECTOR3(100.0f, 0.0f, -100.0f);

			// 法線ベクトルの設定
			terrain[j + 0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			terrain[j + 1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			terrain[j + 2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			terrain[j + 3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


			// 反射光の設定
			terrain[j + 0].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			terrain[j + 1].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			terrain[j + 2].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			terrain[j + 3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			terrain[j + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			terrain[j + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			terrain[j + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			terrain[j + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}
}
