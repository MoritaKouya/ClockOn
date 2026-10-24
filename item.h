//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================

#ifndef _ITEM_H_

#define _ITEM_H_

#include "main.h"

//マクロ定義

#define MAX_ITEM (64)

#define ITEM_SIZE (40) //照準のサイズ

//アイテムの種類の列挙型定義

typedef enum
{
	ITEMTYPE_00 = 0, //時計

	ITEMTYPE_MAX, //最大数
}ITEMTYPE;

//アイテム情報の構造体定義

typedef struct
{
	D3DXVECTOR3 pos; //頂点位置

	D3DXVECTOR3 posOrigin; //原点

	D3DXVECTOR3 posOld; //前フレームの位置

	D3DXVECTOR3 ScreenPos; //スクリーン上の位置

	D3DXVECTOR3 move; //移動量

	ITEMTYPE type; //種類

	bool blanding;

	bool bUse; //使用しているかどうか
	
}Item;

//プロトタイプ宣言

//初期化

void InitItem(void);

//終了

void UninitItem(void);

//更新

void UpdateItem(void);

void MoveItem(int nCount); //移動

//描画

void DrawItem(void);

//設定

void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ITEMTYPE type);

//アイテムのヒット処理

void HitItem(Item* pItem);

void HitItemTimer(void); //時計のヒット処理

//アイテムの獲得処理

Item* GetItem(void);

#endif