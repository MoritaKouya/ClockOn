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
#include "guide.h"
#include "input.h"
#include "item.h"
#include "scaffold.h"
#include "stoptime.h"
#include "stoptime_playereffect.h"
#include "player.h"

//マクロ定義

#define CELL_SIZE (20) //マスの半径

#define NUM_MAP (1) //マップのタイプ

//オブジェクトの列挙型定義

typedef enum
{
	OBJECTTYPE_0 = 0, //空白

	OBJECTTYPE_1, //ブロック

	OBJECTTYPE_2, //足場

	OBJECTTYPE_3, //敵

	OBJECTTYPE_4, //ガイド

	OBJECTTYPE_MAX
}OBJECTTYPE;

//グローバル変数

LPDIRECT3DTEXTURE9 g_pTextureGame= NULL; //テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame= NULL; //頂点バッファのポインタ

const char* g_aStageFile_Path[NUM_MAP] =
{
	"Data/SAVEDATA/STAGE/stage000.csv"
}; //ステージのファイルの相対パス

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

	//ガイド

	InitGuide();

	//チャージエフェクト

	InitChargeEffect();

	//ステージの読み込み

	LoadStage();
}

//=============================
// ステージのロード処理
//=============================
void LoadStage()
{
	FILE* pFile;

	int nCountX,nCountY,nMax_X, nMax_Y, nScan = 0;

	pFile = fopen(g_aStageFile_Path[0], "r");

	if (pFile != NULL)
	{
		//最大数の代入

		(void)fscanf(pFile, "%d%*[,]", &nMax_X); //X座標

		(void)fscanf(pFile, "%d%*[,]", &nMax_Y); //Y座標

		for (nCountY = 0; nCountY < nMax_Y; nCountY++)
		{ //Y座標の最大数分繰り返す

			for (nCountX = 0; nCountX < nMax_X; nCountX++)
			{ //X座標の最大数分繰り返す
				(void)fscanf(pFile, "%d%*[,]", &nScan);

				switch (nScan /10) //数値 / 10の値がオブジェクトのタイプ、余りの値がオブジェクトの種類
				{
				case OBJECTTYPE_0: //タイプ０の場合

					break; //空白

				case OBJECTTYPE_1: //タイプ１の場合

					SetBlock(D3DXVECTOR3((CELL_SIZE * 2) * (float)nCountX, (CELL_SIZE * 2) * (float)nCountY, 0.0f));

					break;

				case OBJECTTYPE_2: //タイプ２の場合

					SetScaffold(D3DXVECTOR3((CELL_SIZE * 2) * (float)nCountX, (CELL_SIZE * 2) * (float)nCountY, 0.0f));
				
					break;

				case OBJECTTYPE_3: //タイプ３の場合

					SetEnemy(D3DXVECTOR3((CELL_SIZE * 2) * (float)nCountX, (CELL_SIZE * 2) * (float)nCountY + CELL_SIZE, 0.0f), (ENEMYTYPE)(nScan % 10));
				
					break;

				case OBJECTTYPE_4:

					SetGuide(D3DXVECTOR3((CELL_SIZE * 2) * (float)nCountX, (CELL_SIZE * 2) * (float)nCountY + CELL_SIZE, 0.0f));

					break;
				}
			}
		}

		fclose(pFile); //ファイルを閉じる
	}
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
	
	//ガイド

	UninitGuide();

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

		//ガイド

		UpdateGuide();

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

	//ガイド

	DrawGuide();

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