//=============================
// 
//アクションゲームmaster版
// Author森田煌也
// 
//=============================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//マクロ定義

#define MAX_ENEMY (32) //敵の最大数

#define ENEMY_SIZE (40) //敵の中心からの対角線の長さ

//マクロ定義

#define MAX_CHARGECOUNTER (10 * 60) //チャージの時間

#define CHARGEEFFECT_INTERVAL (MAX_CHARGECOUNTER / 40) //チャージエフェクトの生成間隔

//敵の状態の列挙型定義

typedef enum
{
	ENEMYSTATE_NORMAL = 0, //通常状態

	ENEMYSTATE_DAMAGE, //ダメージ状態

	ENEMYSTATE_MAX
}ENEMYSTATE;

//敵の種類の列挙型定義

typedef enum
{
	ENEMYTYPE_TUTORIAL = 0,

	ENEMYTYPE_1, //剣持ち

	ENEMYTYPE_2,

	ENEMYTYPE_MAX
}ENEMYTYPE;

//敵の種類の構造体定義

typedef struct
{
	ENEMYTYPE type; //種類

	int nWeapon; //武器

	int nLife; //体力

	float fSpeed; //移動速度
}EnemyInfo;

//敵構造体の定義

typedef struct
{
	D3DXVECTOR3 pos; //位置

	D3DXVECTOR3 posOld; //前の位置

	D3DXVECTOR3 move; //移動

	D3DXVECTOR3 ScreenPos; //スクリーン上の位置

	ENEMYSTATE state; //状態

	EnemyInfo EnemyInfo; //敵の種類

	int nCounterState; //状態カウンター

	int nCounterAction; //行動カウンター

	int nChargeCounter; //チャージのカウンター

	int nPatternAnimEnemy; //アニメーションパターンNo

	int nCountterAnimEnemy; //アニメーションカウンター

	int nAttackPattern; //攻撃パターン

	float nAnglePlayer; //プレイヤーとの角度

	bool bJump; //ジャンプできるかどうか

	bool bRightMove; //向き(true;右向きfalse;左向き)

	bool bUse; //使用しているかどうか
}Enemy;

//プロトタイプ宣言

//初期化

void InitEnemy(void);

void LoadEnemy(void); //敵の情報のロード処理

//終了

void UninitEnemy(void); 

//更新

void UpdateEnemy(void); 

void StateEnemy(int nCnt); //状態

void AnimEnemy(int nCnt); //アニメーション

//行動

void ActionEnemy(int nCnt); //関数呼び出し用

void ActionType1(int nCnt); //行動パターン１

void MoveEnemy(int nCnt); //敵の移動

void Charge(int nCnt); //チャージ

void ChargeAttack(int nCnt);

//描画

void DrawEnemy(void); 

//設定

void SetEnemy(D3DXVECTOR3 pos, int type);

//命中

void HitEnemy(int nCntEnemy, int nDamage); //敵の命中処理

void CollisionPlayer(Enemy* Enemy); //プレイヤーとの当たり判定

//情報の取得

Enemy* GetEnemy(void); //敵の情報の取得

float CalcAngle(D3DXVECTOR3 ppos); //プレイヤーとの角度計算

int GetNumEnemy(void); //敵の数の取得

#endif
