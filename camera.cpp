//=============================
// 
//アクションゲームmaster版
// Author森田煌也
// 
//=============================
#include "camera.h"

#include "game.h"
#include "player.h"

//グローバル変数

D3DXVECTOR3 g_posCamera; //位置

float g_StageWidthCamera; //ステージの端

//初期化

void InitCamera(void)
{
	g_posCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (GetMode() == MODE_GAME)
	{ //ゲーム画面の場合

		g_StageWidthCamera = GetStageWidth();

		g_StageWidthCamera *= CELL_SIZE * 2;
	}
}

//終了

void UninitCamera(void)
{

}

//更新

void UpdateCamera(void)
{
	//プレイヤーを真ん中にするように

	Player* pPlayer = GetPlayer();

	g_posCamera.x = pPlayer->pos.x - (SCREEN_WIDTH / 2);

	if (GetMode() == MODE_GAME)
	{ //ゲーム画面の場合

		if (g_posCamera.x < 0)
		{ //カメラのｙ座標がゼロを下回った場合

			g_posCamera.x = 0;
		}
		else if (g_StageWidthCamera < g_posCamera.x + SCREEN_WIDTH)
		{ //カメラの位置が画面端まで来た場合

			g_posCamera.x = g_StageWidthCamera - SCREEN_WIDTH;
		}
	}
}

//獲得

D3DXVECTOR3* GetPosCamera(void)
{
	return &g_posCamera;
}