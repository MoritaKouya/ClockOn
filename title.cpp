//=============================
// 
//GROWUPPER_master版
// Author森田煌也
// 
//=============================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "bg.h"
#include "titlemenu.h"

//マクロ定義

#define TITLE_SIZE_X (500) //横の半径

#define TITLE_SIZE_Y (150) //縦の半径

//グローバル変数

LPDIRECT3DTEXTURE9 g_pTextureTitle = NULL; //テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL; //頂点バッファのポインタ

D3DXVECTOR3 g_posTitle;

//==================================
//タイトル画面の初期化処理 
//==================================
void InitTitle(void)
{
	g_posTitle = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 0.0f);

	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得

	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice,

		"Data/TEXTURE/TITLE/title.png", //テクスチャのファイル名

		&g_pTextureTitle);

	//頂点バッファの生成

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, //確保するバッファのサイズ
											   //sizeof(VERTEX_2D)*必要な頂点数
		D3DUSAGE_WRITEONLY,

		FVF_VERTEX_2D, //頂点フォーマット

		D3DPOOL_MANAGED,

		&g_pVtxBuffTitle,

		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得

	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定

	pVtx[0].pos.x = g_posTitle.x + sinf(-D3DX_PI * 0.75f) * TITLE_SIZE_X;

	pVtx[0].pos.y = g_posTitle.y + cosf(-D3DX_PI * 0.75f) * TITLE_SIZE_Y;

	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posTitle.x + sinf(D3DX_PI * 0.75f) * TITLE_SIZE_X;

	pVtx[1].pos.y = g_posTitle.y + cosf(D3DX_PI * 0.75f) * TITLE_SIZE_Y;

	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posTitle.x + sinf(-D3DX_PI * 0.25) * TITLE_SIZE_X;

	pVtx[2].pos.y = g_posTitle.y + cosf(-D3DX_PI * 0.25) * TITLE_SIZE_Y;

	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posTitle.x + sinf(D3DX_PI * 0.25) * TITLE_SIZE_X;

	pVtx[3].pos.y = g_posTitle.y + cosf(D3DX_PI * 0.25) * TITLE_SIZE_Y;

	pVtx[3].pos.z = 0.0f;

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

	//各オブジェクトの初期化処理

	InitBg(); //bgの初期化

	InitTitleMenu(); //タイトルメニューの初期化
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

	//各オブジェクトの終了処理

	UninitBg(); //Bg

	UninitTitleMenu(); //タイトルメニュー
}

//==================================
//タイトル画面の更新処理 
//==================================
void UpdateTitle(void)
{
	//フェードの獲得

	FADE fade = GetFade();

	if (fade == FADE_NONE) //フェードしていない場合
	{
		//各オブジェクトの更新処理

		UpdateBg(); //Bg

		UpdateTitleMenu(); //タイトルメニュー

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A))
		{ //決定キーが押された

			switch (int nSelect = GetTitleMenuSelect()) //0で開始1で終了
			{
			case 0: //スタート

				//モード設定

				SetFade(MODE_SELECTSTAGE);

				break;

			case 1: //終了

				UninitTitle();

				//WM_QULTメッセージを送る

				PostQuitMessage(0);

				break;
			}

		}
	}
}

//==================================
//タイトル画面の描画処理 
//==================================
void DrawTitle(void)
{
	//各オブジェクトの描画処理

	DrawBg(); //Bg

	DrawTitleMenu(); //タイトルメニュー

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