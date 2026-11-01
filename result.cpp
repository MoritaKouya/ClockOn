//=============================
// 
//アクションゲームmaster版
// Author森田煌也
// 
//=============================
#include "result.h"

#include "bg.h"
#include "input.h"
#include "fade.h"
#include "gameresult.h"
#include "pressenter.h"
#include "selectstage.h"
#include "sound.h"
#include "timer.h"

//マクロ定義

#define OUTPUT_GAMERESULT (60) //ゲームリザルトが表示されるまでのフレーム

#define OUTPUT_TIMER (120) //タイマーが表示されるまでのフレーム

#define RESULT_ENTER (180) //決定ボタンが押せるようになるまでのフレーム

//グローバル変数

LPDIRECT3DTEXTURE9 g_pTextureResult = NULL; //テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL; //頂点バッファのポインタ

int nResultCounter; //リザルト画面のカウンター

//==================================
//リザルト画面の初期化処理 
//==================================
void InitResult(void)
{
	PlaySound(SOUND_LABEL_BGM002);

	nResultCounter = 0;

	//各項目の初期化

	InitBg(); //背景

	InitGameResult(); //ゲームリザルト

	if (*GetSelectStage() != STAGE_1_0)
	{ //チュートリアルステージじゃない場合

		InitTimer(); //タイマー
	}

	InitPressEnter(); //プレスエンター
}

//==================================
//リザルト画面の終了処理 
//==================================
void UninitResult(void)
{
	StopSound(SOUND_LABEL_BGM002);
	
	//各項目の終了処理

	UninitBg(); //背景

	UninitGameResult(); //ゲームリザルト

	if (*GetSelectStage() != STAGE_1_0)
	{ //チュートリアルステージじゃない場合
	
		UninitTimer(); //タイマー
	}

	UninitPressEnter(); //プレスエンター
}

//==================================
//リザルト画面の更新処理 
//==================================
void UpdateResult(void)
{
	nResultCounter++;

	if (nResultCounter == OUTPUT_GAMERESULT)
	{ //ゲームリザルトの表示

		SetGameResult();
	}
	else if (nResultCounter == OUTPUT_TIMER && *GetSelectStage() != STAGE_1_0)
	{ //タイマーの表示

		bool* bStartTimer = GetStartTimer();

		*bStartTimer = true;

	}
	else if (RESULT_ENTER == nResultCounter)
	{ //プレスエンターの文字を表示

		SetPressEnter(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 4 * 3 ,0.0f), 260.0f, 50.0f);

	}
	else if (RESULT_ENTER < nResultCounter)
	{ //決定ボタンが押せるようになる

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
		{ //決定キーが押された

			//サウンド

			PlaySound(SOUND_LABEL_SE_ENTER); //決定ボタン

			//モード設定

			SetFade(MODE_TITLE);
		}
	
	}

	//各項目の更新

	UpdateGameResult(); //ゲームリザルト

	UpdatePressEnter(); //プレスエンター
}

//==================================
//リザルト画面の描画処理 
//==================================
void DrawResult(void)
{
	//各項目の描画処理

	DrawBg(); //背景

	DrawGameResult(); //ゲームリザルト

	if (*GetSelectStage() != STAGE_1_0)
	{ //チュートリアルステージじゃない場合

		DrawTimer(); //タイマー
	}

	DrawPressEnter(); //プレスエンター
}