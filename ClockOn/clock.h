//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "main.h"

//マクロ定義

#define CLOCK_SIZE (100) //時計の半径

//プロトタイプ宣言

void InitClock(void); //初期化

void UninitClock(void); //終了

void UpdateClock(void); //更新

void DrawClock(void); //描画

#endif