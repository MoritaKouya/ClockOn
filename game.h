//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//マクロ定義

#define CELL_SIZE (20) //マスの半径

//プロトタイプ宣言

//初期化

void InitGame(void);

D3DXVECTOR3 LoadStage(void);

//終了

void UninitGame(void); 

//更新

void UpdateGame(void); 

//描画

void DrawGame(void); 

void DrawHowToUse(void); //プレイ方法

//獲得

float GetStageWidth(void);

#endif 