//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================

#include "block.h"

#include "camera.h"

//グローバル変数

LPDIRECT3DTEXTURE9 g_pTextureBlock = NULL; //テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL; //頂点バッファへのポインタ

Block g_aBlock[MAX_BLOCK]; //ブロックの情報

//================
//ブロックの初期化処理
//================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	int nCntBlock;

	//デバイスの取得

	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice,

		"Data/TEXTURE/BLOCK/block000.png", //テクスチャのファイル名

		&g_pTextureBlock);


	//ブロックの情報の初期化

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_aBlock[nCntBlock].ScreenPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_aBlock[nCntBlock].fWidth = BLOCK_SIZE; //幅

		g_aBlock[nCntBlock].fHeight = BLOCK_SIZE; //高さ

		g_aBlock[nCntBlock].bUse = false; //使用しているかどうか
	}

	//頂点バッファの生成

	pDevice->CreateVertexBuffer
	(	
		sizeof(VERTEX_2D) * 4 * MAX_BLOCK, //確保するバッファのサイズ（ブロックの数分掛ける）

		D3DUSAGE_WRITEONLY,

		FVF_VERTEX_2D, //頂点フォーマット

		D3DPOOL_MANAGED,

		&g_pVtxBuffBlock,

		NULL
	);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pVtx += 4) //頂点データのポインタを4つ分進める
	{
		//頂点座標の設定

		pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth;

		pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight;

		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth;

		pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight;

		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth;

		pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight;

		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth;

		pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight;

		pVtx[3].pos.z = 0.0f;

		//rhwの設定 1.0f固定

		pVtx[0].rhw = 1.0f;

		pVtx[1].rhw = 1.0f;

		pVtx[2].rhw = 1.0f;

		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定（０．０f～１．０ｆで設定）

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);

		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	g_pVtxBuffBlock->Unlock();
}

//==============
// ブロックの終了処理
//==============
void UninitBlock(void)
{
	//テクスチャの破棄

	if (g_pTextureBlock != NULL)
	{
		g_pTextureBlock->Release();

		g_pTextureBlock = NULL;
	}

	//頂点バッファの破棄

	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();

		g_pVtxBuffBlock = NULL;
	}
}

//==============
// ブロックの更新処理
//==============
void UpdateBlock(void)
{
	int nCount;

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCount = 0; nCount < MAX_BLOCK; nCount++, pVtx += 4)
	{
		if (g_aBlock[nCount].bUse == true)
		{
			//画面上のブロックの位置を計算

			g_aBlock[nCount].ScreenPos.x = g_aBlock[nCount].pos.x - GetPosCamera()->x;

			g_aBlock[nCount].ScreenPos.y = g_aBlock[nCount].pos.y - GetPosCamera()->y;

			//頂点座標の設定

			pVtx[0].pos.x = g_aBlock[nCount].ScreenPos.x - g_aBlock[nCount].fWidth;

			pVtx[0].pos.y = g_aBlock[nCount].ScreenPos.y - g_aBlock[nCount].fHeight;

			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBlock[nCount].ScreenPos.x + g_aBlock[nCount].fWidth;

			pVtx[1].pos.y = g_aBlock[nCount].ScreenPos.y - g_aBlock[nCount].fHeight;

			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBlock[nCount].ScreenPos.x - g_aBlock[nCount].fWidth;

			pVtx[2].pos.y = g_aBlock[nCount].ScreenPos.y + g_aBlock[nCount].fHeight;

			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBlock[nCount].ScreenPos.x + g_aBlock[nCount].fWidth;

			pVtx[3].pos.y = g_aBlock[nCount].ScreenPos.y + g_aBlock[nCount].fHeight;

			pVtx[3].pos.z = 0.0f;
		}
	}

	g_pVtxBuffBlock->Unlock();
}

//==============
// ブロックの描画処理
//==============
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	int nCntBlock;

	//デバイスの取得

	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定

	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定

	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{ //ブロックが使用されている場合

			//カメラに写ったら

			if (0 < g_aBlock[nCntBlock].ScreenPos.x && g_aBlock[nCntBlock].ScreenPos.x < SCREEN_WIDTH

				&& 0 < g_aBlock[nCntBlock].ScreenPos.y && g_aBlock[nCntBlock].ScreenPos.y < SCREEN_HEIGHT)
			{

				//テクスチャの設定

				pDevice->SetTexture(0, g_pTextureBlock);

				//ポリゴンの描画
														  //描画を開始する頂点インデックス
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);
			}
		}
	}
}
//==============
// ブロックの設定処理
//==============
void SetBlock(D3DXVECTOR3 pos)
{
	//サウンド

	//if (type == BlockTYPE_PLAYER)
	//{ //プレイヤーの銃声
	//	PlaySound(SOUND_LABEL_SE_SHOT000);
	//}
	//else
	//{ //敵の銃声
	//	PlaySound(SOUND_LABEL_SE_SHOT001);
	//}
	int nCntBlock;

	//ポリゴンの描画

	VERTEX_2D* pVtx; //forの前でロック、後でアンロック

	//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pVtx += 4)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{
			//各項目の設定

			g_aBlock[nCntBlock].pos = pos; //位置

			g_aBlock[nCntBlock].bUse = true; //使用している状態にする

			//頂点座標の更新

			pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth;

			pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight;

			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth;

			pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight;

			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth;

			pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight;

			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x +  g_aBlock[nCntBlock].fWidth;

			pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y +  g_aBlock[nCntBlock].fHeight;

			pVtx[3].pos.z = 0.0f;
		
			break;
		}
	}

	g_pVtxBuffBlock->Unlock();
}

//==============
// ブロックの当たり判定
//==============
bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove,
	float fWidth, float fHeight)
{
	bool bLanding = false; //着地しているかどうか

	for (int nCount = 0; nCount < MAX_BLOCK; nCount++)
	{
		if (g_aBlock[nCount].bUse == true)
		{
			//カメラに写ったら

			if (0 < g_aBlock[nCount].ScreenPos.x && g_aBlock[nCount].ScreenPos.x < SCREEN_WIDTH

				&& 0 < g_aBlock[nCount].ScreenPos.y && g_aBlock[nCount].ScreenPos.y < SCREEN_HEIGHT)
			{

				//縦の当たり判定

				if (g_aBlock[nCount].pos.x - g_aBlock[nCount].fWidth < pPosOld->x + fWidth

					&& pPosOld->x - fWidth < g_aBlock[nCount].pos.x + g_aBlock[nCount].fWidth)
				{ //横の判定
					if (pPosOld->y <= g_aBlock[nCount].pos.y - g_aBlock[nCount].fHeight

						&& g_aBlock[nCount].pos.y - g_aBlock[nCount].fHeight < pPos->y)
					{ //上から下
						bLanding = true; //ジャンプが可能

						pPos->y = g_aBlock[nCount].pos.y - g_aBlock[nCount].fHeight; //プレイヤーの位置をブロックの上にする

						pMove->y = 0.0f;
					}
					else if (g_aBlock[nCount].pos.y + g_aBlock[nCount].fHeight <= pPosOld->y - fHeight

						&& pPos->y - fHeight < g_aBlock[nCount].pos.y + g_aBlock[nCount].fHeight)
					{ //下から上

						pPos->y = g_aBlock[nCount].pos.y + g_aBlock[nCount].fHeight + fHeight; //プレイヤーの位置をブロックの上にする

						pMove->y = 0.0f;
					}
				}

				//横の当たり判定

				if (pPosOld->y - fHeight < g_aBlock[nCount].pos.y + g_aBlock[nCount].fHeight

					&& g_aBlock[nCount].pos.y - g_aBlock[nCount].fHeight < pPosOld->y)
				{ //縦の判定

					if (pPosOld->x + fWidth <= g_aBlock[nCount].pos.x - g_aBlock[nCount].fWidth

						&& g_aBlock[nCount].pos.x - g_aBlock[nCount].fWidth < pPos->x + fWidth)
					{ //左から右
						pPos->x = g_aBlock[nCount].pos.x - g_aBlock[nCount].fWidth - fWidth;

						pMove->x = 0.0f;
					}
					else if (g_aBlock[nCount].pos.x + g_aBlock[nCount].fWidth <= pPosOld->x - fWidth

						&& pPos->x - fWidth < g_aBlock[nCount].pos.x + g_aBlock[nCount].fWidth)
					{ //右から左
						pPos->x = g_aBlock[nCount].pos.x + g_aBlock[nCount].fWidth + fWidth;

						pMove->x = 0.0f;
					}
				}
			}
		}
	}
	return bLanding;
}

//==============
// ブロックの情報取得
//==============
Block* GetBlock(void)
{
	return &g_aBlock[0];
}