//=============================
// 
//アクションゲームmaster版
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

void Lockon(void); //ロックオン

//描画

void DrawAim(void);

//獲得

D3DXVECTOR3 GetPosAim(void); //エイムの位置の獲得

float GetDistance(D3DXVECTOR3 posEnemy); //敵とエイムの距離の獲得

#endif