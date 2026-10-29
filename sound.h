//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,         // BGM1
	SOUND_LABEL_BGM002,         // BGM1


	SOUND_LABEL_BGM_1_0,        //1-0

	SOUND_LABEL_CLOCK,          // 時計0
	SOUND_LABEL_SE_CLOCK,       // 時計のサウンド0

	SOUND_LABEL_SE_ARROW,       //矢印ボタン
	SOUND_LABEL_SE_ENTER,       //決定ボタン

	SOUND_LABEL_SE_SHOT,		// 弾発射音

	SOUND_LABEL_SE_HIT000,      // ヒット音（敵）
	SOUND_LABEL_SE_HIT001,      // ヒット音（床）
	
	SOUND_LABEL_SE_DASH,        // ダッシュ
	SOUND_LABEL_SE_AVOIDANCE,   // 回避音
	SOUND_LABEL_SE_JUMP,        //ジャンプ
	SOUND_LABEL_SE_FOOTSTEPS,   //足音
	SOUND_LABEL_SE_LOCKON,      //ロックオン

	SOUND_LABEL_SE_ITEM,        //アイテム
	SOUND_LABEL_SE_ITEMBOX,     //アイテムボックス

	SOUND_LABEL_SE_GOAL,        //ゴール

	SOUND_LABEL_SE_GAMERESULT,  //ゲームリザルト

	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
