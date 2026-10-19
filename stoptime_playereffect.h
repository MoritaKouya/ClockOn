//=============================
// 
//GROWUPPER_master版
// Author森田煌也
// 
//=============================
#ifndef _STOPTIMEPLAYEREFFECT_H_
#define _STOPTIMEPLAYEREFFECT_H_

#include "main.h"

//チャージエフェクト構造体の定義

typedef struct
{
	D3DXVECTOR3 pos; //位置

	float fSize; //大きさ

	bool bUse; //使用しているかどうか
}StoptimePlayerEffect;

//プロトタイプ宣言

//初期化

void InitStoptimePlayerEffect(void);

//終了

void UninitStoptimePlayerEffect(void);

//更新

void UpdateStoptimePlayerEffect(void);

//描画

void DrawStoptimePlayerEffect(void);

//設定

void SetStoptimePlayerEffect(void);

#endif