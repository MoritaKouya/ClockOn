//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#include "clock.h"

#include "stoptime.h"

//グローバル変数

LPDIRECT3DTEXTURE9 g_pTextureClock = NULL; //テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffClock = NULL; //頂点バッファのポインタ

D3DXVECTOR3 g_posClock; //時計の情報

//==================================
//時計の初期化処理 
//==================================
void InitClock(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得

	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice,

		"Data/TEXTURE/CLOCK/clock.png", //テクスチャのファイル名

		&g_pTextureClock);

	//頂点バッファの生成

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, //確保するバッファのサイズ
											   //sizeof(VERTEX_2D)*必要な超点数
		D3DUSAGE_WRITEONLY,

		FVF_VERTEX_2D, //頂点フォーマット

		D3DPOOL_MANAGED,

		&g_pVtxBuffClock,

		NULL);

	//時計の情報の初期化

	g_posClock = D3DXVECTOR3(SCREEN_WIDTH / 6 * 5, SCREEN_HEIGHT / 4 * 3, 0.0f);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得

	g_pVtxBuffClock->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定

	pVtx[0].pos.x = g_posClock.x - CLOCK_SIZE;

	pVtx[0].pos.y = g_posClock.y - CLOCK_SIZE;

	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posClock.x + CLOCK_SIZE;

	pVtx[1].pos.y = g_posClock.y - CLOCK_SIZE;

	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posClock.x - CLOCK_SIZE;

	pVtx[2].pos.y = g_posClock.y + CLOCK_SIZE;

	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posClock.x + CLOCK_SIZE;

	pVtx[3].pos.y = g_posClock.y + CLOCK_SIZE;

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

	g_pVtxBuffClock->Unlock();
}

//==================================
//時計の終了処理 
//==================================
void UninitClock(void)
{
	//テクスチャの破棄

	if (g_pTextureClock != NULL)
	{
		g_pTextureClock->Release();

		g_pTextureClock = NULL;
	}

	//頂点バッファの破棄

	if (g_pVtxBuffClock != NULL)
	{
		g_pVtxBuffClock->Release();

		g_pVtxBuffClock = NULL;
	}
}

//==================================
//時計の更新処理 
//==================================
void UpdateClock(void)
{
	bool StopTimeFlag = GetStopTime()->bStopTimeFlag;

	bool StopTimeFlagOld = GetStopTime()->bStopTimeFlagOld;

	if (StopTimeFlag != StopTimeFlagOld)
	{
		VERTEX_2D* pVtx; //頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得

		g_pVtxBuffClock->Lock(0, 0, (void**)&pVtx, 0);

		if (StopTimeFlagOld == false && StopTimeFlag == true)
		{ //前フレームで使用できずに、今回使用できる場合		

			//頂点カラーの設定

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (StopTimeFlagOld == true && StopTimeFlag == false)
		{ //時間停止を前フレームで使用できて、今回使用できない場合

			//頂点カラーの設定

			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}

		//頂点バッファをアンロックする

		g_pVtxBuffClock->Unlock();
	}
}

//==================================
//時計の描画処理 
//==================================
void DrawClock(void)
{
		LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

		//デバイスの取得

		pDevice = GetDevice();

		//頂点バッファをデータストリームに設定

		pDevice->SetStreamSource(0, g_pVtxBuffClock, 0, sizeof(VERTEX_2D));
		//頂点バッファのポインタ  頂点情報構造体のサイズ

		//頂点フォーマットの設定

		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定

		pDevice->SetTexture(0, g_pTextureClock);

		//ポリゴンの描画
												  //描画する最初の頂点インデックス
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}