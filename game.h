//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//プロトタイプ宣言

//初期化

void InitGame(void);

void LoadStage(void);

//終了

void UninitGame(void); 

//更新

void UpdateGame(void); 

//描画

void DrawGame(void); 

void DrawHowToUse(void); //プレイ方法

#endif 