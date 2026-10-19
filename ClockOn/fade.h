//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#ifndef _FADE_H_

#define _FADE_H_

#include "main.h"

//フェードの状態

typedef enum
{
	FADE_NONE = 0, //何もしていない状態

	FADE_IN, //フェードイン状態

	FADE_OUT, //フェードアウト状態

	FADE_MAX
}FADE;

//プロトタイプ

void InitFade(MODE modeNext); //初期化

void UninitFade(void); //終了

void UpdateFade(void); //更新

void DrawFade(void); //描画

void SetFade(MODE modeNext); //設定

FADE GetFade(void); //獲得

#endif
