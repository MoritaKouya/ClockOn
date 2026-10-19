//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#include "aim.h"
#include "input.h"
#include "player.h"

//マクロ定義

#define AIM_SIZE (40) //照準のサイズ

#define AIM_SPEED (10) //照準のスピード

//グローバル変数

LPDIRECT3DTEXTURE9 g_pTextureAim = NULL; //テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAim = NULL; //頂点バッファへのポインタ

D3DXVECTOR3 g_posAim; //位置

D3DXVECTOR3 g_moveAim; //移動量

D3DXVECTOR3 g_RightStick; //右スティックの位置

D3DXVECTOR3 g_Mouse; //マウスの移動量

//==================================
// エイムの初期化処理
//==================================
void InitAim(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得

	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,

		"Data/TEXTURE/AIMING/aiming001.png", //テクスチャのファイル名

		&g_pTextureAim);

	//位置の初期化

	g_posAim = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	//移動量の初期化

	g_moveAim = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//スティックを初期化

	g_RightStick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, //確保するバッファのサイズ
													   //sizeof(VERTEX_2D)*必要な頂点数

		D3DUSAGE_WRITEONLY,

		FVF_VERTEX_2D, //頂点フォーマット

		D3DPOOL_MANAGED,

		&g_pVtxBuffAim,

		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得

	g_pVtxBuffAim->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定

	pVtx[0].pos.x = g_posAim.x - AIM_SIZE;

	pVtx[0].pos.y = g_posAim.y - AIM_SIZE;

	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posAim.x + AIM_SIZE;

	pVtx[1].pos.y = g_posAim.y - AIM_SIZE;

	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posAim.x - AIM_SIZE;

	pVtx[2].pos.y = g_posAim.y + AIM_SIZE;

	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posAim.x + AIM_SIZE;

	pVtx[3].pos.y = g_posAim.y + AIM_SIZE;

	pVtx[3].pos.z = 0.0f;

	//rhwの設定 1.0f固定

	pVtx[0].rhw = 1.0f;

	pVtx[1].rhw = 1.0f;

	pVtx[2].rhw = 1.0f;

	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);

	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする

	g_pVtxBuffAim->Unlock();
}

//==================================
// エイムの終了処理
//==================================
void UninitAim(void)
{
	if (g_pTextureAim != NULL)
	{
		g_pTextureAim->Release();

		g_pTextureAim = NULL;
	}

	//頂点バッファの破棄

	if (g_pVtxBuffAim != NULL)
	{
		g_pVtxBuffAim->Release();

		g_pVtxBuffAim = NULL;
	}
}

//==================================
// エイムの更新処理
//==================================
void UpdateAim(void)
{
	MoveAim();
}

//==================================
// 移動処理
//==================================
void MoveAim(void)
{
	//スティックの入力を獲得

	g_RightStick = GetRightStick();

	if (g_RightStick.x != 0 || g_RightStick.y != 0) 
	{ //入力されている場合

		//移動量の更新

		g_moveAim.x = g_RightStick.x * AIM_SPEED;

		g_moveAim.y = -g_RightStick.y * AIM_SPEED;

	}
	else
	{ //スティック入力されていない場合

		g_Mouse = GetMouseVelocity();

		g_moveAim.x = g_Mouse.x;

		g_moveAim.y = g_Mouse.y;
	}
	
	//位置を更新

	g_posAim.x += g_moveAim.x;

	g_posAim.y += g_moveAim.y;

	//壁に当たった時

	if (g_posAim.x - AIM_SIZE < 0) //一番左に当たった場合
	{
		g_posAim.x = AIM_SIZE; //g_posAimを一番左にする
	}
	else if (SCREEN_WIDTH < g_posAim.x + AIM_SIZE) //一番右に当たった場合
	{
		g_posAim.x = SCREEN_WIDTH - AIM_SIZE; //g_posAimを一番右にする
	}

	if (g_posAim.y - AIM_SIZE < 0) //一番上にあたった場合
	{
		g_posAim.y = AIM_SIZE; //g_posAimを一番上にする
	}
	else if (SCREEN_HEIGHT < g_posAim.y + AIM_SIZE) //一番下にあたった場合
	{
		g_posAim.y = SCREEN_HEIGHT - AIM_SIZE; //g_posAimを一番下にする
	}

	//移動量の更新

	g_moveAim.x += (0.0f - g_moveAim.x) * 0.1f;

	g_moveAim.y += (0.0f - g_moveAim.y) * 0.1f;

	//頂点バッファをロックし、頂点情報へのポインタを取得

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	g_pVtxBuffAim->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = g_posAim.x - AIM_SIZE;

	pVtx[0].pos.y = g_posAim.y - AIM_SIZE;

	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posAim.x + AIM_SIZE;

	pVtx[1].pos.y = g_posAim.y - AIM_SIZE;

	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posAim.x - AIM_SIZE;

	pVtx[2].pos.y = g_posAim.y + AIM_SIZE;

	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posAim.x + AIM_SIZE;

	pVtx[3].pos.y = g_posAim.y + AIM_SIZE;

	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする

	g_pVtxBuffAim->Unlock();
}

//==================================
// エイムの描画処理
//==================================
void DrawAim(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得

	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定

	pDevice->SetStreamSource(0, g_pVtxBuffAim, 0, sizeof(VERTEX_2D));
	//頂点バッファのポインタ  頂点情報構造体のサイズ
//頂点フォーマットの設定

	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定

	pDevice->SetTexture(0, g_pTextureAim);

	//ポリゴンの描画
											  //描画する最初の頂点インデックス
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//==================================
// エイムの位置の獲得処理
//==================================
D3DXVECTOR3 GetPosAim(void)
{
	return g_posAim;
}