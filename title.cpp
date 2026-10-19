//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#include "title.h"
#include "input.h"
#include "fade.h"

//グローバル変数

LPDIRECT3DTEXTURE9 g_pTextureTitle = NULL; //テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL; //頂点バッファのポインタ

//==================================
//タイトル画面の初期化処理 
//==================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得

	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice,

		"Data/TEXTURE/title_template.png", //テクスチャのファイル名

		&g_pTextureTitle);

	//頂点バッファの生成

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, //確保するバッファのサイズ
											   //sizeof(VERTEX_2D)*必要な超点数
		D3DUSAGE_WRITEONLY,

		FVF_VERTEX_2D, //頂点フォーマット

		D3DPOOL_MANAGED,

		&g_pVtxBuffTitle,

		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得

	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);

	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);

	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定 1.0f固定

	pVtx[0].rhw = 1.0f;

	pVtx[1].rhw = 1.0f;

	pVtx[2].rhw = 1.0f;

	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);

	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする

	g_pVtxBuffTitle->Unlock();

}

//==================================
//タイトル画面の終了処理 
//==================================
void UninitTitle(void)
{
	//テクスチャの破棄

	if (g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();

		g_pTextureTitle = NULL;
	}

	//頂点バッファの破棄

	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();

		g_pVtxBuffTitle = NULL;
	}
}

//==================================
//タイトル画面の更新処理 
//==================================
void UpdateTitle(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_B) == true)
	{ //決定キーが押された

		//モード設定

		SetFade(MODE_GAME);
	}
}

//==================================
//タイトル画面の描画処理 
//==================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得

	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定

	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));
	//頂点バッファのポインタ  頂点情報構造体のサイズ

	//頂点フォーマットの設定

	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定

	pDevice->SetTexture(0, g_pTextureTitle);

	//ポリゴンの描画
											  //描画する最初の頂点インデックス
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}