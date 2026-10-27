//=============================
// 
//アクションゲーム
// Author森田煌也
// 
//=============================
#ifndef _CHARGEEFFECT_H_
#define _CHARGEEFFECT_H_

#include "main.h"

//マクロ定義

#define CHARGEEFFECT_SIZE (100) //チャージエフェクトの半径

//チャージエフェクト構造体の定義

typedef struct
{
	D3DXVECTOR3 pos; //位置

	int nAnimationCounter; //アニメーションカウンター

	int nAnimationPattern; //アニメーションパターン

	bool bUse; //使用しているかどうか
}ChargeEffect;

//プロトタイプ宣言

//初期化

void InitChargeEffect(void);

//終了

void UninitChargeEffect(void);

//更新

void UpdateChargeEffect(void);

//描画

void DrawChargeEffect(void);

//設定

void SetChargeEffect(int nCount);

#endif