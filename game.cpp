//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#include "game.h"

#include <stdio.h>

#include "aim.h"
#include "bg.h"
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



const char* g_aStageFile_Path[NUM_MAP] =
{
	"Data/SAVEDATA/STAGE/stage000.csv"
}; //ステージのファイルの相対パス

//==================================
//ゲーム画面の初期化処理 
//==================================
void InitGame(void)
{
	//+----------------------------
	//|各オブジェクトの初期化処理
	//+----------------------------

	//背景

	InitBg();

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

					SetBlock(D3DXVECTOR3(CELL_SIZE + (CELL_SIZE * 2) * (float)nCountX, CELL_SIZE + (CELL_SIZE * 2) * (float)nCountY, 0.0f));

					break;

				case OBJECTTYPE_2: //タイプ２の場合

					SetScaffold(D3DXVECTOR3(CELL_SIZE + (CELL_SIZE * 2) * (float)nCountX, (CELL_SIZE * 2) * (float)nCountY, 0.0f));
				
					break;

				case OBJECTTYPE_3: //タイプ３の場合

					SetEnemy(D3DXVECTOR3(CELL_SIZE + (CELL_SIZE * 2) * (float)nCountX, CELL_SIZE + (CELL_SIZE * 2) * (float)nCountY, 0.0f), (ENEMYTYPE)(nScan % 10));
				
					break;

				case OBJECTTYPE_4:

					SetGuide(D3DXVECTOR3((CELL_SIZE * 2) * (float)nCountX, (CELL_SIZE * 2) * (float)nCountY + CELL_SIZE, 0.0f), nScan % 10);

					break;
				}
			}
		}

		fclose(pFile); //ファイルを閉じる
	}
}

//==================================
//ゲーム画面の終了処理 
//==================================
void UninitGame(void)
{
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

	//背景

	UninitBg();
}

//==================================
//ゲーム画面の更新処理 
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

	

	  //足場

		UpdateScaffold();

		//球

		UpdateBullet();

		//敵

		UpdateEnemy();

		//ガイド

		UpdateGuide();

		if (GetStopTime()->bStopTime == false)
		{ //時間停止中じゃない場合

		//チャージエフェクト

		UpdateChargeEffect();

		//アイテム

		UpdateItem();
	}

	//背景

	UpdateBg();
}

//==================================
//ゲーム画面の描画処理 
//==================================
void DrawGame(void)
{
	//+----------------------------
	//|各オブジェクトの終了処理
	//+----------------------------

	//背景

	DrawBg();

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
// 操作方法の表示処理
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