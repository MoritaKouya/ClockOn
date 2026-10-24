//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#include "player.h"

#include "camera.h"
#include "effect.h"
#include "fade.h"
#include "input.h"
#include "item.h"
#include "bullet.h"
#include "aim.h"
#include "block.h"
#include "laser.h"
#include "scaffold.h"
#include "sound.h"

//#include "hpber.h"

//マクロ定義

#define PLAYER_SPEED (1) //プレイヤーの速度

#define SHOOT_RECOIL (3) //球の反動

#define MAX_PLAYERTEXTURE (1) //プレイヤーのテクスチャ数

//アニメーション

#define MAX_ANIM_PLAYER (6) //アニメーションの最大数

#define MAX_COLUMN_PLAYER (6) //列数 

#define MAX_LINES_PLAYER (2) //行数

#define ANIM_INTERVAL_PLAYER (12) //アニメーションの間隔

//グローバル変数

LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL; //テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL; //頂点バッファへのポインタ

int g_nCountterAnimPlayer; //アニメーションカウンター

int g_nPatternAnimPlayer; //アニメーションパターンNo,

float g_fAnglePlayer; //対角線の角度

Player g_player; //プレイヤーの情報

bool g_rot; //trueで左falseで右

D3DXVECTOR3 g_stick; //スティックの情報を代入する処理

const char* g_aPlayerTexture_Path[MAX_PLAYERTEXTURE] =
{
	"Data/TEXTURE/PLAYER/player001.png",
}; //テクスチャの相対パスの構造体

//==================================
// プレイヤーの初期化処理
//==================================
void InitPlayer(D3DXVECTOR3 posPlayer)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得

	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,

		g_aPlayerTexture_Path[0], //テクスチャのファイル名

		&g_pTexturePlayer);

	g_nCountterAnimPlayer = 0; //カウンターを初期化する

	g_nPatternAnimPlayer = 0; //パターンNo,を初期化する

	g_player.pos = posPlayer; //位置を初期化

	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //移動量を初期化

	g_player.ScreenPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //画面上の位置を初期化

	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //向きを初期化する（ｚ値を使用）

	g_player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	g_player.state = PLAYERSTATE_NORMAL; //状態

	g_player.nCounterState = 0; //状態カウンター

	g_player.nBulletCounter = 0; //球のカウンター

	g_player.nDashCounter = 0; //ダッシュのカウンター

	g_player.nLife = MAX_HP; //体力

	g_player.nAttack = 1; //攻撃力

	g_player.nRate = 1; //連射力

	g_player.fWidth = PLAYER_SIZE; //幅

	g_player.fHeight = PLAYER_SIZE * 2; //高さ

	g_player.bJump = false; //ジャンプしているかどうか

	g_player.bRightAim = true; //右向きかどうか

	//向きの初期化

	g_rot = false;

	//対角線の角度を算出する

	g_fAnglePlayer = atan2f(100, 100);

	//スティックを初期化

	g_stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, //確保するバッファのサイズ
								                       //sizeof(VERTEX_2D)*必要な頂点数

		D3DUSAGE_WRITEONLY,

		FVF_VERTEX_2D, //頂点フォーマット

		D3DPOOL_MANAGED,

		&g_pVtxBuffPlayer,

		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得

	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定

	pVtx[0].pos.x = g_player.pos.x - PLAYER_SIZE;

	pVtx[0].pos.y = g_player.pos.y - PLAYER_SIZE * 2;

	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.pos.x + PLAYER_SIZE;

	pVtx[1].pos.y = g_player.pos.y - PLAYER_SIZE * 2;

	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.pos.x - PLAYER_SIZE;

	pVtx[2].pos.y = g_player.pos.y;

	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.pos.x + PLAYER_SIZE;

	pVtx[3].pos.y = g_player.pos.y;

	pVtx[3].pos.z = 0.0f;

	//rhwの設定 1.0f固定

	pVtx[0].rhw = 1.0f;

	pVtx[1].rhw = 1.0f;

	pVtx[2].rhw = 1.0f;

	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定

	pVtx[0].col = g_player.col;

	pVtx[1].col = g_player.col;

	pVtx[2].col = g_player.col;

	pVtx[3].col = g_player.col;

	//テクスチャ座標の設定

	pVtx[0].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_PLAYER) * (g_nPatternAnimPlayer % MAX_COLUMN_PLAYER), (1.0f / MAX_LINES_PLAYER) * (g_player.bRightAim));

	pVtx[1].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_PLAYER) * ((g_nPatternAnimPlayer % MAX_COLUMN_PLAYER) + 1.0f), (1.0f / MAX_LINES_PLAYER) * (g_player.bRightAim));

	pVtx[2].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_PLAYER) * (g_nPatternAnimPlayer % MAX_COLUMN_PLAYER), (1.0f / MAX_LINES_PLAYER) * (g_player.bRightAim) + (1.0f / MAX_LINES_PLAYER));

	pVtx[3].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_PLAYER) * ((g_nPatternAnimPlayer % MAX_COLUMN_PLAYER) + 1.0f), (1.0f / MAX_LINES_PLAYER) * (g_player.bRightAim) + (1.0f / MAX_LINES_PLAYER));

	//頂点バッファをアンロックする

	g_pVtxBuffPlayer->Unlock();
}

//==================================
// プレイヤーの終了処理
//==================================
void UninitPlayer(void)
{
	//テクスチャの破棄

	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();

		g_pTexturePlayer = NULL;
	}

	//頂点バッファの破棄

	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();

		g_pVtxBuffPlayer = NULL;
	}
}

//==================================
// プレイヤーの更新処理
//==================================
void UpdatePlayer(void)
{
	//スクリーン上のプレイヤーの位置を計算

	g_player.ScreenPos.x = g_player.pos.x - GetPosCamera()->x;

	g_player.ScreenPos.y = g_player.pos.y - GetPosCamera()->y;

	//移動

	MovePlayer();

	////回転

	//RotatePlayer();

	//状態

	StatePlayer();

	//標準との角度を求める

	D3DXVECTOR3 posAim = GetPosAim();

	float AngleAim = CalcAngle(posAim);

	//球の発射処理

	ShootBallet(AngleAim);

	//アイテムの当たり判定

	CollisionItem();

	//アニメーション

	AnimPlayer();

	//SetPlayerEffect(g_player.pos, g_player.EffectCol,g_player.rot, PLAYER_SIZE + 10, 120);

	//頂点情報の更新

	UpdatepVtx();

	//プレイヤーのエフェクト

	if (1 < g_player.move.x || g_player.move.x < 0)
	{
		SetEffect(g_player.pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 20.0f, 30.0f);
	}
}
//=========================
// プレイヤーの移動処理
//=========================
void MovePlayer(void)
{
	//スティックの入力を獲得

	g_stick = GetLeftStick();

	if (g_stick.x != 0 || g_stick.y != 0) //入力されている場合
	{
		//移動量の更新

		g_player.move.x += g_stick.x * PLAYER_SPEED;
	}
	else
	{ //スティックが入力されていない場合

		if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true) //Aキーが押された場合
		{ //左移動
				//移動量を更新

			if (GetKeyboardPress(DIK_D) != true || GetJoypadPress(JOYKEY_RIGHT) != true) //Dキーが押されていない場合
			{
				g_player.move.x += sinf(-D3DX_PI * 0.5f) * PLAYER_SPEED; //左にすすむ
			}	
		}
		else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true) //Dキーが押された場合
		{ //右移動

			//移動量を更新

			g_player.move.x += sinf(D3DX_PI * 0.5f) * PLAYER_SPEED;
		}

	}

	//重力

	g_player.move.y += 2;

	//ジャンプ

	if (GetKeyboardTrigger(DIK_SPACE) && g_player.bJump == true || GetJoypadTrigger(JOYKEY_B) && g_player.bJump == true)
	{
		g_player.move.y = -30; //ジャンプの大きさ

		g_player.bJump = false;
	}
	
	//ダッシュ

	g_player.nDashCounter--;

	if (g_player.nDashCounter <= 0)
	{

		if (GetJoypadTrigger(JOYKEY_A) == true || GetMouseTrigger(MOUSEBOTTOM_RIGHT))
		{
			g_player.move.x *= 2.5f; //ジャンプの大きさ

			g_player.nDashCounter = 30;
		}
	}
}
//=========================
// プレイヤーの状態処理
//=========================
void StatePlayer(void)
{
	switch (g_player.state) //プレイヤーの状態
	{
	case PLAYERSTATE_NORMAL: //通常

		break;

	case PLAYERSTATE_DAMAGE: //ダメージ

		g_player.nCounterState--; //カウンターステートを減らす

		if (g_player.nCounterState <= 0) //カウンターステートが0になった
		{
			g_player.state = PLAYERSTATE_NORMAL; //プレイヤーの状態を通常にする

			//頂点カラーの設定

			g_player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
		}

		break;
	}
}
//=========================
// 球の発射処理
//=========================
void ShootBallet(float AngleAim)
{
	g_player.nBulletCounter--; //球のカウンターを減らす

	if (g_player.nBulletCounter <= 0) //カウンターがゼロ以下の場合
	{
		if (GetMousePress(MOUSEBOTTOM_LEFT) == true || GetRT() == true) //球の発射
		{ //スペースキーが押された

			//球の設定

			SetBullet(D3DXVECTOR3(g_player.pos.x + cosf(AngleAim) * (PLAYER_SIZE / 2),

				g_player.pos.y - PLAYER_SIZE + sinf(AngleAim) * (PLAYER_SIZE / 2),

				0.0f)

				, D3DXVECTOR3(cosf(AngleAim) * 40.0f, sinf(AngleAim) * 40.0f, 0.0f)
				
				,180

				, g_player.nAttack

				, AngleAim

				, BULLETTYPE_PLAYER);

			//バレットのカウンターを増やす

			g_player.nBulletCounter = 30;

			PlaySound(SOUND_LABEL_SE_SHOT);
		}
	}
}

//===================
//標準角度計算
//===================
float CalcAngle(D3DXVECTOR3 pos)
{
	float dx = pos.x - g_player.ScreenPos.x; //x座標の差

	float dy = pos.y - (g_player.ScreenPos.y - PLAYER_SIZE); //y座標の差

	float fangle = (float)atan2(dy, dx); //角度

	if (dx < 0)
	{ //プレイヤーより標準が左にある場合

		g_player.bRightAim = false; //プレイヤーの向きを左にする
	}
	else
	{ //プレイヤーより標準が右にある場合
		
		g_player.bRightAim = true; //プレイヤーの向きを右向きにする
	}

	return fangle; //角度を返す
}

//==================================
// プレイヤーの頂点情報の更新処理
//==================================
void AnimPlayer(void)
{
	//カウンターをプラス

	g_nCountterAnimPlayer++;

	if (g_nCountterAnimPlayer == ANIM_INTERVAL_PLAYER)
	{ //カウンターがアニメーション間隔分進んだ場合

		//カウンターを初期化

		g_nCountterAnimPlayer = 0;

		//アニメーションのパターンをプラス

		g_nPatternAnimPlayer++;

		//アニメーションのパターンがリセットされるようにする

		g_nPatternAnimPlayer %= MAX_ANIM_PLAYER;
	}
}

//==================================
// プレイヤーの頂点情報の更新処理
//==================================
void UpdatepVtx()
{
	//前回の位置を保存

	g_player.posOld = g_player.pos;
	
	//位置を更新

	g_player.pos.x += g_player.move.x;

	g_player.pos.y += g_player.move.y;

	g_player.ScreenPos.x += g_player.move.x;

	g_player.ScreenPos.y += g_player.move.y;

	//ブロックとの当たり判定

	g_player.bJump = CollisionBlock(&g_player.pos, &g_player.posOld, &g_player.move,
		g_player.fWidth, g_player.fHeight); //着地時だけジャンプが可能

	if (g_player.bJump == false)
	{
		g_player.bJump = CollisionScaffold(&g_player.pos, &g_player.posOld, &g_player.move,
			g_player.fWidth, g_player.fHeight, true); //着地時だけジャンプが可能
	}

	CollisionLaser(&g_player.pos, &g_player.posOld, &g_player.move,
		g_player.fWidth, g_player.fHeight); //着地時だけジャンプが可能


	//移動量の更新

	g_player.move.x += (0.0f - g_player.move.x) * 0.1f;

	//頂点バッファをロックし、頂点情報へのポインタを取得

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の更新

	pVtx[0].pos.x = g_player.ScreenPos.x - g_player.fWidth;

	pVtx[0].pos.y = g_player.ScreenPos.y - g_player.fHeight;

	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.ScreenPos.x + g_player.fWidth;

	pVtx[1].pos.y = g_player.ScreenPos.y - g_player.fHeight;

	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.ScreenPos.x - g_player.fWidth;

	pVtx[2].pos.y = g_player.ScreenPos.y;

	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.ScreenPos.x + g_player.fWidth;

	pVtx[3].pos.y = g_player.ScreenPos.y;

	pVtx[3].pos.z = 0.0f;

	//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定

	pVtx[0].col = g_player.col;

	pVtx[1].col = g_player.col;

	pVtx[2].col = g_player.col;

	pVtx[3].col = g_player.col;

	g_pVtxBuffPlayer->Unlock();

	//テクスチャ座標の更新

	pVtx[0].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_PLAYER) * (g_nPatternAnimPlayer % MAX_COLUMN_PLAYER), (1.0f / MAX_LINES_PLAYER) * (g_player.bRightAim));

	pVtx[1].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_PLAYER) * ((g_nPatternAnimPlayer % MAX_COLUMN_PLAYER) + 1.0f), (1.0f / MAX_LINES_PLAYER) * (g_player.bRightAim));

	pVtx[2].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_PLAYER) * (g_nPatternAnimPlayer % MAX_COLUMN_PLAYER), (1.0f / MAX_LINES_PLAYER) * (g_player.bRightAim) + (1.0f / MAX_LINES_PLAYER));

	pVtx[3].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_PLAYER) * ((g_nPatternAnimPlayer % MAX_COLUMN_PLAYER) + 1.0f), (1.0f / MAX_LINES_PLAYER) * (g_player.bRightAim) + (1.0f / MAX_LINES_PLAYER));

	//頂点バッファをアンロックする

	g_pVtxBuffPlayer->Unlock();
}

//==================================
// プレイヤーの描画処理
//==================================
void DrawPlayer(void)
{

	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得

	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定

	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));
	                          //頂点バッファのポインタ  頂点情報構造体のサイズ
	//頂点フォーマットの設定

	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	
	pDevice->SetTexture(0, g_pTexturePlayer);

	//ポリゴンの描画
	                                          //描画する最初の頂点インデックス
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
                                                 //描画するプリミティブ数
} 
//
//===================
// プレイヤーの取得処理
//===================
Player* GetPlayer(void)
{
	return &g_player; //敵の情報を返す
}
//
//===================
// プレイヤーヒット処理
//===================
void HitPlayer(HIT hit, int amount)
{
	switch (hit)
	{
	case HIT_BULLET: //球の場合

		break;

	case HIT_ENEMY: //敵の場合

		if (g_player.nCounterState == 0)
		{
			////ダメージスクリーン

			//SetDamageScreen();

			//プレイヤーのライフを減らす

			g_player.nLife--;

			//HPバーを減らす

		/*	ModifyHpBer(g_player.nLife);*/

			if (g_player.nLife <= 0) //プレイヤーの体力がなくなった
			{
				if (GetFade() == FADE_NONE)
				{

					//リザルト画面

					SetFade(MODE_RESULT);
				}
			}
			else
			{
				//カウンターを設定

				g_player.nCounterState = 75;

				g_player.state = PLAYERSTATE_DAMAGE;

				g_player.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			}
		}

		break;

	}

}

void CollisionItem(void)
{
	//アイテムの情報取得

	Item* pItem = GetItem();

	//アイテムの幅と高さの代入

	float fItemWidth = ITEM_SIZE; //幅

	float fItemHeight = ITEM_SIZE * 2; //高さ

	for (int nCount = 0; nCount < MAX_ITEM; nCount++, pItem++)
	{
		if (pItem->bUse == true)
		{
			//縦の当たり判定

			if (pItem->pos.x - fItemWidth < g_player.pos.x + g_player.fWidth

				&& g_player.pos.x - g_player.fWidth < pItem->pos.x + fItemWidth)
			{ //横の判定
				if (g_player.pos.y - g_player.fHeight < pItem->pos.y + fItemHeight

					&& pItem->pos.y - fItemHeight < g_player.pos.y) 
				{ //縦の判定

					HitItem(pItem);
					
				}
			}
		}
	}
}