//=============================
// 
//アクションゲームmaster版
// Author森田煌也
// 
//=============================
#ifndef _BULLET_H_
#define _BULLET_H_

//球の所有者の列挙型定義

typedef enum
{
	BULLETOWNER_PLAYER = 0, //プレイヤーの球

	BULLETOWNER_ENEMY, //敵の球

	BULLETOWNER_MAX
}BULLETOWNER;

//球の種類の列挙型定義

typedef enum
{
	BULLETTYPE_PLAYER = 0, //プレイヤーの弾

	BULLETTYPE_FIRE,       //火の玉

	BULLETTYPE_MAX         //最大数
}BULLETTYPE;

//球構造体の定義

typedef struct
{
	D3DXVECTOR3 pos; //位置

	D3DXVECTOR3 move; //移動量

	D3DXVECTOR3 ScreenPos; //スクリーン上の位置

	int nLife; //寿命

	int nDamage; //ダメージ量

	float fAngle; //角度

	float fSize; //球のサイズ

	BULLETOWNER Owner; //球の所有者

	BULLETTYPE type; //種類

	bool bUse; //使用しているかどうか
}Bullet;

//プロトタイプ宣言

//初期化

void InitBullet(void);

//終了

void UninitBullet(void);

//更新

void UpdateBullet(void);

void MoveBullet(int nCnt); //球の移動

//描画

void DrawBullet(void);

//設定

void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, int nDamage, float fAngle, float fSize, BULLETOWNER Owner, BULLETTYPE type);

//当たり判定

void CollisionEnemy(Bullet* pBullet); //敵の当たり判定

void CollisionItemBox(Bullet* pBullet); //ブロックの当たり判定

void CollisionBlock(Bullet* pBullet); //ブロックの当たり判定
//
//void CollisionBoss(Bullet* pBullet); //ボスの当たり判定
//
//void CollisionPlayer(Bullet* pBullet); //プレイヤーの当たり判定

#endif