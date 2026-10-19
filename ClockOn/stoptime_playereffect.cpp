//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#include "stoptime_playereffect.h"

#include "player.h"

//マクロ定義

#define MAX_COLUMN_STOPTIMEPLAYER (1) //列数 

#define MAX_LINES_STOPTIMEPLAYER (1) //行数

#define MAX_ANIM_STOPTIMEPLAYER (MAX_COLUMN_STOPTIMEPLAYER * MAX_LINES_STOPTIMEPLAYER) //アニメーションの最大数

//グローバル変数

LPDIRECT3DTEXTURE9 g_pTextureStoptimePlayerEffect = NULL; //テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStoptimePlayerEffect = NULL; //頂点バッファへのポインタ

StoptimePlayerEffect g_StoptimePlayerEffect; //時間停止時のプレイヤーのエフェクトの情報

const char* g_aStoptimePlayerEffectTexture_Path[1] =
{
	"Data/TEXTURE/EFFECT/stoptimeeffect.png"

}; //テクスチャの相対パスの構造体

//==================================
// 時間停止時のプレイヤーのエフェクトの初期化処理
//==================================
void InitStoptimePlayerEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得

	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,

		g_aStoptimePlayerEffectTexture_Path[0], //テクスチャのファイル名

		&g_pTextureStoptimePlayerEffect);

	g_StoptimePlayerEffect.pos = D3DXVECTOR3(110.0f, 110.0f, 0.0f); //位置を初期化

	g_StoptimePlayerEffect.fSize = 0.0f;

	g_StoptimePlayerEffect.bUse = false;

	//頂点バッファの生成

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, //確保するバッファのサイズ
													   //sizeof(VERTEX_2D)*必要な頂点数

		D3DUSAGE_WRITEONLY,

		FVF_VERTEX_2D, //頂点フォーマット

		D3DPOOL_MANAGED,

		&g_pVtxBuffStoptimePlayerEffect,

		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得

	g_pVtxBuffStoptimePlayerEffect->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定

	pVtx[0].pos.x = g_StoptimePlayerEffect.pos.x - g_StoptimePlayerEffect.fSize;

	pVtx[0].pos.y = g_StoptimePlayerEffect.pos.y - g_StoptimePlayerEffect.fSize;

	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_StoptimePlayerEffect.pos.x + g_StoptimePlayerEffect.fSize;

	pVtx[1].pos.y = g_StoptimePlayerEffect.pos.y - g_StoptimePlayerEffect.fSize;

	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_StoptimePlayerEffect.pos.x - g_StoptimePlayerEffect.fSize;

	pVtx[2].pos.y = g_StoptimePlayerEffect.pos.y + g_StoptimePlayerEffect.fSize;

	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_StoptimePlayerEffect.pos.x + g_StoptimePlayerEffect.fSize;

	pVtx[3].pos.y = g_StoptimePlayerEffect.pos.y + g_StoptimePlayerEffect.fSize;

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

	g_pVtxBuffStoptimePlayerEffect->Unlock();
}

//==================================
// 時間停止時のプレイヤーのエフェクトの終了処理
//==================================
void UninitStoptimePlayerEffect(void)
{
	if (g_pTextureStoptimePlayerEffect != NULL)
	{
		g_pTextureStoptimePlayerEffect->Release();

		g_pTextureStoptimePlayerEffect = NULL;
	}

	//頂点バッファの破棄

	if (g_pVtxBuffStoptimePlayerEffect != NULL)
	{
		g_pVtxBuffStoptimePlayerEffect->Release();

		g_pVtxBuffStoptimePlayerEffect = NULL;
	}
}

//==================================
// 時間停止時のプレイヤーのエフェクトの更新処理
//==================================
void UpdateStoptimePlayerEffect(void)
{
	if (g_StoptimePlayerEffect.bUse == true)
	{
		g_StoptimePlayerEffect.fSize += 100;

		if (1280 < g_StoptimePlayerEffect.fSize)
		{
			g_StoptimePlayerEffect.bUse = false;
		}

		VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得

		g_pVtxBuffStoptimePlayerEffect->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定

		pVtx[0].pos.x = g_StoptimePlayerEffect.pos.x - g_StoptimePlayerEffect.fSize;

		pVtx[0].pos.y = g_StoptimePlayerEffect.pos.y - g_StoptimePlayerEffect.fSize;

		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_StoptimePlayerEffect.pos.x + g_StoptimePlayerEffect.fSize;

		pVtx[1].pos.y = g_StoptimePlayerEffect.pos.y - g_StoptimePlayerEffect.fSize;

		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_StoptimePlayerEffect.pos.x - g_StoptimePlayerEffect.fSize;

		pVtx[2].pos.y = g_StoptimePlayerEffect.pos.y + g_StoptimePlayerEffect.fSize;

		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_StoptimePlayerEffect.pos.x + g_StoptimePlayerEffect.fSize;

		pVtx[3].pos.y = g_StoptimePlayerEffect.pos.y + g_StoptimePlayerEffect.fSize;

		pVtx[3].pos.z = 0.0f;

		//テクスチャ座標の設定

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);

		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点バッファをアンロックする

		g_pVtxBuffStoptimePlayerEffect->Unlock();
	}
}

//==================================
// 時間停止時のプレイヤーのエフェクトの描画処理
//==================================
void DrawStoptimePlayerEffect(void)
{

	if (g_StoptimePlayerEffect.bUse == true)
	{

		LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

		//デバイスの取得

		pDevice = GetDevice();

		//頂点バッファをデータストリームに設定

		pDevice->SetStreamSource(0, g_pVtxBuffStoptimePlayerEffect, 0, sizeof(VERTEX_2D));
		//頂点バッファのポインタ  頂点情報構造体のサイズ
	//頂点フォーマットの設定

		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定

		pDevice->SetTexture(0, g_pTextureStoptimePlayerEffect);

		//ポリゴンの描画
												  //描画する最初の頂点インデックス
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//==================================
// 時間停止時のプレイヤーのエフェクトの設定処理
//==================================
void SetStoptimePlayerEffect(void)
{
	//各種項目の設定

	//位置の調整

	Player* pPlayer = GetPlayer();

	g_StoptimePlayerEffect.pos = pPlayer->pos;

	g_StoptimePlayerEffect.pos.y -= PLAYER_SIZE;

	//サイズの初期化

	g_StoptimePlayerEffect.fSize = 0.0f;

	//使用している状態にする

	g_StoptimePlayerEffect.bUse = true;

	
}