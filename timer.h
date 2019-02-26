//=============================================================================
//
// �^�C�}�[���� [timer.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_
#define TIME_MAX             (30)

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);

void EnableTimer(bool bEnable);
void ResetTimer(int nTime = TIME_MAX);

#endif
