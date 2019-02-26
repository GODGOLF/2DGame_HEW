//=============================================================================
//
// スコア処理 [score.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "ResultScore.h"
#include "score.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULT_SCORE		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_FRAME_RESULT_SCORE	"data/TEXTURE/frame_score.png"	// 読み込むテクスチャファイル名
#define	RESULT_SCORE_SIZE_X		(35.0f)							// スコアの数字の幅
#define	RESULT_SCORE_SIZE_Y		(50.0f)							// スコアの数字の高さ
#define	RESULT_SCORE_INTERVAL_X	(0.0f)							// スコアの数字の表示間隔

#define	NUM_PLACE			(8)								// スコアの桁数

#define	RESULT_SCORE_POS_X			(SCREEN_WIDTH/2 - ((RESULT_SCORE_INTERVAL_X + RESULT_SCORE_SIZE_X) * NUM_PLACE/2) + 10.0f)	// スコアの表示基準位置Ｘ座標
#define	RESULT_SCORE_POS_Y			(SCREEN_HEIGHT/2 + RESULT_SCORE_SIZE_Y)																	// スコアの表示基準位置Ｙ座標

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResultScore(LPDIRECT3DDEVICE9 pDevice);
void SetTextureResultScore(int idx, int number);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResultScore[2] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResultScore = NULL;		// 頂点バッファインターフェースへのポインタ

D3DXVECTOR3				g_posResultScore;						// 位置
D3DXVECTOR3				g_rotResultScore;						// 向き

int						g_ResultScore;						// スコア

														//=============================================================================
														// 初期化処理
														//=============================================================================
HRESULT InitResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posResultScore = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotResultScore = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// スコアの初期化
	g_ResultScore = 0;

	// 頂点情報の作成
	MakeVertexResultScore(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_RESULT_SCORE,			// ファイルの名前
		&g_pD3DTextureResultScore[0]);	// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_FRAME_RESULT_SCORE,		// ファイルの名前
		&g_pD3DTextureResultScore[1]);	// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResultScore(void)
{
	if (g_pD3DTextureResultScore[0] != NULL)
	{// テクスチャの開放
		g_pD3DTextureResultScore[0]->Release();
		g_pD3DTextureResultScore[0] = NULL;
	}
	if (g_pD3DTextureResultScore[1] != NULL)
	{// テクスチャの開放
		g_pD3DTextureResultScore[1]->Release();
		g_pD3DTextureResultScore[1] = NULL;
	}

	if (g_pD3DVtxBuffResultScore != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffResultScore->Release();
		g_pD3DVtxBuffResultScore = NULL;
	}
}

//=============================================================================
// 更新処理
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
// 描画処理
//=============================================================================
void DrawResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffResultScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureResultScore[0]);

	// ポリゴンの描画
	for (int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), NUM_POLYGON);
	}

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureResultScore[1]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (NUM_PLACE * 4), NUM_POLYGON);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResultScore(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * NUM_PLACE + 4),	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,								// 頂点バッファの使用法　
		FVF_VERTEX_2D,									// 使用する頂点フォーマット
		D3DPOOL_MANAGED,								// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffResultScore,							// 頂点バッファインターフェースへのポインタ
		NULL)))											// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + nCntPlace * RESULT_SCORE_SIZE_X + RESULT_SCORE_INTERVAL_X, RESULT_SCORE_POS_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + nCntPlace * (RESULT_SCORE_INTERVAL_X + RESULT_SCORE_SIZE_X) + RESULT_SCORE_SIZE_X, RESULT_SCORE_POS_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + nCntPlace * RESULT_SCORE_SIZE_X + RESULT_SCORE_INTERVAL_X, RESULT_SCORE_POS_Y + RESULT_SCORE_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + nCntPlace * (RESULT_SCORE_INTERVAL_X + RESULT_SCORE_SIZE_X) + RESULT_SCORE_SIZE_X, RESULT_SCORE_POS_Y + RESULT_SCORE_SIZE_Y, 0.0f);

			// rhwの設定
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X - 15, RESULT_SCORE_POS_Y - 25, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + (RESULT_SCORE_INTERVAL_X + RESULT_SCORE_SIZE_X) * NUM_PLACE + 15, RESULT_SCORE_POS_Y - 25, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X - 15, RESULT_SCORE_POS_Y + 55, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RESULT_SCORE_POS_X + (RESULT_SCORE_INTERVAL_X + RESULT_SCORE_SIZE_X) * NUM_PLACE + 15, RESULT_SCORE_POS_Y + 55, 0.0f);

			// rhwの設定
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffResultScore->Unlock();
	}

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureResultScore(int idx, int number)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (idx * 4);

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffResultScore->Unlock();
}

//=============================================================================
// スコアの変更
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

