//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#include "stoptime.h"

#include "clock.h"
#include "clockhands.h"
#include "input.h"
#include "sound.h"
#include "stoptime_playereffect.h"
#include "stoptimecounter.h"

//マクロ定義

#define CD_INTERVAL (5 * 60) //時間停止の回復間隔

//グローバル変数

StopTime g_StopTime; //時間停止の情報構造体

int g_nStopTimeCD_Counter; //クールダウンのカウンター

//==================================
//時間停止の初期化処理 
//==================================
void InitStopTime(void)
{
	//各項目の初期化処理

	g_StopTime.nCounterStopTime = STOP_TIME_FRAME; //時間停止のカウンターを0にする

	g_StopTime.bStopTimeFlag = true; //時間停止できるようにする
	
	g_StopTime.bStopTimeFlagOld = true; //前フレームは時間停止できている

	g_StopTime.bStopTime = false; //時間停止していない状態にする
	
	g_nStopTimeCD_Counter = 0; //クールダウンのカウンターをゼロにする

	//各オブジェクトの初期化処理

	InitStopTimeCounter(); //時間停止カウンター

	InitClock(); //時計

	InitClockHands(); //時計の針

}

//==================================
//時間停止の終了処理 
//==================================
void UninitStopTime(void)
{
	//各オブジェクトの初期化処理

	UninitStopTimeCounter(); //時間停止カウンター

	UninitClock(); //時計

	UninitClockHands(); //時計の針
}

//==================================
//時間停止の更新処理 
//==================================
void UpdateStopTime(void)
{
	//時間停止の切り替え

	g_StopTime.bStopTimeFlagOld = g_StopTime.bStopTime; //前フレームの状態を保存

	SwitchStopTime(); //切り替えの処理

	if (g_StopTime.bStopTime == true)
	{ //時間停止中の処理

		//カウンターを減らす

		g_StopTime.nCounterStopTime--;

		if (g_StopTime.nCounterStopTime == 0)
		{ //カウンターがゼロになった場合

		  //時間停止を終了する

			g_StopTime.bStopTime = false; //時間停止していない状態にする

			g_StopTime.bStopTimeFlag = false; //時間停止できないようにする

			StopSound(SOUND_LABEL_CLOCK); //サウンド
		}
		else if (g_StopTime.nCounterStopTime % 60 == 0)
		{ //カウンターが60で割れる場合

			//カウンターを画面に表示する

			SetStopTimeCounter(g_StopTime.nCounterStopTime / 60);
		}
	}
	else
	{ //時間停止中じゃないときの処理

		if (g_StopTime.nCounterStopTime <= STOP_TIME_FRAME)
		{ //カウンターが最大数じゃない場合

			g_nStopTimeCD_Counter++; //クールダウンのカウンターをプラス

			if (g_nStopTimeCD_Counter == 5)
			{ //時間停止の回復間隔分たった場合

				g_nStopTimeCD_Counter = 0; //クールダウンのカウンターをリセット

				g_StopTime.nCounterStopTime++; //カウンターを増やす

				if (g_StopTime.nCounterStopTime % 60 == 0)
				{
					PlaySound(SOUND_LABEL_SE_CLOCK);
				}

				if (STOP_TIME_FRAME == g_StopTime.nCounterStopTime)
				{ //カウンターが最大数になったら

					g_StopTime.bStopTimeFlag = true; //時間停止を可能にする
				}
			}
		}
	}
	//各オブジェクトの更新処理

	UpdateStopTimeCounter(); //時間停止カウンター

	UpdateClock(); //時計

	UpdateClockHands(); //時計の針
}

//==================================
//時間停止の切り替え処理
//==================================
void SwitchStopTime(void)
{
	if (g_StopTime.bStopTimeFlag == true)
	{ //時間停止できる場合

		if (GetLTTrigger() == true || GetKeyboardTrigger(DIK_LSHIFT) || GetKeyboardTrigger(DIK_RSHIFT))
		{ //時間停止のボタンが押された場合

			if (g_StopTime.bStopTime == false)
			{ //時間停止していない場合

				//時間停止の処理

				g_StopTime.bStopTime = true; //時間停止している状態にする

				SetStoptimePlayerEffect(); //エフェクト

				PlaySound(SOUND_LABEL_CLOCK); //サウンド
			}
			else
			{ //時間停止している場合

				//時間停止を終了する

				g_StopTime.bStopTime = false; //時間停止していない状態にする

				g_StopTime.bStopTimeFlag = false; //時間停止できないようにする

				StopSound(SOUND_LABEL_CLOCK); //サウンド
			}
		}
	}
}

//==================================
//時間停止の描画処理 
//==================================
void DrawStopTime(void)
{
	//各オブジェクトの初期化処理

	DrawStopTimeCounter(); //時間停止カウンター

	DrawClock(); //時計

	DrawClockHands(); //時計の針
}

//==================================
//時間停止のカウンターの獲得処理
//==================================
StopTime* GetStopTime(void)
{
	return &g_StopTime;
}