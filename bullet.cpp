//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================

#include "main.h"

#include "block.h"

#include "bullet.h"

#include "camera.h"

//
//#include "explosion.h"
//
#include "enemy.h"
//
//#include "player.h"
//
//#include "effect.h"
//
//#include "boss.h"
//
#include "sound.h"

#include "stoptime.h"

//マクロ定義

#define MAX_BULLET (128) //球の最大数

#define BULLET_SIZE (20) //球の半径

//グローバル変数

LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL; //テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL; //頂点バッファへのポインタ

Bullet g_aBullet[MAX_BULLET]; //球の情報

//================
//球の初期化処理
//================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	int nCntBullet;

	//デバイスの取得

	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice,

		"Data/TEXTURE/BULLET/bullet000.png", //テクスチャのファイル名

		&g_pTextureBullet);


	//球の情報の初期化

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //位置

		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //移動量

		g_aBullet[nCntBullet].ScreenPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //画面上の位置を初期化

		g_aBullet[nCntBullet].nLife = 0; //寿命

		g_aBullet[nCntBullet].nDamage = 0; //ダメージ

		g_aBullet[nCntBullet].fAngle = 0.0f; //角度

		g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER; //タイプ

		g_aBullet[nCntBullet].bUse = false; //使用していない状態にする
	}

	//頂点バッファの生成

	pDevice->CreateVertexBuffer
	(	
		sizeof(VERTEX_2D) * 4 * MAX_BULLET, //確保するバッファのサイズ（球の数分掛ける）

		D3DUSAGE_WRITEONLY,

		FVF_VERTEX_2D, //頂点フォーマット

		D3DPOOL_MANAGED,

		&g_pVtxBuffBullet,

		NULL
	);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pVtx += 4) //頂点データのポインタを4つ分進める
	{
		//頂点座標の設定

		pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(-D3DX_PI * 0.75f) * BULLET_SIZE;

		pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(-D3DX_PI * 0.75f) * BULLET_SIZE;

		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(D3DX_PI * 0.75f) * BULLET_SIZE;

		pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(D3DX_PI * 0.75f) * BULLET_SIZE;

		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(-D3DX_PI * 0.25) * BULLET_SIZE;

		pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(-D3DX_PI * 0.25) * BULLET_SIZE;

		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(D3DX_PI * 0.25) * BULLET_SIZE;

		pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(D3DX_PI * 0.25) * BULLET_SIZE;

		pVtx[3].pos.z = 0.0f;

		//rhwの設定 1.0f固定

		pVtx[0].rhw = 1.0f;

		pVtx[1].rhw = 1.0f;

		pVtx[2].rhw = 1.0f;

		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定（０．０f～１．０ｆで設定）

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);

		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	g_pVtxBuffBullet->Unlock();
}

//==============
// 球の終了処理
//==============
void UninitBullet(void)
{
	//テクスチャの破棄

	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();

		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄

	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();

		g_pVtxBuffBullet = NULL;
	}
}

//==============
// 球の更新処理
//==============
void UpdateBullet(void)
{
	int nCntBullet;

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//球が使用されている

			//画面上の弾の位置を計算

			g_aBullet[nCntBullet].ScreenPos.x = g_aBullet[nCntBullet].pos.x - GetPosCamera()->x;

			g_aBullet[nCntBullet].ScreenPos.y = g_aBullet[nCntBullet].pos.y - GetPosCamera()->y;

			if (GetStopTime()->bStopTime == false)
			{ //時間停止中じゃない場合

			//移動

				MoveBullet(nCntBullet);

				if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
				{//プレイヤーの球

					////エフェクト

					//SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.75f), 25.0f, 12);

					//当たり判定

					CollisionEnemy(&g_aBullet[nCntBullet]); //敵

					CollisionBlock(&g_aBullet[nCntBullet]); //ブロック

					//CollisionBoss(&g_aBullet[nCntBullet]); //ボス
				}
				else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
				{//敵の球

					////エフェクト

					//SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 0.0f, 0.75f, 1.0f), 25.0f, 12);

					//当たり判定

					//CollisionPlayer(&g_aBullet[nCntBullet]); //プレイヤー
				}
			}


			VERTEX_2D* pVtx;

			//頂点バッファをロックし、頂点データへのポインタを取得

			g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += nCntBullet * 4;

			//頂点座標の更新

			pVtx[0].pos.x = g_aBullet[nCntBullet].ScreenPos.x + sinf(-g_aBullet[nCntBullet].fAngle - D3DX_PI * 0.75f) * BULLET_SIZE;

			pVtx[0].pos.y = g_aBullet[nCntBullet].ScreenPos.y + cosf(-g_aBullet[nCntBullet].fAngle - D3DX_PI * 0.75f) * BULLET_SIZE;

			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].ScreenPos.x + sinf(-g_aBullet[nCntBullet].fAngle + D3DX_PI * 0.75f) * BULLET_SIZE;

			pVtx[1].pos.y = g_aBullet[nCntBullet].ScreenPos.y + cosf(-g_aBullet[nCntBullet].fAngle + D3DX_PI * 0.75f) * BULLET_SIZE;

			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].ScreenPos.x + sinf(-g_aBullet[nCntBullet].fAngle - D3DX_PI * 0.25f) * BULLET_SIZE;

			pVtx[2].pos.y = g_aBullet[nCntBullet].ScreenPos.y + cosf(-g_aBullet[nCntBullet].fAngle - D3DX_PI * 0.25f) * BULLET_SIZE;

			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].ScreenPos.x + sinf(-g_aBullet[nCntBullet].fAngle + D3DX_PI * 0.25f) * BULLET_SIZE;

			pVtx[3].pos.y = g_aBullet[nCntBullet].ScreenPos.y + cosf(-g_aBullet[nCntBullet].fAngle + D3DX_PI * 0.25f) * BULLET_SIZE;

			pVtx[3].pos.z = 0.0f;
		}
		
	}

	
}
//==============
// 球の移動処理
//==============
void MoveBullet(int nCnt)
{


	if (g_aBullet[nCnt].bUse == true)
	{//球が使用されている場合

		//球の位置を更新

		g_aBullet[nCnt].pos.x += g_aBullet[nCnt].move.x;

		g_aBullet[nCnt].pos.y += g_aBullet[nCnt].move.y;

		//画面上の位置を更新

		g_aBullet[nCnt].ScreenPos.x += g_aBullet[nCnt].move.x;

		g_aBullet[nCnt].ScreenPos.y += g_aBullet[nCnt].move.y;

		if (g_aBullet[nCnt].ScreenPos.x < 0 || SCREEN_WIDTH < g_aBullet[nCnt].ScreenPos.x)
		{ //球の位置が画面外に出た
			g_aBullet[nCnt].bUse = false; //使用していない状態にする
		}

		if (g_aBullet[nCnt].ScreenPos.y < 0 || SCREEN_HEIGHT < g_aBullet[nCnt].ScreenPos.y)
		{ //球の位置が画面外に出た
			g_aBullet[nCnt].bUse = false; //使用していない状態にする
		}

		g_pVtxBuffBullet->Unlock();
	}
}

//==============
// 球の描画処理
//==============
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	int nCntBullet;

	//デバイスの取得

	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定

	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定

	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定

	pDevice->SetTexture(0, g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{ //球が使用されている場合

		//ポリゴンの描画
												  //描画を開始する頂点インデックス
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}

	}
}
//==============
// 球の設定処理
//==============
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, int nDamage, float fAngle, BULLETTYPE type) 
{	
	int nCntBullet;

	//ポリゴンの描画

	VERTEX_2D* pVtx; //forの前でロック、後でアンロック

	//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pVtx += 4)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			g_aBullet[nCntBullet].pos = pos; //位置

			g_aBullet[nCntBullet].move = move; //移動量

			g_aBullet[nCntBullet].nLife = nLife; //寿命

			g_aBullet[nCntBullet].nDamage = 1; //ダメージ

			g_aBullet[nCntBullet].fAngle = fAngle;

			g_aBullet[nCntBullet].bUse = true; //使用している状態にする

			g_aBullet[nCntBullet].type = type;

			//頂点座標の更新

			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(-g_aBullet[nCntBullet].fAngle -D3DX_PI * 0.75f) * BULLET_SIZE;

			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(-g_aBullet[nCntBullet].fAngle -D3DX_PI * 0.75f) * BULLET_SIZE;

			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(-g_aBullet[nCntBullet].fAngle+D3DX_PI * 0.75f) * BULLET_SIZE;

			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(-g_aBullet[nCntBullet].fAngle+D3DX_PI * 0.75f) * BULLET_SIZE;

			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(-g_aBullet[nCntBullet].fAngle -D3DX_PI * 0.25f) * BULLET_SIZE;

			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(-g_aBullet[nCntBullet].fAngle -D3DX_PI * 0.25f) * BULLET_SIZE;

			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(-g_aBullet[nCntBullet].fAngle+D3DX_PI * 0.25f) * BULLET_SIZE;

			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(-g_aBullet[nCntBullet].fAngle+D3DX_PI * 0.25f) * BULLET_SIZE;

			pVtx[3].pos.z = 0.0f;

			

			break;
		}
	}

	g_pVtxBuffBullet->Unlock();
}
//
//==================
// 敵との当たり判定
//==================
void CollisionEnemy(Bullet* pBullet)
{
	int nCntEnemy;

	//敵の取得

	Enemy* pEnemy = GetEnemy(); //敵の情報の先頭アドレスが代入される

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//敵が使用されている

			if (pEnemy->pos.x - ENEMY_SIZE <= pBullet->pos.x

				&& pBullet->pos.x <= pEnemy->pos.x + ENEMY_SIZE

				&& pEnemy->pos.y - (ENEMY_SIZE * 2) <= pBullet->pos.y

				&& pBullet->pos.y <= pEnemy->pos.y) //敵と弾が当たった
			{
				//ヒット音

				PlaySound(SOUND_LABEL_SE_HIT000);

				HitEnemy(nCntEnemy, pBullet->nDamage);

				pBullet->bUse = false; //球を使用していない状態にする

				break;
			}
		}
	}
}

//==================
// ブロックとの当たり判定
//==================
void CollisionBlock(Bullet* pBullet)
{
	int nCntEnemy;

	//ブロックの取得

	Block* pBlock = GetBlock(); //ブロックの情報の先頭アドレスが代入される

	for (nCntEnemy = 0; nCntEnemy < MAX_BLOCK; nCntEnemy++, pBlock++)
	{
		if (pBlock->bUse == true)
		{//ブロックが使用されている

			if (pBlock->pos.x - pBlock->fWidth <= pBullet->pos.x

				&& pBullet->pos.x <= pBlock->pos.x + pBlock->fWidth

				&& pBlock->pos.y - pBlock->fHeight <= pBullet->pos.y

				&& pBullet->pos.y <= pBlock->pos.y + pBlock->fHeight) //ブロックと弾が当たった
			{
				//ヒット音

				PlaySound(SOUND_LABEL_SE_HIT001);

				pBullet->bUse = false; //球を使用していない状態にする

				break;
			}
		}
	}
}

////==================
//// ボスとの当たり判定
////==================
//void CollisionBoss(Bullet* pBullet)
//{
//	//ボスの取得
//
//	BOSS* pBoss = GetBoss(); //ボスの情報が代入される
//
//	Player* pPlayer = GetPlayer();
//
//	//中心座標の距離（ｘ、ｙ）
//
//	float fDistance_x = pBoss->pos.x - pBullet->pos.x, //距離ｘ
//
//		fDistance_y = pBoss->pos.y - pBullet->pos.y; //距離y
//
//	//二つの中心中心座標の距離の2乗
//
//	float fDistanceSquared = (fDistance_x * fDistance_x) + (fDistance_y * fDistance_y);
//
//	//２つの円の半径の合計
//
//	float fTotalRadius = (BOSS_COLLISION + BULLET_SIZE); //半径の合計の長さ
//
//	if (fDistanceSquared <= fTotalRadius * fTotalRadius) //半径の合計の２乗より距離の２乗が小さい場合
//	{//敵とプレイヤーが当たった
//
//		//サウンド
//
//		PlaySound(SOUND_LABEL_SE_HIT000);
//
//		HitBoss(pPlayer->nAttack); //ボスのヒット処理
//
//		pBullet->bUse = false; //球を使用していない状態にする
//	}
//}
//
////==================
//// プレイヤーとの当たり判定
////==================
//void CollisionPlayer(Bullet* pBullet)
//{
//	//プレイヤーの取得
//
//	Player* pPlayer = GetPlayer(); //プレイヤーの情報が代入される
//
//	//中心座標の距離（ｘ、ｙ）
//
//	float fDistance_x = pPlayer->pos.x - pBullet->pos.x, //距離ｘ
//
//		fDistance_y = pPlayer->pos.y - pBullet->pos.y; //距離y
//
//	//二つの中心中心座標の距離の2乗
//
//	float fDistanceSquared = (fDistance_x * fDistance_x) + (fDistance_y * fDistance_y);
//
//	//２つの円の半径の合計
//
//	float fTotalRadius = (PLAYER_SIZE + BULLET_SIZE); //半径の合計の長さ
//
//	if (fDistanceSquared <= fTotalRadius * fTotalRadius) //半径の合計の２乗より距離の２乗が小さい場合
//	{//敵とプレイヤーが当たった
//
//		HitPlayer(HIT_ENEMY, pBullet->nDamage); //プレイヤーのヒット処理
//
//		pBullet->bUse = false; //球を使用していない状態にする
//	}
//}