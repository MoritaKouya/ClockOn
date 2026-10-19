//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#include "camera.h"
#include "player.h"

//グローバル変数

D3DXVECTOR3 g_posCamera; //位置

//初期化

void InitCamera(void)
{
	g_posCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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

	if (g_posCamera.x < 0)
	{ //カメラのｙ座標がゼロを下回った場合

		g_posCamera.x = 0;
	}

	if (g_posCamera.y < 0)
	{ //カメラのｙ座標がゼロを下回った場合
		g_posCamera.y = 0;
	}

}

//獲得

D3DXVECTOR3* GetPosCamera(void)
{
	return &g_posCamera;
}