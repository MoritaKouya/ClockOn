//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//マクロ定義

#define MAX_BLOCK (4096) //ブロックの最大数

#define BLOCK_SIZE (20) //ブロックの半径

//ブロックタイプの列挙型定義

typedef enum
{
	BLOCKTYPE_0 = 0, //通常のブロック

	BLOCKTYPE_MAX
}BLOCKTYPE;

//ブロック構造体の定義

typedef struct
{
	D3DXVECTOR3 pos; //位置

	float fWidth; //幅

	float fHeight; //高さ

	BLOCKTYPE type; //ブロックのタイプ

	bool bUse; //使用しているかどうか
}Block;

//プロトタイプ宣言

//初期化

void InitBlock(void);

//終了

void UninitBlock(void);

//更新

void UpdateBlock(void);

//描画

void DrawBlock(void);

//設定

void SetBlock(D3DXVECTOR3 pos);

//判定

bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove,
	float fWidth, float fHeight); //当たり判定

Block* GetBlock(void);

#endif