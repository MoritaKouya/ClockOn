//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================

#include "scaffold.h"

#include "input.h"

//グローバル変数

LPDIRECT3DTEXTURE9 g_pTextureScaffold = NULL; //テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScaffold = NULL; //頂点バッファへのポインタ

Scaffold g_aScaffold[MAX_SCAFFOLD]; //足場の情報

//================
//足場の初期化処理
//================
void InitScaffold(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	int nCntScaffold;

	//デバイスの取得

	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice,

		"Data/TEXTURE/BLOCK/aa.png", //テクスチャのファイル名

		&g_pTextureScaffold);


	//足場の情報の初期化

	for (nCntScaffold = 0; nCntScaffold < MAX_SCAFFOLD; nCntScaffold++)
	{
		g_aScaffold[nCntScaffold].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_aScaffold[nCntScaffold].fWidth = SCAFFOLD_SIZE; //幅

		g_aScaffold[nCntScaffold].fHeight = SCAFFOLD_SIZE; //高さ

		g_aScaffold[nCntScaffold].bUse = false; //使用しているかどうか
	}

	//頂点バッファの生成

	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_SCAFFOLD, //確保するバッファのサイズ（足場の数分掛ける）

		D3DUSAGE_WRITEONLY,

		FVF_VERTEX_2D, //頂点フォーマット

		D3DPOOL_MANAGED,

		&g_pVtxBuffScaffold,

		NULL
	);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffScaffold->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScaffold = 0; nCntScaffold < MAX_SCAFFOLD; nCntScaffold++, pVtx += 4) //頂点データのポインタを4つ分進める
	{
		//頂点座標の設定

		pVtx[0].pos.x = g_aScaffold[nCntScaffold].pos.x - g_aScaffold[nCntScaffold].fWidth;

		pVtx[0].pos.y = g_aScaffold[nCntScaffold].pos.y - g_aScaffold[nCntScaffold].fHeight;

		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aScaffold[nCntScaffold].pos.x + g_aScaffold[nCntScaffold].fWidth;

		pVtx[1].pos.y = g_aScaffold[nCntScaffold].pos.y - g_aScaffold[nCntScaffold].fHeight;

		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aScaffold[nCntScaffold].pos.x - g_aScaffold[nCntScaffold].fWidth;

		pVtx[2].pos.y = g_aScaffold[nCntScaffold].pos.y;

		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aScaffold[nCntScaffold].pos.x + g_aScaffold[nCntScaffold].fWidth;

		pVtx[3].pos.y = g_aScaffold[nCntScaffold].pos.y;

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

	g_pVtxBuffScaffold->Unlock();
}

//==============
// 足場の終了処理
//==============
void UninitScaffold(void)
{
	//テクスチャの破棄

	if (g_pTextureScaffold != NULL)
	{
		g_pTextureScaffold->Release();

		g_pTextureScaffold = NULL;
	}

	//頂点バッファの破棄

	if (g_pVtxBuffScaffold != NULL)
	{
		g_pVtxBuffScaffold->Release();

		g_pVtxBuffScaffold = NULL;
	}
}

//==============
// 足場の更新処理
//==============
void UpdateScaffold(void)
{

}

//==============
// 足場の描画処理
//==============
void DrawScaffold(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	int nCntScaffold;

	//デバイスの取得

	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定

	pDevice->SetStreamSource(0, g_pVtxBuffScaffold, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定

	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntScaffold = 0; nCntScaffold < MAX_SCAFFOLD; nCntScaffold++)
	{
		if (g_aScaffold[nCntScaffold].bUse == true)
		{ //足場が使用されている場合

			//テクスチャの設定

			pDevice->SetTexture(0, g_pTextureScaffold);

			//ポリゴンの描画
													  //描画を開始する頂点インデックス
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScaffold * 4, 2);
		}
	}
}
//==============
// 足場の設定処理
//==============
void SetScaffold(D3DXVECTOR3 pos)
{
	//サウンド

	//if (type == ScaffoldTYPE_PLAYER)
	//{ //プレイヤーの銃声
	//	PlaySound(SOUND_LABEL_SE_SHOT000);
	//}
	//else
	//{ //敵の銃声
	//	PlaySound(SOUND_LABEL_SE_SHOT001);
	//}
	int nCntScaffold;

	//ポリゴンの描画

	VERTEX_2D* pVtx; //forの前でロック、後でアンロック

	//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffScaffold->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScaffold = 0; nCntScaffold < MAX_SCAFFOLD; nCntScaffold++, pVtx += 4)
	{
		if (g_aScaffold[nCntScaffold].bUse == false)
		{
			//各項目の設定

			g_aScaffold[nCntScaffold].pos = pos; //位置

			g_aScaffold[nCntScaffold].bUse = true; //使用している状態にする

			//頂点座標の設定

			pVtx[0].pos.x = g_aScaffold[nCntScaffold].pos.x - g_aScaffold[nCntScaffold].fWidth;

			pVtx[0].pos.y = g_aScaffold[nCntScaffold].pos.y - g_aScaffold[nCntScaffold].fHeight;

			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aScaffold[nCntScaffold].pos.x + g_aScaffold[nCntScaffold].fWidth;

			pVtx[1].pos.y = g_aScaffold[nCntScaffold].pos.y - g_aScaffold[nCntScaffold].fHeight;

			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aScaffold[nCntScaffold].pos.x - g_aScaffold[nCntScaffold].fWidth;

			pVtx[2].pos.y = g_aScaffold[nCntScaffold].pos.y;

			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aScaffold[nCntScaffold].pos.x + g_aScaffold[nCntScaffold].fWidth;

			pVtx[3].pos.y = g_aScaffold[nCntScaffold].pos.y;

			pVtx[3].pos.z = 0.0f;

			break;
		}
	}

	g_pVtxBuffScaffold->Unlock();
}

//==============
// 足場の当たり判定
//==============
bool CollisionScaffold(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove,
	float fWidth, float fHeight, bool bplayer)
{
	bool bLanding = false; //着地しているかどうか

	for (int nCount = 0; nCount < MAX_SCAFFOLD; nCount++)
	{
		if (g_aScaffold[nCount].bUse == true)
		{
			//縦の当たり判定

			if (g_aScaffold[nCount].pos.x < pPos->x + fWidth

				&& pPos->x - fWidth < g_aScaffold[nCount].pos.x + g_aScaffold[nCount].fWidth)
			{ //横の判定
				if (pPosOld->y <= g_aScaffold[nCount].pos.y - g_aScaffold[nCount].fHeight

					&& g_aScaffold[nCount].pos.y - g_aScaffold[nCount].fHeight < pPos->y )
				{ //上から下
					bLanding = true; //ジャンプが可能

					pPos->y = g_aScaffold[nCount].pos.y - g_aScaffold[nCount].fHeight; //プレイヤーの位置を足場の上にする

					pMove->y = 0.0f;

					//スティックの入力を獲得


					if (bplayer == true)
					{ //プレイヤーの場合

						D3DXVECTOR3 stick = GetLeftStick();

						if (stick.y < -0.5 || GetKeyboardPress(DIK_S) == true) //下方向に入力されている場合
						{
							//移動量の更新

							pPos->y = g_aScaffold[nCount].pos.y + 1;
						}
					}
				}
			}
		}
	}

	return bLanding;
}

//==============
// 足場の情報取得
//==============
Scaffold* GetScaffold(void)
{
	return &g_aScaffold[0];
}