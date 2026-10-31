//=============================
// 
//アクションゲームmaster版
// Author森田煌也
// 
//=============================
#include "item.h"

#include "block.h"
#include "camera.h"
#include "scaffold.h"
#include "sound.h"
#include "stoptime.h"
#include "stoptimecounter.h"

//グローバル変数

LPDIRECT3DTEXTURE9 g_apTextureItem[ITEMTYPE_MAX] = {}; //テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL; //頂点バッファへのポインタ

Item g_aItem[MAX_ITEM]; //アイテムの情報

bool g_bItemPosUp; //上昇するか

float g_ItemPosUpCounter; //上昇カウンター

const char* g_aItemTexture_Path[ITEMTYPE_MAX] =
{
	"Data/TEXTURE/ITEM/item_clock.png",

}; //テクスチャの相対パスの構造体

//==================================
// アイテムの初期化処理
//==================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	int nCount;

	//デバイスの取得

	pDevice = GetDevice();

	for (nCount = 0; nCount < ITEMTYPE_MAX; nCount++)
	{

		D3DXCreateTextureFromFile(pDevice,

			g_aItemTexture_Path[nCount], //テクスチャのファイル名

			&g_apTextureItem[nCount]);
	}

	//構造体配列の各項目の初期化

	for (nCount = 0; nCount < MAX_ITEM; nCount++)
	{

		g_aItem[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //位置

		g_aItem[nCount].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //位置

		g_aItem[nCount].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //前フレームの位置
		
		g_aItem[nCount].ScreenPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //画面上の位置
		
		g_aItem[nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //移動量

		g_aItem[nCount].type = ITEMTYPE_00; //種類

		g_aItem[nCount].blanding = false;

		g_aItem[nCount].bUse = false; //使用していない状態にする
	}

	g_bItemPosUp = true;

	g_ItemPosUpCounter = 30;

	//頂点バッファの生成

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM, //確保するバッファのサイズ
													   //sizeof(VERTEX_2D)*必要な頂点数

		D3DUSAGE_WRITEONLY,

		FVF_VERTEX_2D, //頂点フォーマット

		D3DPOOL_MANAGED,

		&g_pVtxBuffItem,

		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得

	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCount = 0; nCount < MAX_ITEM; nCount++, pVtx += 4)
	{
		//頂点座標の設定

		pVtx[0].pos.x = g_aItem[nCount].pos.x - ITEM_SIZE;

		pVtx[0].pos.y = g_aItem[nCount].pos.y - ITEM_SIZE * 2;

		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aItem[nCount].pos.x + ITEM_SIZE;

		pVtx[1].pos.y = g_aItem[nCount].pos.y - ITEM_SIZE * 2;

		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aItem[nCount].pos.x - ITEM_SIZE;

		pVtx[2].pos.y = g_aItem[nCount].pos.y;

		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aItem[nCount].pos.x + ITEM_SIZE;

		pVtx[3].pos.y = g_aItem[nCount].pos.y;

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
	}

	//頂点バッファをアンロックする

	g_pVtxBuffItem->Unlock();
}

//==================================
// アイテムの終了処理
//==================================
void UninitItem(void)
{
	for (int nCount = 0; nCount < ITEMTYPE_MAX; nCount++)
	{
		if (g_apTextureItem[nCount] != NULL)
		{
			g_apTextureItem[nCount]->Release();

			g_apTextureItem[nCount] = NULL;
		}
	}

	//頂点バッファの破棄

	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();

		g_pVtxBuffItem = NULL;
	}
}

//==================================
// アイテムの更新処理
//==================================
void UpdateItem(void)
{
	int nCount;
	for (nCount = 0; nCount < MAX_ITEM; nCount++)
	{
		if (g_aItem[nCount].bUse == true)
		{ //使用されている場合


			MoveItem(nCount); //移動
		}
	}
}

//==================================
// 移動処理
//==================================
void MoveItem(int nCount)
{
	if (GetStopTime()->bStopTime == false)
	{ //時間停止中じゃない場合

	//重力

		g_aItem[nCount].move.y += 0.5f;

		//前フレームの位置を保存

		g_aItem[nCount].posOld = g_aItem[nCount].posOrigin;

		//位置の更新

		g_aItem[nCount].posOrigin.x += g_aItem[nCount].move.x;

		g_aItem[nCount].posOrigin.y += g_aItem[nCount].move.y;

		//ブロックとの当たり判定

		g_aItem[nCount].blanding = CollisionBlock(&g_aItem[nCount].posOrigin, &g_aItem[nCount].posOld, &g_aItem[nCount].move, ITEM_SIZE, ITEM_SIZE * 2);

		//足場の当たり判定

		if (g_aItem[nCount].blanding == false)
		{
			g_aItem[nCount].blanding = CollisionScaffold(&g_aItem[nCount].posOrigin, &g_aItem[nCount].posOld, &g_aItem[nCount].move, ITEM_SIZE, ITEM_SIZE * 2, false);
		}

		//アイテムの浮遊

		if (g_bItemPosUp == true)
		{ //上昇

			//カウンターをマイナス

			g_ItemPosUpCounter -= 0.125;

			//カウンターがゼロになったら

			if (g_ItemPosUpCounter == 0)
			{
				g_bItemPosUp = false;
			}
		}
		else
		{ //下降

			//カウンターをプラス

			g_ItemPosUpCounter += 0.125;

			//カウンターが浮遊間隔分たったら

			if (g_ItemPosUpCounter == 20)
			{
				g_bItemPosUp = true;
			}
		}

		g_aItem[nCount].pos.x = g_aItem[nCount].posOrigin.x;

		g_aItem[nCount].pos.y = g_aItem[nCount].posOrigin.y - 20 - g_ItemPosUpCounter;
	}

	//画面上のアイテムボックスの位置を計算

	g_aItem[nCount].ScreenPos.x = g_aItem[nCount].pos.x - GetPosCamera()->x;

	g_aItem[nCount].ScreenPos.y = g_aItem[nCount].pos.y - GetPosCamera()->y;


	VERTEX_2D* pVtx; //頂点情報へのポインタ

//頂点バッファをロックし、頂点情報へのポインタを取得

	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCount * 4;

	pVtx[0].pos.x = g_aItem[nCount].ScreenPos.x - ITEM_SIZE;

	pVtx[0].pos.y = g_aItem[nCount].ScreenPos.y - ITEM_SIZE * 2;

	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aItem[nCount].ScreenPos.x + ITEM_SIZE;

	pVtx[1].pos.y = g_aItem[nCount].ScreenPos.y - ITEM_SIZE * 2;

	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aItem[nCount].ScreenPos.x - ITEM_SIZE;

	pVtx[2].pos.y = g_aItem[nCount].ScreenPos.y;

	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aItem[nCount].ScreenPos.x + ITEM_SIZE;

	pVtx[3].pos.y = g_aItem[nCount].ScreenPos.y;

	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする

	g_pVtxBuffItem->Unlock();

	if (GetStopTime()->bStopTime == false)
	{ //時間停止中じゃない場合

	//減速

		g_aItem[nCount].move.x += (0.0f - g_aItem[nCount].move.x) * 0.1f;
	}
}

//==================================
// アイテムの描画処理
//==================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得

	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定

	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));
	//頂点バッファのポインタ  頂点情報構造体のサイズ
//頂点フォーマットの設定

	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < MAX_ITEM; nCount++)
	{
		if (g_aItem[nCount].bUse == true)
		{ //使用されている場合
		  
		    //テクスチャの設定

			pDevice->SetTexture(0, g_apTextureItem[g_aItem[nCount].type]);

			//ポリゴンの描画
													  //描画する最初の頂点インデックス
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//==================================
// アイテムの設定処理
//==================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ITEMTYPE type)
{
	int nCount;

	//ポリゴンの描画

	VERTEX_2D* pVtx; //forの前でロック、後でアンロック

	//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCount = 0; nCount < MAX_ITEM; nCount++, pVtx += 4)
	{
		if (g_aItem[nCount].bUse == false)
		{
			//敵の情報の設定(Enemy構造体)

			g_aItem[nCount].pos = pos; //位置

			g_aItem[nCount].posOrigin = pos; //原点

			g_aItem[nCount].move = move; //移動量

			g_aItem[nCount].type = type; //タイプ

			//敵を使用している状態にする

			g_aItem[nCount].bUse = true;

			//頂点座標の設定

			pVtx[0].pos.x = g_aItem[nCount].pos.x - ITEM_SIZE;

			pVtx[0].pos.y = g_aItem[nCount].pos.y - ITEM_SIZE * 2;

			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aItem[nCount].pos.x + ITEM_SIZE;

			pVtx[1].pos.y = g_aItem[nCount].pos.y - ITEM_SIZE * 2;

			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aItem[nCount].pos.x - ITEM_SIZE;

			pVtx[2].pos.y = g_aItem[nCount].pos.y;

			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aItem[nCount].pos.x + ITEM_SIZE;

			pVtx[3].pos.y = g_aItem[nCount].pos.y;

			pVtx[3].pos.z = 0.0f;

			break;
		}
	}

	g_pVtxBuffItem->Unlock();
}

//==================================
// アイテムのヒット処理
//==================================
void HitItem(Item* pItem)
{
	PlaySound(SOUND_LABEL_SE_ITEM);

	pItem->bUse = false; //アイテムを使用していない状態にする

	switch (pItem->type) //タイプごとに比較
	{
	case ITEMTYPE_00: //時計の場合

		HitItemTimer(); //時計の処理
		
		break;
	}
}

//==================================
// タイマーのヒット処理
//==================================
void HitItemTimer(void)
{
	

	//サウンド

	PlaySound(SOUND_LABEL_SE_CLOCK);

	StopTime* pStopTime = GetStopTime(); //時間停止の情報のポインタを獲得

	pStopTime->nCounterStopTime = STOP_TIME_FRAME; //時間停止のカウンターを最大数にリセット

	pStopTime->bStopTimeFlag = true; //時間停止を使用できるようにする
	
	//秒数表示

	SetStopTimeCounter(pStopTime->nCounterStopTime);
}

//==================================
// アイテムの獲得処理
//==================================
Item* GetItem(void)
{
	return &g_aItem[0]; //先頭アドレス
}