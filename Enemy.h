//=============================================================================
//
// モデル処理 [model.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

D3DXVECTOR3 GetPositionEnemy(void);
D3DXVECTOR3 GetRotationEnemy(void);
D3DXVECTOR3 GetRotationDestEnemy(void);

#endif
