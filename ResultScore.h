//=============================================================================
//
// ���U���g��ʏ��� [result.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _RESULT_SCORE_H_
#define _RESULT_SCORE_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResultScore(void);
void UninitResultScore(void);
void UpdateResultScore(void);
void DrawResultScore(void);

void ChangeResultScore(int value);

#endif
