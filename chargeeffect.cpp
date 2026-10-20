//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#include "chargeeffect.h"

#include "enemy.h"

//マクロ定義

#define MAX_COLUMN_CHARGE (4) //列数 

#define MAX_LINES_CHARGE (2) //行数

#define MAX_ANIM_CHARGE (MAX_COLUMN_CHARGE * MAX_LINES_CHARGE) //アニメーションの最大数

//グローバル変数

LPDIRECT3DTEXTURE9 g_pTextureChargeEffect = NULL; //テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChargeEffect = NULL; //頂点バッファへのポインタ

ChargeEffect g_ChargeEffect; //プレイヤーの情報

const char* g_aChargeEffectTexture_Path[1] =
{
	"Data/TEXTURE/EFFECT/chargeeffect.png"

}; //テクスチャの相対パスの構造体

//==================================
// プレイヤーの初期化処理
//==================================
void InitChargeEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得

	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,

		g_aChargeEffectTexture_Path[0], //テクスチャのファイル名

		&g_pTextureChargeEffect);

	g_ChargeEffect.pos = D3DXVECTOR3(110.0f, 110.0f, 0.0f); //位置を初期化

	g_ChargeEffect.nAnimationCounter = 0;

	g_ChargeEffect.nAnimationPattern = 0;

	g_ChargeEffect.bUse = false;

	//頂点バッファの生成

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 , //確保するバッファのサイズ
													   //sizeof(VERTEX_2D)*必要な頂点数

		D3DUSAGE_WRITEONLY,

		FVF_VERTEX_2D, //頂点フォーマット

		D3DPOOL_MANAGED,

		&g_pVtxBuffChargeEffect,

		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得

	g_pVtxBuffChargeEffect->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定

	pVtx[0].pos.x = g_ChargeEffect.pos.x - CHARGEEFFECT_SIZE;

	pVtx[0].pos.y = g_ChargeEffect.pos.y - CHARGEEFFECT_SIZE;

	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_ChargeEffect.pos.x + CHARGEEFFECT_SIZE;

	pVtx[1].pos.y = g_ChargeEffect.pos.y - CHARGEEFFECT_SIZE;

	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_ChargeEffect.pos.x - CHARGEEFFECT_SIZE;

	pVtx[2].pos.y = g_ChargeEffect.pos.y + CHARGEEFFECT_SIZE;

	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_ChargeEffect.pos.x + CHARGEEFFECT_SIZE;

	pVtx[3].pos.y = g_ChargeEffect.pos.y + CHARGEEFFECT_SIZE;

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

	pVtx[0].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_CHARGE) * (g_ChargeEffect.nAnimationPattern % MAX_COLUMN_CHARGE), (1.0f / MAX_LINES_CHARGE) * (g_ChargeEffect.nAnimationPattern / MAX_COLUMN_CHARGE));

	pVtx[1].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_CHARGE) * ((g_ChargeEffect.nAnimationPattern % MAX_COLUMN_CHARGE) + 1.0f), (1.0f / MAX_LINES_CHARGE) * (g_ChargeEffect.nAnimationPattern / MAX_COLUMN_CHARGE));

	pVtx[2].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_CHARGE) * (g_ChargeEffect.nAnimationPattern % MAX_COLUMN_CHARGE), (1.0f / MAX_LINES_CHARGE) * (g_ChargeEffect.nAnimationPattern / MAX_COLUMN_CHARGE) + (1.0f / MAX_LINES_CHARGE));

	pVtx[3].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_CHARGE) * ((g_ChargeEffect.nAnimationPattern % MAX_COLUMN_CHARGE) + 1.0f), (1.0f / MAX_LINES_CHARGE) * (g_ChargeEffect.nAnimationPattern / MAX_COLUMN_CHARGE) + (1.0f / MAX_LINES_CHARGE));

	//頂点バッファをアンロックする

	g_pVtxBuffChargeEffect->Unlock();
}

//==================================
// プレイヤーの終了処理
//==================================
void UninitChargeEffect(void)
{
	if (g_pTextureChargeEffect != NULL)
	{
		g_pTextureChargeEffect->Release();

		g_pTextureChargeEffect = NULL;
	}

	//頂点バッファの破棄

	if (g_pVtxBuffChargeEffect != NULL)
	{
		g_pVtxBuffChargeEffect->Release();

		g_pVtxBuffChargeEffect = NULL;
	}
}

//==================================
// プレイヤーの更新処理
//==================================
void UpdateChargeEffect(void)
{
	if (g_ChargeEffect.bUse == true)
	{
		//アニメーション

		g_ChargeEffect.nAnimationCounter++;

		if (g_ChargeEffect.nAnimationCounter >= 5)
		{
			g_ChargeEffect.nAnimationCounter = 0;

			g_ChargeEffect.nAnimationPattern++;

			g_ChargeEffect.nAnimationPattern %= MAX_ANIM_CHARGE;
		}

		VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得

		g_pVtxBuffChargeEffect->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定

		pVtx[0].pos.x = g_ChargeEffect.pos.x - CHARGEEFFECT_SIZE;

		pVtx[0].pos.y = g_ChargeEffect.pos.y - CHARGEEFFECT_SIZE;

		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_ChargeEffect.pos.x + CHARGEEFFECT_SIZE;

		pVtx[1].pos.y = g_ChargeEffect.pos.y - CHARGEEFFECT_SIZE;

		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_ChargeEffect.pos.x - CHARGEEFFECT_SIZE;

		pVtx[2].pos.y = g_ChargeEffect.pos.y + CHARGEEFFECT_SIZE;

		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_ChargeEffect.pos.x + CHARGEEFFECT_SIZE;

		pVtx[3].pos.y = g_ChargeEffect.pos.y + CHARGEEFFECT_SIZE;

		pVtx[3].pos.z = 0.0f;

		//テクスチャ座標の設定

		pVtx[0].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_CHARGE) * (g_ChargeEffect.nAnimationPattern % MAX_COLUMN_CHARGE), (1.0f / MAX_LINES_CHARGE) * (g_ChargeEffect.nAnimationPattern / MAX_COLUMN_CHARGE));

		pVtx[1].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_CHARGE) * ((g_ChargeEffect.nAnimationPattern % MAX_COLUMN_CHARGE) + 1.0f), (1.0f / MAX_LINES_CHARGE) * (g_ChargeEffect.nAnimationPattern / MAX_COLUMN_CHARGE));

		pVtx[2].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_CHARGE) * (g_ChargeEffect.nAnimationPattern % MAX_COLUMN_CHARGE), (1.0f / MAX_LINES_CHARGE) * (g_ChargeEffect.nAnimationPattern / MAX_COLUMN_CHARGE) + (1.0f / MAX_LINES_CHARGE));

		pVtx[3].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_CHARGE) * ((g_ChargeEffect.nAnimationPattern % MAX_COLUMN_CHARGE) + 1.0f), (1.0f / MAX_LINES_CHARGE) * (g_ChargeEffect.nAnimationPattern / MAX_COLUMN_CHARGE) + (1.0f / MAX_LINES_CHARGE));

		//頂点バッファをアンロックする

		g_pVtxBuffChargeEffect->Unlock();
	}
}

//==================================
// プレイヤーの描画処理
//==================================
void DrawChargeEffect(void)
{

	if (g_ChargeEffect.bUse == true)
	{

		LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

		//デバイスの取得

		pDevice = GetDevice();

		//頂点バッファをデータストリームに設定

		pDevice->SetStreamSource(0, g_pVtxBuffChargeEffect, 0, sizeof(VERTEX_2D));
		//頂点バッファのポインタ  頂点情報構造体のサイズ
	//頂点フォーマットの設定

		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定

		pDevice->SetTexture(0, g_pTextureChargeEffect);

		//ポリゴンの描画
												  //描画する最初の頂点インデックス
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}