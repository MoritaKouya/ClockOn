//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#include "chargeeffect.h"

#include "effect.h"
#include "enemy.h"

//マクロ定義

#define MAX_CHARGEEFFECT (1028) //チャージエフェクトの最大数

//グローバル定義

ChargeEffect g_ChargeEffect[MAX_CHARGEEFFECT];

//==================================
// チャージエフェクトの初期化処理
//==================================
void InitChargeEffect(void)
{
	for (int nCount = 0; nCount < MAX_CHARGEEFFECT; nCount++)
	{
		g_ChargeEffect[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_ChargeEffect[nCount].EnemyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_ChargeEffect[nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_ChargeEffect[nCount].bUse = false;
	}
}

//==================================
// チャージエフェクトの終了処理
//==================================
void UninitChargeEffect(void)
{
	//すべて使用していない状態にする

	for (int nCount = 0; nCount < MAX_CHARGEEFFECT; nCount++)
	{
		g_ChargeEffect[nCount].bUse = false;
	}
}

//==================================
// チャージエフェクトの更新処理
//==================================
void UpdateChargeEffect(void)
{
	for (int nCount = 0; nCount < MAX_CHARGEEFFECT; nCount++)
	{
		if (g_ChargeEffect[nCount].bUse == true)
		{ //使用している場合

			//位置を更新

			g_ChargeEffect[nCount].pos.x += g_ChargeEffect[nCount].move.x;

			g_ChargeEffect[nCount].pos.y += g_ChargeEffect[nCount].move.y;

			//エフェクトを追加

			SetEffect(g_ChargeEffect[nCount].pos, D3DXVECTOR3(0.0f,0.0f,0.0f), g_ChargeEffect[nCount].col, g_ChargeEffect[nCount].fSize, 60, false);
		}
	}
}

//==================================
// チャージエフェクトの設定処理
//==================================
void SetChargeEffect(D3DXVECTOR3 pos, D3DXVECTOR3 EnemyPos, D3DXCOLOR col, float fSize)
{
	for (int nCount = 0; nCount < MAX_CHARGEEFFECT; nCount++)
	{
		if(g_ChargeEffect[nCount].bUse == false)
		{ //使用しない場合

			g_ChargeEffect[nCount].pos = pos; //位置

			g_ChargeEffect[nCount].EnemyPos = EnemyPos; //敵の位置

			//移動量の計算

			g_ChargeEffect[nCount].col = col; //色

			g_ChargeEffect[nCount].fSize = fSize; //サイズ

			g_ChargeEffect[nCount].bUse = true; //使用している状態にする
		}
	}
}