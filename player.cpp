//=============================
// 
//アクションゲームmaster版
// Author森田煌也
// 
//=============================
#include "player.h"

#include "enemy.h"
#include "camera.h"
#include "effect.h"
#include "fade.h"
#include "input.h"
#include "item.h"
#include "bullet.h"
#include "aim.h"
#include "block.h"
#include "laser.h"
#include "lockonaim.h"
#include "playereffect.h"
#include "scaffold.h"
#include "screeneffect.h"
#include "sound.h"

//#include "hpber.h"

//マクロ定義

#define PLAYER_SPEED (1) //プレイヤーの速度

#define SHOOT_RECOIL (3) //球の反動

#define MAX_PLAYERTEXTURE (1) //プレイヤーのテクスチャ数

#define FOOTSTEPS_INTERVAL (40) //足音の間隔

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

	g_player.fWidth = PLAYER_SIZE; //幅

	g_player.fHeight = PLAYER_SIZE * 2; //高さ

	g_player.bLockon = false; //ロックオンしていない状態にする

	g_player.nLockonEnemy = 0; //ロックオンにしている敵

	g_player.bJump = false; //ジャンプしているかどうか

	g_player.nFootstepsCounter = FOOTSTEPS_INTERVAL;

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
	Enemy* pEnemy = GetEnemy(); //敵の情報の取得

	//ロックオンエイムに情報代入

	LockonAim* pLockonAim = GetLockonAim();

	//ロックオンの処理

	if (GetKeyboardTrigger(DIK_F) == true || GetJoypadTrigger(JOYKEY_X) == true) //Aキーが押された場合
	{ //左移動

		Lockon(); //ロックオン
	}

	if (g_player.bLockon == true)
	{ //敵が使用されている場合

		pEnemy += g_player.nLockonEnemy; //敵のポインタをロックオンしている敵に合わせる
	}

	//移動

	MovePlayer();

	////回転

	//RotatePlayer();

	if (GetMode() == MODE_GAME)
	{
		//状態

		StatePlayer();

		//球の発射

		float AngleAim;

		if (g_player.bLockon == true)
		{ //ロックオン状態の場合

			AngleAim = CalcAngle(D3DXVECTOR3(pEnemy->ScreenPos.x, pEnemy->ScreenPos.y - ENEMY_SIZE, 0.0f)); //ロックオンしている敵の角度計算

			if (pEnemy->bUse == false)
			{ //敵が使用されていない場合

				g_player.bLockon = false; //ロックオン状態を解除する

				//ロックオンエイムを使用していない状態にする

				pLockonAim->bUse = false;
			}
		}
		else
		{ //ロックオンしていない場合

			//標準との角度を求める

			D3DXVECTOR3 posAim = GetPosAim(); //標準の位置を獲得

			AngleAim = CalcAngle(posAim); //標準との角度計算

		}
		//球の発射処理

		ShootBullet(AngleAim);

		//アイテムの当たり判定

		CollisionItem();
	}

	//アニメーション

	AnimPlayer();

	//SetPlayerEffect(g_player.pos, g_player.EffectCol,g_player.rot, PLAYER_SIZE + 10, 120);

	//足音

	
	if (g_player.bJump == true)
	{
		FootstepsPlayer();
	}

	//頂点情報の更新

	UpdatepVtx();

	if (GetMode() == MODE_GAME)
	{ //ゲームモードの場合

		//プレイヤーのエフェクト

		if (g_player.bJump == true)
		{
			if (1 < g_player.move.x)
			{ //右向きの移動エフェクト

				SetEffect(D3DXVECTOR3(g_player.pos.x + PLAYER_SIZE, g_player.pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20.0f, 30.0f, true);

			}
			else if (g_player.move.x < -1)
			{ //左向きの移動エフェクト

				SetEffect(D3DXVECTOR3(g_player.pos.x - PLAYER_SIZE, g_player.pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20.0f, 30.0f, true);

			}
		}

		//プレイヤーのエフェクト

		SetPlayerEffect(g_player.pos, PLAYER_SIZE + 20.0f, 15);

		if (pEnemy->bUse == true && g_player.bLockon == true)
		{ //敵が使用されている場合

			//ロックオンエイムの位置を合わせる

			pLockonAim->pos = pEnemy->ScreenPos;

			pLockonAim->pos.y -= ENEMY_SIZE;

			//ロックオンエイムを使用している状態にする

			pLockonAim->bUse = true;
		}
		else
		{
			pLockonAim->bUse = false;
		}
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

	//ステージ選択画面の向き切り替え

	if (GetMode() == MODE_SELECTSTAGE)
	{ //ステージ選択画面の場合

		if (0 < g_player.move.x)
		{ //右向きの場合

			g_player.bRightAim = true;
		}
		else if (g_player.move.x < 0)
		{ //左向きの場合

			g_player.bRightAim = false;
		}
	}
	else if (GetMode() == MODE_GAME)
	{
		//ダッシュ

		g_player.nDashCounter--;

		if (g_player.nDashCounter <= 0)
		{ //ダッシュカウンターがゼロの場合

			if (GetJoypadTrigger(JOYKEY_A) == true || GetMouseTrigger(MOUSEBOTTOM_RIGHT))
			{ //ダッシュボタンが押された場合

				PlaySound(SOUND_LABEL_SE_DASH);

				if (1 < g_player.move.x)
				{ //右向きの移動量の増加
					g_player.move.x = 32; //移動量の増加
				}
				else if (g_player.move.x < -1)
				{ //左向きの移動量の増加
					g_player.move.x = -32; //移動量の増加
				}
				else //移動量がゼロの場合
				{
					if (g_player.bRightAim == true)
					{ //右向きの移動量の増加

						g_player.move.x = 32; //移動量の増加
					}
					else
					{//左向きの移動量の増加

						g_player.move.x = -32; //移動量の増加
					}
				}

				//ダッシュカウンターを設定

				g_player.nDashCounter = 60;

				//ダッシュ状態にする

				g_player.bDash = true;
			}
		}
		else if (g_player.nDashCounter <= 10)
		{ //ダッシュ状態ではなくする
			g_player.bDash = false;
		}

		//ジャンプ

		if (GetKeyboardTrigger(DIK_SPACE) && g_player.bJump == true || GetJoypadTrigger(JOYKEY_B) && g_player.bJump == true)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);

			g_player.move.y = -30; //ジャンプの大きさ

			g_player.move.x *= 0.7f; //ジャンプ中の減速処理

			g_player.bJump = false;
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

		if (g_player.col != D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))
		{
			g_player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		break;

	case PLAYERSTATE_DAMAGE: //ダメージ

		g_player.nCounterState--; //カウンターステートを減らす

		if (g_player.nCounterState <= 0) //カウンターステートが0になった
		{
			g_player.state = PLAYERSTATE_NORMAL; //プレイヤーの状態を通常にする
		}
		else if (7 < g_player.nCounterState % 15)
		{
			//頂点カラーの設定

			g_player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			//頂点カラーの設定

			g_player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		}

		break;
	}
}
//=========================
// 球の発射処理
//=========================
void ShootBullet(float AngleAim)
{
	g_player.nBulletCounter--; //球のカウンターを減らす

	if (g_player.nBulletCounter <= 0) //カウンターがゼロ以下の場合
	{
		if (GetMouseTrigger(MOUSEBOTTOM_LEFT) == true || GetRTTrigger() == true) //球の発射
		{ //スペースキーが押された

			//球の設定

			SetBullet(D3DXVECTOR3(g_player.pos.x + cosf(AngleAim) * (PLAYER_SIZE / 2),

				g_player.pos.y - PLAYER_SIZE + sinf(AngleAim) * (PLAYER_SIZE / 2),

				0.0f)

				, D3DXVECTOR3(cosf(AngleAim) * 40.0f, sinf(AngleAim) * 40.0f, 0.0f)
				
				,180

				, 1

				, AngleAim

				, 20.0f

				, BULLETOWNER_PLAYER
		
				, BULLETTYPE_PLAYER
				);

			//バレットのカウンターを増やす

			g_player.nBulletCounter = 20;

			PlaySound(SOUND_LABEL_SE_SHOT);
		}
	}
}

//===================
//角度計算
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
// プレイヤーのアニメーション処理
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
// プレイヤーの足音処理
//==================================
void FootstepsPlayer(void)
{
	if (g_player.move.x < -1 || 1 < g_player.move.x)
	{ //横移動している場合

		g_player.nFootstepsCounter++; //カウンターをプラス

		if (FOOTSTEPS_INTERVAL <= g_player.nFootstepsCounter)
		{ //カウンターが間隔分進んだら

			//足音を鳴らす

			PlaySound(SOUND_LABEL_SE_FOOTSTEPS);

			//リセットされるようにする

			g_player.nFootstepsCounter %= FOOTSTEPS_INTERVAL;
		}
	}
}

//==================================
// プレイヤーの頂点情報の更新処理
//==================================
void UpdatepVtx()
{
	//重力

	g_player.move.y += 2;

	//前回の位置を保存

	g_player.posOld = g_player.pos;
	
	//位置を更新

	g_player.pos.x += g_player.move.x;

	g_player.pos.y += g_player.move.y;

	//スクリーン上のプレイヤーの位置を計算

	g_player.ScreenPos.x = g_player.pos.x - GetPosCamera()->x;

	g_player.ScreenPos.y = g_player.pos.y - GetPosCamera()->y;

	//ブロックとの当たり判定

	g_player.bJump = CollisionBlock(&g_player.pos, &g_player.posOld, &g_player.move,
		g_player.fWidth, g_player.fHeight); //着地時だけジャンプが可能

	//足場の当たり判定

	if (g_player.bJump == false)
	{
		g_player.bJump = CollisionScaffold(&g_player.pos, &g_player.posOld, &g_player.move,
			g_player.fWidth, g_player.fHeight, true); //着地時だけジャンプが可能
	}

	//レーザーの当たり判定

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

		//ダッシュしているか

		if (g_player.bDash == true)
		{
			//ダッシュスクリーン

			if (UseScreenEffect() == false)
			{ //スクリーンエフェクトを使用していない場合

				//回避音

				PlaySound(SOUND_LABEL_SE_AVOIDANCE);

				//スクリーンエフェクト

				SetScreenEffect(D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f));
			}
		}
		else
		{

			if (g_player.nCounterState == 0)
			{
				//ダメージ音

				PlaySound(SOUND_LABEL_SE_DAMAGE);

				//ダメージスクリーン

				if (UseScreenEffect() == false)
				{ //スクリーンエフェクトを使用していない場合

					SetScreenEffect(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
				}

				//プレイヤーのライフを減らす

				g_player.nLife--;

				//HPバーを減らす

			/*	ModifyHpBer(g_player.nLife);*/

				if (g_player.nLife <= 0) //プレイヤーの体力がなくなった
				{
					//フェードしていない場合

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

				}
			}

			break;

		}
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