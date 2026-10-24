//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#ifndef _BULLET_H_
#define _BULLET_H_

//球の種類の列挙型定義

typedef enum
{
	BULLETTYPE_PLAYER = 0, //プレイヤーの球

	BULLETTYPE_ENEMY, //敵の球

	BULLETTYPE_MAX
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

void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, int nDamage, float fAngle, BULLETTYPE type);

//当たり判定

void CollisionEnemy(Bullet* pBullet); //敵の当たり判定

void CollisionItemBox(Bullet* pBullet); //ブロックの当たり判定

void CollisionBlock(Bullet* pBullet); //ブロックの当たり判定
//
//void CollisionBoss(Bullet* pBullet); //ボスの当たり判定
//
//void CollisionPlayer(Bullet* pBullet); //プレイヤーの当たり判定

#endif