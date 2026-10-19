//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#include "clockhands.h"

#include "clock.h"
#include "stoptime.h"

//グローバル変数

LPDIRECT3DTEXTURE9 g_pTextureClockHands = NULL; //テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffClockHands = NULL; //頂点バッファのポインタ

D3DXVECTOR3 g_posClockHands; //時計の針の情報

int g_AngleClockHands; //針の方向（１２方向）

//==================================
//時計の針の初期化処理 
//==================================
void InitClockHands(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得

	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice,

		"Data/TEXTURE/CLOCK/clockhands.png", //テクスチャのファイル名

		&g_pTextureClockHands);

	//頂点バッファの生成

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, //確保するバッファのサイズ
											   //sizeof(VERTEX_2D)*必要な超点数
		D3DUSAGE_WRITEONLY,

		FVF_VERTEX_2D, //頂点フォーマット

		D3DPOOL_MANAGED,

		&g_pVtxBuffClockHands,

		NULL);

	//時計の針の針の情報の初期化

	g_posClockHands = D3DXVECTOR3(SCREEN_WIDTH / 6 * 5, SCREEN_HEIGHT / 4 * 3, 0.0f);

	g_AngleClockHands = 0;

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得

	g_pVtxBuffClockHands->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定

	pVtx[0].pos.x = g_posClockHands.x + sinf((D3DX_PI / g_AngleClockHands) - D3DX_PI * 0.75f) * CLOCK_SIZE;

	pVtx[0].pos.y = g_posClockHands.y + cosf((D3DX_PI / g_AngleClockHands) - D3DX_PI * 0.75f) * CLOCK_SIZE;

	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posClockHands.x + sinf((D3DX_PI / g_AngleClockHands) + D3DX_PI * 0.75f) * CLOCK_SIZE;

	pVtx[1].pos.y = g_posClockHands.y + cosf((D3DX_PI / g_AngleClockHands) + D3DX_PI * 0.75f) * CLOCK_SIZE;

	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posClockHands.x + sinf((D3DX_PI / g_AngleClockHands) - D3DX_PI * 0.25f) * CLOCK_SIZE;

	pVtx[2].pos.y = g_posClockHands.y + cosf((D3DX_PI / g_AngleClockHands) - D3DX_PI * 0.25f) * CLOCK_SIZE;

	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posClockHands.x + sinf((D3DX_PI / g_AngleClockHands) + D3DX_PI * 0.25f) * CLOCK_SIZE;

	pVtx[3].pos.y = g_posClockHands.y + cosf((D3DX_PI / g_AngleClockHands) + D3DX_PI * 0.25f) * CLOCK_SIZE;

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

	g_pVtxBuffClockHands->Unlock();
}

//==================================
//時計の針の終了処理 
//==================================
void UninitClockHands(void)
{
	//テクスチャの破棄

	if (g_pTextureClockHands != NULL)
	{
		g_pTextureClockHands->Release();

		g_pTextureClockHands = NULL;
	}

	//頂点バッファの破棄

	if (g_pVtxBuffClockHands != NULL)
	{
		g_pVtxBuffClockHands->Release();

		g_pVtxBuffClockHands = NULL;
	}
}

//==================================
//時計の針の更新処理 
//==================================
void UpdateClockHands(void)
{
	g_AngleClockHands = GetStopTime()->nCounterStopTime;

	VERTEX_2D* pVtx; //頂点情報へのポインタ

//頂点バッファをロックし、頂点情報へのポインタを取得

	g_pVtxBuffClockHands->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定

	pVtx[0].pos.x = g_posClockHands.x + sinf((-D3DX_PI * 2 / STOP_TIME_FRAME * g_AngleClockHands) - D3DX_PI * 0.75f) * CLOCK_SIZE;

	pVtx[0].pos.y = g_posClockHands.y + cosf((-D3DX_PI * 2 / STOP_TIME_FRAME * g_AngleClockHands) - D3DX_PI * 0.75f) * CLOCK_SIZE;

	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posClockHands.x + sinf((-D3DX_PI * 2 / STOP_TIME_FRAME * g_AngleClockHands) + D3DX_PI * 0.75f) * CLOCK_SIZE;

	pVtx[1].pos.y = g_posClockHands.y + cosf((-D3DX_PI * 2 / STOP_TIME_FRAME * g_AngleClockHands) + D3DX_PI * 0.75f) * CLOCK_SIZE;

	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posClockHands.x + sinf((-D3DX_PI * 2 / STOP_TIME_FRAME * g_AngleClockHands) - D3DX_PI * 0.25f) * CLOCK_SIZE;

	pVtx[2].pos.y = g_posClockHands.y + cosf((-D3DX_PI * 2 / STOP_TIME_FRAME * g_AngleClockHands) - D3DX_PI * 0.25f) * CLOCK_SIZE;

	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posClockHands.x + sinf((-D3DX_PI * 2 / STOP_TIME_FRAME * g_AngleClockHands) + D3DX_PI * 0.25f) * CLOCK_SIZE;

	pVtx[3].pos.y = g_posClockHands.y + cosf((-D3DX_PI * 2 / STOP_TIME_FRAME * g_AngleClockHands) + D3DX_PI * 0.25f) * CLOCK_SIZE;

	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする

	g_pVtxBuffClockHands->Unlock();
}

//==================================
//時計の針の描画処理 
//==================================
void DrawClockHands(void)
{

		LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

		//デバイスの取得

		pDevice = GetDevice();

		//頂点バッファをデータストリームに設定

		pDevice->SetStreamSource(0, g_pVtxBuffClockHands, 0, sizeof(VERTEX_2D));
		//頂点バッファのポインタ  頂点情報構造体のサイズ

		//頂点フォーマットの設定

		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定

		pDevice->SetTexture(0, g_pTextureClockHands);

		//ポリゴンの描画
												  //描画する最初の頂点インデックス
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}