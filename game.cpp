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
#include "effect.h"
#include "enemy.h"
#include "fade.h"
#include "goal.h"
#include "guide.h"
#include "input.h"
#include "item.h"
#include "itembox.h"
#include "laser.h"
#include "scaffold.h"
#include "screeneffect.h"
#include "sparkle.h"
#include "stoptime.h"
#include "stoptime_playereffect.h"
#include "player.h"
#include "playereffect.h"

//マクロ定義

#define NUM_MAP (1) //マップのタイプ

//オブジェクトの列挙型定義

typedef enum
{
	OBJECTTYPE_0 = 0, //空白

	OBJECTTYPE_1, //ブロック

	OBJECTTYPE_2, //足場

	OBJECTTYPE_3, //敵

	OBJECTTYPE_4, //ガイド

	OBJECTTYPE_5, //アイテムボックス

	OBJECTTYPE_6, //レーザー

	OBJECTTYPE_MAX
}OBJECTTYPE;

//グローバル変数

const char* g_aStageFile_Path[NUM_MAP] =
{
	"Data/SAVEDATA/STAGE/stage000.csv"
}; //ステージのファイルの相対パス

LPD3DXFONT g_FontTutrial;

float g_StageWidth; //ステージの幅

//==================================
//ゲーム画面の初期化処理 
//==================================
void InitGame(void)
{
	D3DXVECTOR3 posPlayer; //プレイヤーの初期位置を代入

	//デバッグ表示用フォントを生成

	//デバイスの取得

	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	pDevice = GetDevice();

	D3DXCreateFont(pDevice, 18, 0, FW_HEAVY, 0, FALSE, SHIFTJIS_CHARSET,

		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,

		"ＭＳ ゴシック", &g_FontTutrial);

	//+----------------------------
	//|各オブジェクトの初期化処理
	//+----------------------------

	//背景

	InitBg();

	//時間停止

	InitStopTime();

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

	//アイテム

	InitItem(); //アイテム

	InitItemBox(); //アイテムボックス

	//ゴール

	InitGoal();

	//レーザー

	InitLaser();

	//チャージエフェクト

	InitChargeEffect();

	//キラキラ

	InitSparkle();

	//エフェクト

	InitEffect();

	InitScreenEffect(); //画面の演出

	//ステージの読み込み

	posPlayer = LoadStage();

	//プレイヤー

	InitPlayer(posPlayer); //プレイヤー

	InitPlayerEffect(); //プレイヤーのエフェクト

	InitCamera(); //カメラ
}

//=============================
// ステージのロード処理
//=============================
D3DXVECTOR3 LoadStage(void)
{
	FILE* pFile; //ファイルポインタ

	int nCountX, //X座標のカウント

		nCountY, //Y座標のカウント

		nMax_Y, //Y座標の最大数

		nScan = 0; //読み込んだ値を代入する変数

	D3DXVECTOR3 posPlayer; //プレイヤーの位置

	pFile = fopen(g_aStageFile_Path[0], "r");

	if (pFile != NULL)
	{
		//最大数の代入

		(void)fscanf(pFile, "%f%*[,]", &g_StageWidth); //X座標

		(void)fscanf(pFile, "%d%*[,]", &nMax_Y); //Y座標

		for (nCountY = 0; nCountY < nMax_Y; nCountY++)
		{ //Y座標の最大数分繰り返す

			for (nCountX = 0; nCountX < g_StageWidth; nCountX++)
			{ //X座標の最大数分繰り返す

				(void)fscanf(pFile, "%d%*[,]", &nScan);

				switch (nScan / 10) //数値 / 10の値がオブジェクトのタイプ、余りの値がオブジェクトの種類
				{
				case OBJECTTYPE_0: //タイプ０の場合

					if (nScan == 1)
					{
						//プレイヤーの位置を設定

						posPlayer = D3DXVECTOR3(CELL_SIZE + (CELL_SIZE * 2) * (float)nCountX, CELL_SIZE  + (CELL_SIZE * 2) * (float)nCountY, 0.0f);

					}
					else if (nScan == 2)
					{
						SetGoal(D3DXVECTOR3(CELL_SIZE + (CELL_SIZE * 2) * (float)nCountX, GOAL_SIZE / 2 + (CELL_SIZE * 2) * (float)nCountY, 0.0f));
					}

					break;

				case OBJECTTYPE_1: //タイプ１の場合

					//ブロック

					SetBlock(D3DXVECTOR3(CELL_SIZE + (CELL_SIZE * 2) * (float)nCountX, CELL_SIZE + (CELL_SIZE * 2) * (float)nCountY, 0.0f));

					break;

				case OBJECTTYPE_2: //タイプ２の場合

					//足場

					SetScaffold(D3DXVECTOR3(CELL_SIZE + (CELL_SIZE * 2) * (float)nCountX, CELL_SIZE + (CELL_SIZE * 2) * (float)nCountY, 0.0f));

					break;

				case OBJECTTYPE_3: //タイプ３の場合

					//敵

					SetEnemy(D3DXVECTOR3(CELL_SIZE + (CELL_SIZE * 2) * (float)nCountX, (CELL_SIZE * 2) + (CELL_SIZE * 2) * (float)nCountY, 0.0f), (ENEMYTYPE)(nScan % 10));

					break;

				case OBJECTTYPE_4: //タイプ４の場合

					//ガイド

					SetGuide(D3DXVECTOR3(CELL_SIZE + (CELL_SIZE * 2) * (float)nCountX, (CELL_SIZE * 2) + (CELL_SIZE * 2) * (float)nCountY, 0.0f));

					break;

				case OBJECTTYPE_5: //タイプ５の場合

					//アイテムボックス

					SetItemBox(D3DXVECTOR3(CELL_SIZE + (CELL_SIZE * 2) * (float)nCountX, (CELL_SIZE * 2) * (float)nCountY + CELL_SIZE, 0.0f), (ITEMTYPE)(nScan % 10));

					break;

				case OBJECTTYPE_6:

					//レーザー

					SetLaser(D3DXVECTOR3(CELL_SIZE + (CELL_SIZE * 2) * (float)nCountX, (CELL_SIZE * 2) * (float)nCountY, 0.0f), (LASERTYPE)(nScan % 10));

					break;
				}
			}
		}

		fclose(pFile); //ファイルを閉じる
	}

	return posPlayer;

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

	UninitPlayerEffect(); //プレイヤーのエフェクト

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

	//アイテム

	UninitItem(); //アイテム

	UninitItemBox(); //アイテムボックス

	//ゴール

	UninitGoal();

	//レーザー

	UninitLaser();

	//キラキラ

	UninitSparkle();

	//エフェクト

	UninitEffect();

	UninitScreenEffect(); //画面の演出

	//チャージエフェクト

	UninitChargeEffect();

	//アイテム

	UninitItem();

	UninitItemBox();

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

	UpdatePlayerEffect(); //プレイヤーのエフェクト

	//時間停止

	UpdateStopTime();

	UpdateStoptimePlayerEffect(); //時間停止のエフェクト

	//照準

	UpdateAim();

	//ブロック

	UpdateBlock();

	//足場

	UpdateScaffold();

	//球

	UpdateBullet();

	//敵

	UpdateEnemy();

	//ガイド

	UpdateGuide();

	//アイテム

	UpdateItem(); //アイテム

	UpdateItemBox(); //アイテムボックス

	//ゴール

	UpdateGoal();

	//レーザー

	UpdateLaser();

	//キラキラ

	UpdateSparkle();

	//エフェクト

	UpdateEffect();

	UpdateScreenEffect(); //画面の演出

	if (GetStopTime()->bStopTime == false)
	{ //時間停止中じゃない場合

	//チャージエフェクト

		UpdateChargeEffect();
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

	//ゴール

	DrawGoal();

	//ガイド

	DrawGuide();

	//アイテム

	DrawItem();

	DrawItemBox(); //アイテムボックス

	//キラキラ

	DrawSparkle();

	//チャージエフェクト

	DrawChargeEffect();

	//レーザー

	DrawLaser();

	//時計

	DrawStopTime();

	//敵

	DrawEnemy();

	//プレイヤー

	DrawPlayerEffect(); //プレイヤーのエフェクト

	DrawPlayer(); //プレイヤー

	DrawStoptimePlayerEffect(); //時間停止のエフェクト

	//球

	DrawBullet();

	//エフェクト

	DrawEffect();

	DrawScreenEffect(); //画面の演出

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
	RECT rect = { 920, 0,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[256];

	//文字列に代入

	wsprintf(&aStr[0], "操作方法：キーボード / パッド\n""移動：WASD / Lスティックor十字キー\n""照準操作：マウス移動 / Rスティック\n""銃：左クリック / RTボタン\n""タイムストップ：SHIFTキー / LTボタン\n""ダッシュ：右クリック / Aボタン\n""ジャンプ：SPACEキー / Bボタン\n");

	//テキストの描画

	g_FontTutrial->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
}

//==========================
//ステージの幅の獲得処理
//==========================
float GetStageWidth(void)
{
	return g_StageWidth;
}