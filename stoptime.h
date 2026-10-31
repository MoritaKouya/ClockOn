//=============================
// 
//アクションゲームmaster版
// Author森田煌也
// 
//=============================
#ifndef _STOPTIME_H_
#define _STOPTIME_H_

#include "main.h"

//マクロ定義

#define STOP_TIME_SEC (12) //時間停止の秒数

#define STOP_TIME_FRAME (STOP_TIME_SEC * 60) //時間停止のフレーム

//時間停止の構造体定義

typedef struct
{
	//グローバル変数

	int nCounterStopTime; //時間停止のカウンター

	bool bStopTimeFlag; //時間停止できるか

	bool bStopTimeFlagOld; //前フレームで停止できていたか（時計の描画の切り替えに使用）

	bool bStopTime; //時間停止しているか
	
}StopTime;

//プロトタイプ宣言

 //初期化

void InitStopTime(void);

//終了

void UninitStopTime(void);

//更新

void UpdateStopTime(void);

void SwitchStopTime(void); //時間停止の切り替え処理

//描画

void DrawStopTime(void);

StopTime* GetStopTime(void); //時間停止のカウンターの獲得処理

#endif