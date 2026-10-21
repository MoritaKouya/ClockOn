//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#ifndef _Scaffold_H_
#define _Scaffold_H_

#include "main.h"

//マクロ定義

#define MAX_SCAFFOLD (1024) //足場の最大数

#define SCAFFOLD_SIZE (20) //足場の半径

//足場タイプの列挙型定義

typedef enum
{
	SCAFFOLDTYPE_0 = 0, //通常の足場

	SCAFFOLDTYPE_MAX
}SCAFFOLDTYPE;

//足場構造体の定義

typedef struct
{
	D3DXVECTOR3 pos; //位置

	D3DXVECTOR3 ScreenPos; //スクリーン上の位置

	float fWidth; //幅

	float fHeight; //高さ

	SCAFFOLDTYPE type; //足場のタイプ

	bool bUse; //使用しているかどうか
}Scaffold;

//プロトタイプ宣言

//初期化

void InitScaffold(void);

//終了

void UninitScaffold(void);

//更新

void UpdateScaffold(void);

//描画

void DrawScaffold(void);

//設定

void SetScaffold(D3DXVECTOR3 pos);

//判定

bool CollisionScaffold(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove,
	float fWidth, float fHeight, bool bplayer); //当たり判定

Scaffold* GetScaffold(void);

#endif