//=============================
// 
//アクションゲームmaster版
// Author森田煌也
// 
//=============================
#include "chargeeffect.h"

#include "effect.h"
#include "enemy.h"

#include <stdlib.h>
#include <time.h>

//マクロ定義

#define MAX_CHARGEEFFECT (1028) //チャージエフェクトの最大数

#define MAX_LIFE_CHARGEEFFECT (30) //最大寿命

#define NUM_DIRECTION (64) //方向の数

//グローバル定義

ChargeEffect g_ChargeEffect[MAX_CHARGEEFFECT];

//==================================
// チャージエフェクトの初期化処理
//==================================
void InitChargeEffect(void)
{
	//各項目の初期化

	for (int nCount = 0; nCount < MAX_CHARGEEFFECT; nCount++)
	{
		g_ChargeEffect[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //位置

		g_ChargeEffect[nCount].EnemyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //敵の位置

		g_ChargeEffect[nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //移動量

		g_ChargeEffect[nCount].fSize = 0.0f; //大きさ

		g_ChargeEffect[nCount].nLife = 0; //寿命

		g_ChargeEffect[nCount].bUse = false; //使用していない状態にする
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
		
			//寿命

			g_ChargeEffect[nCount].nLife--;

			if (g_ChargeEffect[nCount].nLife <= 0)
			{ //寿命がゼロ以下になった場合

				g_ChargeEffect[nCount].bUse = false; //使用していない状態にする
			}
		}
	}
}

//==================================
// チャージエフェクトの設定処理
//==================================
void SetChargeEffect(D3DXVECTOR3 posEnemy, D3DXCOLOR col, float fSize, float fDistance)
{
	for (int nCount = 0; nCount < MAX_CHARGEEFFECT; nCount++)
	{
		if(g_ChargeEffect[nCount].bUse == false)
		{ //使用しない場合

			g_ChargeEffect[nCount].EnemyPos = posEnemy; // 敵の位置

			// ランダムな方向の角度を計算
			
			int nRand = rand() % NUM_DIRECTION;

			float fAngle = (D3DX_PI * 2.0f / NUM_DIRECTION) * nRand;

			// 敵の周囲に配置
			
			g_ChargeEffect[nCount].pos.x = posEnemy.x + sinf(fAngle) * fDistance;
			
			g_ChargeEffect[nCount].pos.y = posEnemy.y + cosf(fAngle) * fDistance;

			//敵に向かう移動量を計算する

			g_ChargeEffect[nCount].move.x = (posEnemy.x - g_ChargeEffect[nCount].pos.x) / MAX_LIFE_CHARGEEFFECT;
			
			g_ChargeEffect[nCount].move.y = (posEnemy.y - g_ChargeEffect[nCount].pos.y) / MAX_LIFE_CHARGEEFFECT;

			//各項目の設定

			g_ChargeEffect[nCount].col = col; //色
			
			g_ChargeEffect[nCount].fSize = fSize; //サイズ

			g_ChargeEffect[nCount].nLife = MAX_LIFE_CHARGEEFFECT; //寿命
			
			g_ChargeEffect[nCount].bUse = true; //使用している状態にする
	
			break;
		}
	}
}