//=============================
// 
//アクションゲーム
// Author森田煌也
// 
//=============================
#ifndef _CHARGEEFFECT_H_
#define _CHARGEEFFECT_H_

#include "main.h"

//チャージエフェクト構造体の定義

typedef struct
{
	D3DXVECTOR3 pos; //位置

	D3DXVECTOR3 EnemyPos; //敵の位置

	D3DXVECTOR3 move; //移動

	D3DXCOLOR col; //色

	float fSize; //サイズ

	bool bUse; //使用しているかどうか
}ChargeEffect;

//プロトタイプ宣言

//初期化

void InitChargeEffect(void);

//終了

void UninitChargeEffect(void);

//更新

void UpdateChargeEffect(void);

//設定

void SetChargeEffect(D3DXVECTOR3 pos, D3DXVECTOR3 EnemyPos, D3DXCOLOR col, float fSize);

#endif