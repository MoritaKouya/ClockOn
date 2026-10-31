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
#include "sound.h"

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
}

//==================================
//リザルト画面の更新処理 
//==================================
void UpdateResult(void)
{
	nResultCounter++;

	if (nResultCounter == 60)
	{
		SetGameResult();
	}
	else if (60 < nResultCounter)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
		{ //決定キーが押された

			//サウンド

			PlaySound(SOUND_LABEL_SE_ENTER); //決定ボタン

			//モード設定

			SetFade(MODE_TITLE);
		}
	}

	

	//各項目の更新

	UpdateGameResult();
}

//==================================
//リザルト画面の描画処理 
//==================================
void DrawResult(void)
{
	//各項目の描画処理

	DrawBg(); //背景

	DrawGameResult();
}