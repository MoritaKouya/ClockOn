//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#ifndef _CAMERA_H_

#define _CAMERA_H_

#include "main.h"

//プロトタイプ宣言

//初期化

void InitCamera(void);

//終了

void UninitCamera(void);

//更新

void UpdateCamera(void);

//獲得

D3DXVECTOR3* GetPosCamera(void);

#endif