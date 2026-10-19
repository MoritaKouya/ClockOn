//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#include "game.h"

#include <stdio.h>

#include "aim.h"
#include "bullet.h"
#include "block.h"
#include "camera.h"
#include "chargeeffect.h"
#include "enemy.h"
#include "fade.h"
#include "input.h"
#include "item.h"
#include "scaffold.h"
#include "stoptime.h"
#include "stoptime_playereffect.h"
#include "player.h"

//グローバル変数

LPDIRECT3DTEXTURE9 g_pTextureGame= NULL; //テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame= NULL; //頂点バッファのポインタ

//==================================
//タイトル画面の初期化処理 
//==================================
void InitGame(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得

	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice,

		"Data/TEXTURE/BG/bg001.png", //テクスチャのファイル名

		&g_pTextureGame);

	//頂点バッファの生成

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, //確保するバッファのサイズ
											   //sizeof(VERTEX_2D)*必要な超点数
		D3DUSAGE_WRITEONLY,

		FVF_VERTEX_2D, //頂点フォーマット

		D3DPOOL_MANAGED,

		&g_pVtxBuffGame,

		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得

	g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);

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

	g_pVtxBuffGame->Unlock();

	//+----------------------------
	//|各オブジェクトの初期化処理
	//+----------------------------

	//時間停止

	InitStopTime();

	//プレイヤー

	InitPlayer(); //プレイヤー

	InitCamera(); //カメラ

	InitStoptimePlayerEffect(); //時間停止のエフェクト

	//球

	InitBullet();

	//照準

	InitAim();

	//ブロック

	InitBlock();

	//足場

	InitScaffold();

	//敵

	InitEnemy();

	//チャージエフェクト

	InitChargeEffect();

	//アイテム

	InitItem();

	SetItem(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(30.0f, -20.0f, 0.0f), ITEMTYPE_00);

	SetItem(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(-30.0f, -20.0f, 0.0f), ITEMTYPE_00);


	//敵呼び出し

	SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 0.0f), ENEMYTYPE_0);

	//ブロック呼び出し

	for (int nCount = 0; nCount < SCREEN_WIDTH / (BLOCK_SIZE * 2) + 2; nCount++)
	{
		SetBlock(D3DXVECTOR3(-BLOCK_SIZE + (BLOCK_SIZE * 2) * (float)nCount, SCREEN_HEIGHT - 40, 0.0f));
	}

	for (int nCount = 0; nCount < SCREEN_WIDTH / (BLOCK_SIZE * 2) + 2; nCount++)
	{
		SetBlock(D3DXVECTOR3(-BLOCK_SIZE + (BLOCK_SIZE * 2) * (float)nCount, SCREEN_HEIGHT, 0.0f));
	}

	for (int nCount = 0; nCount < SCREEN_WIDTH / (BLOCK_SIZE * 2) + 2; nCount++)
	{
		SetScaffold(D3DXVECTOR3(-BLOCK_SIZE + (BLOCK_SIZE * 2) * (float)nCount, SCREEN_HEIGHT / 4 * 2, 0.0f));
	}

	for (int nCount = 0; nCount < SCREEN_WIDTH / (BLOCK_SIZE * 2) + 2; nCount++)
	{
		SetScaffold(D3DXVECTOR3(-BLOCK_SIZE + (BLOCK_SIZE * 2) * (float)nCount, SCREEN_HEIGHT / 4 * 3, 0.0f));
	}

	//ステージの読み込み

	/*LoadStage();*/
}

void LoadStage()
{
	/*FILE* pFile;

	int nCount, nCount2, nScan = 0;

	pFile = fopen("Data/SAVEDATA/STAGE/stage000.csv", "r");

	if (pFile != NULL)
	{

		for (nCount = 0; nScan != 6666; nCount++)
		{
			for (nCount2 = 0; nScan != 666 && nScan != 6666; nCount2++)
			{
				(void)fscanf(pFile, "%d", &nScan);

				switch (nScan)
				{
				case 1:

					SetBlock(D3DXVECTOR3(-BLOCK_SIZE + (BLOCK_SIZE * 2) * (float)nCount, (BLOCK_SIZE * 2) * (float)nCount2, 0.0f));

					break;
				}
			}
		}

		fclose(pFile);
	}*/
}

//==================================
//タイトル画面の終了処理 
//==================================
void UninitGame(void)
{
	//テクスチャの破棄

	if (g_pTextureGame!= NULL)
	{
		g_pTextureGame->Release();

		g_pTextureGame= NULL;
	}

	//頂点バッファの破棄

	if (g_pVtxBuffGame!= NULL)
	{
		g_pVtxBuffGame->Release();

		g_pVtxBuffGame= NULL;
	}

	//+----------------------------
	//|各オブジェクトの終了処理
	//+----------------------------

	//時間停止

	UninitStopTime();

	//プレイヤー

	UninitPlayer();

	UninitCamera(); //カメラ

	UninitStoptimePlayerEffect(); //時間停止のエフェクト

	//球

	UninitBullet();

	//照準

	UninitAim();

	//ブロック

	UninitBlock();

	//足場

	UninitScaffold();

	//敵

	UninitEnemy();

	//チャージエフェクト

	UninitChargeEffect();

	//アイテム

	UninitItem();
}

//==================================
//タイトル画面の更新処理 
//==================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_START))
	{ //決定キーが押された

		//モード設定

		SetFade(MODE_RESULT);
	}

	//+----------------------------
	//|各オブジェクトの更新処理
	//+----------------------------

	//プレイヤー

	UpdateCamera(); //カメラ

	UpdatePlayer(); //プレイヤー

	//時間停止

	UpdateStopTime();

	UpdateStoptimePlayerEffect(); //時間停止のエフェクト

	//照準

	UpdateAim();

	//ブロック

	UpdateBlock();

	//時間停止で止まるもの

	if (GetStopTime()->bStopTime == false)
	{ //時間停止中じゃない場合

	  //足場

		UpdateScaffold();

		//球

		UpdateBullet();

		//敵

		UpdateEnemy();

		//チャージエフェクト

		UpdateChargeEffect();

		//アイテム

		UpdateItem();
	}
}

//==================================
//タイトル画面の描画処理 
//==================================
void DrawGame(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得

	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定

	pDevice->SetStreamSource(0, g_pVtxBuffGame, 0, sizeof(VERTEX_2D));
	//頂点バッファのポインタ  頂点情報構造体のサイズ

	//頂点フォーマットの設定

	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定

	pDevice->SetTexture(0, g_pTextureGame);

	//ポリゴンの描画
											  //描画する最初の頂点インデックス
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//+----------------------------
	//|各オブジェクトの終了処理
	//+----------------------------

	//ブロック

	DrawBlock();

	//足場

	DrawScaffold();

	//アイテム

	DrawItem();

	//時計

	DrawStopTime();

	//チャージエフェクト

	DrawChargeEffect();

	//敵

	DrawEnemy();

	//プレイヤー

	DrawPlayer(); //プレイヤー

	DrawStoptimePlayerEffect(); //時間停止のエフェクト

	//球

	DrawBullet();	

	//操作方法

	DrawHowToUse();

	//照準

	DrawAim();
}

//==========================
// FPSの表示処理
//==========================
void DrawHowToUse(void)
{
	RECT rect = { 0, 580,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[256];

	//文字列に代入

	wsprintf(&aStr[0], "操作方法：キーボード / パッド\n移動：WASD / Lスティックor十字キー\n照準操作：マウス移動 / Rスティック\n銃：左クリック / RTボタン\nタイムストップ：SHIFTキー / LTボタン\nダッシュ：右クリック / Aボタン\nジャンプ：SPACEキー / Bボタン\n");

	//テキストの描画

	LPD3DXFONT font = GetFont();

	font->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}