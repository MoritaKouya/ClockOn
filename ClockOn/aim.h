//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================

#ifndef _AIM_H_

#define _AIM_H_

#include "main.h"

//プロトタイプ宣言

//初期化

void InitAim(void);

//終了

void UninitAim(void);

//更新

void UpdateAim(void);

void MoveAim(void); //移動

//描画

void DrawAim(void);

//獲得

D3DXVECTOR3 GetPosAim(void);

#endif