//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#include "enemy.h"

#include "block.h"
#include "camera.h"
#include "chargeeffect.h"
#include "combo.h"
#include "enemyhp.h"
#include "enemyweapon.h"
#include "explosion.h"
#include "item.h"
#include "player.h"
#include "scaffold.h"
#include "sound.h"
#include "stoptime.h"

#include <stdio.h>

//マクロ定義

#define MAX_CHARGECOUNTER (10 * 60) //チャージの時間

#define CHARGEEFFECT_INTERVAL (MAX_CHARGECOUNTER / 40) //チャージエフェクトの生成間隔

//アニメーション

#define MAX_ANIM_ENEMY (6) //アニメーションの最大数

#define MAX_COLUMN_ENEMY (6) //列数 

#define MAX_LINES_ENEMY (2) //行数

#define ANIM_INTERVAL_ENEMY (12) //アニメーションの間隔

//グローバル変数

LPDIRECT3DTEXTURE9 g_apTextureEnemy[ENEMYTYPE_MAX] = {};

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy;

EnemyInfo g_aEnemyInfo[ENEMYTYPE_MAX];

Enemy g_aEnemy[MAX_ENEMY]; //敵の情報

int g_nNumEnemy; //敵の数を代入する変数,

const char* g_aEnemyTexture_Path[ENEMYTYPE_MAX] =
{
	"Data/TEXTURE/ENEMY/enemy000.png",

	"Data/TEXTURE/ENEMY/enemy000.png",

	"Data/TEXTURE/ENEMY/enemy001.png",
}; //テクスチャの相対パスのパス

//===================
// 敵の初期化処理
//===================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	int nCntEnemy;

	//デバイスの取得

	pDevice = GetDevice();

	//テクスチャ（４枚分）の読み込み

	for (nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{
		D3DXCreateTextureFromFile(pDevice,

		g_aEnemyTexture_Path[nCntEnemy], //テクスチャのファイル名

		&g_apTextureEnemy[nCntEnemy]);
	}

	//敵の情報のロード処理

	LoadEnemy();

	//敵の情報の初期化

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //位置

		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //移動

		g_aEnemy[nCntEnemy].ScreenPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //画面上の位置を初期化

		g_aEnemy[nCntEnemy].EnemyInfo = g_aEnemyInfo[ENEMYTYPE_1]; //種類

		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL; //状態

		g_aEnemy[nCntEnemy].nCounterState = 0; //状態カウント

		g_aEnemy[nCntEnemy].nChargeCounter = 0; //チャージカウンター

		g_aEnemy[nCntEnemy].nCounterAction = 0;

		g_aEnemy[nCntEnemy].bJump = true; //ジャンプできる状態にしておく

		g_aEnemy[nCntEnemy].bRightMove = false; //左向きにしておく

		g_aEnemy[nCntEnemy].bUse = false; //使用していない状態にする
	}

	//敵の総数を設定

	g_nNumEnemy = 0;

	//頂点バッファの生成

	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY, //確保するバッファのサイズ（球の数分掛ける）

		D3DUSAGE_WRITEONLY,

		FVF_VERTEX_2D, //頂点フォーマット

		D3DPOOL_MANAGED,

		&g_pVtxBuffEnemy,

		NULL
	);

	//頂点情報の設定

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pVtx += 4) //頂点データのポインタを4つ分進める
	{
		//頂点座標の設定

		pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE;

		pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE * 2);

		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE;

		pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE * 2);

		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE;

		pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y;

		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE;

		pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y;

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

		//テクスチャ座標の設定

		pVtx[0].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_ENEMY) * (g_aEnemy[nCntEnemy].nPatternAnimEnemy % MAX_COLUMN_ENEMY), (1.0f / MAX_LINES_ENEMY) * (g_aEnemy[nCntEnemy].bRightMove));

		pVtx[1].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_ENEMY) * ((g_aEnemy[nCntEnemy].nPatternAnimEnemy % MAX_COLUMN_ENEMY) + 1.0f), (1.0f / MAX_LINES_ENEMY) * (g_aEnemy[nCntEnemy].bRightMove));

		pVtx[2].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_ENEMY) * (g_aEnemy[nCntEnemy].nPatternAnimEnemy % MAX_COLUMN_ENEMY), (1.0f / MAX_LINES_ENEMY) * (g_aEnemy[nCntEnemy].bRightMove) + (1.0f / MAX_LINES_ENEMY));

		pVtx[3].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_ENEMY) * ((g_aEnemy[nCntEnemy].nPatternAnimEnemy % MAX_COLUMN_ENEMY) + 1.0f), (1.0f / MAX_LINES_ENEMY) * (g_aEnemy[nCntEnemy].bRightMove) + (1.0f / MAX_LINES_ENEMY));
	}

	g_pVtxBuffEnemy->Unlock();

	//各オブジェクトの初期化処理

	InitEnemyHp(); //hp

	InitEnemyWeapon(); //武器

	InitChargeEffect(); //チャージエフェクト
}

//===================
// 敵のロード処理
//===================
void LoadEnemy(void)
{
	FILE* pFile; //ファイルポインタ

	int nCount,

		nType = 0; //読み込んだ値を代入する変数

	D3DXVECTOR3 posPlayer; //プレイヤーの位置

	pFile = fopen("Data/SAVEDATA/ENEMY/enemy.csv", "r");

	if (pFile != NULL)
	{ //ファイルが開いた場合
		
		for (nCount = 0; nCount < ENEMYTYPE_MAX; nCount++)
		{ //敵のタイプのの最大数分繰り返す

			//タイプを読み込む

			(void)fscanf(pFile, "%d%*[,]", &nType);

			//各項目に代入する

			g_aEnemyInfo[nType].type = (ENEMYTYPE)nType; //タイプ

			(void)fscanf(pFile, "%d%*[,]", &g_aEnemyInfo[nType].nWeapon); //武器

			(void)fscanf(pFile, "%d%*[,]", &g_aEnemyInfo[nType].nLife); //ライフ
			
			(void)fscanf(pFile, "%f%*[,]", &g_aEnemyInfo[nType].fSpeed); //スピード
		}

		fclose(pFile); //ファイルを閉じる
	}
}

//===================
// 敵の終了処理
//===================
void UninitEnemy(void)
{
	int nCntEnemy;

	//テクスチャ（４枚分）の破棄

	for (nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();

			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}

	//頂点バッファの破棄

	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();

		g_pVtxBuffEnemy = NULL;
	}

	//各オブジェクトの終了処理

	UninitEnemyHp(); //hp

	UninitEnemyWeapon(); //武器

	UninitChargeEffect(); //チャージエフェクト
}

//===================
// 敵の更新処理
//===================
void UpdateEnemy(void)
{
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)//敵の数分
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{ //敵が使われている場合

			//スクリーン上の敵の位置を計算

			g_aEnemy[nCntEnemy].ScreenPos.x = g_aEnemy[nCntEnemy].pos.x - GetPosCamera()->x;

			g_aEnemy[nCntEnemy].ScreenPos.y = g_aEnemy[nCntEnemy].pos.y - GetPosCamera()->y;

			//カメラに写ったら

			if (0 < g_aEnemy[nCntEnemy].ScreenPos.x + ENEMY_SIZE && g_aEnemy[nCntEnemy].ScreenPos.x - ENEMY_SIZE < SCREEN_WIDTH

				&& 0 < g_aEnemy[nCntEnemy].ScreenPos.y + ENEMY_SIZE && g_aEnemy[nCntEnemy].ScreenPos.y- ENEMY_SIZE < SCREEN_HEIGHT)
			{

				if (GetStopTime()->bStopTime == false)
				{ //時間停止中じゃない場合

					StateEnemy(nCntEnemy); //状態

					//プレイヤーに近づいたら

					if (-SCREEN_WIDTH < g_aEnemy[nCntEnemy].ScreenPos.x && g_aEnemy[nCntEnemy].ScreenPos.x < SCREEN_WIDTH * 2

						&& -SCREEN_WIDTH < g_aEnemy[nCntEnemy].ScreenPos.y && g_aEnemy[nCntEnemy].ScreenPos.y < SCREEN_HEIGHT * 2)
					{
						Player* pPlayer = GetPlayer();

						float distanceX = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;

						float threshold = 80.0f; // 向きを変えない距離

						//離れているときだけ向きを更新する
						if (distanceX * distanceX > threshold * threshold)
						{

							if (pPlayer->pos.x < g_aEnemy[nCntEnemy].pos.x)
							{
								g_aEnemy[nCntEnemy].bRightMove = false;
							}
							else
							{
								g_aEnemy[nCntEnemy].bRightMove = true;
							}
						}

						//敵の行動

						if (0 < g_aEnemy[nCntEnemy].nChargeCounter)
						{ //チャージカウンターが0じゃない場合
							
							Charge(nCntEnemy); // ここでエフェクトを呼ぶ
						}
						else
						{
							//アクション

							ActionEnemy(nCntEnemy);

							AnimEnemy(nCntEnemy);
						}
					}
				}
			}
			//頂点情報の設定

			VERTEX_2D* pVtx;

			//頂点バッファをロックし、頂点データへのポインタを取得

			g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += nCntEnemy * 4;

			pVtx[0].pos.x = g_aEnemy[nCntEnemy].ScreenPos.x - ENEMY_SIZE;

			pVtx[0].pos.y = g_aEnemy[nCntEnemy].ScreenPos.y - (ENEMY_SIZE * 2);

			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].ScreenPos.x + ENEMY_SIZE;

			pVtx[1].pos.y = g_aEnemy[nCntEnemy].ScreenPos.y - (ENEMY_SIZE * 2);

			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].ScreenPos.x - ENEMY_SIZE;

			pVtx[2].pos.y = g_aEnemy[nCntEnemy].ScreenPos.y;

			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].ScreenPos.x + ENEMY_SIZE;

			pVtx[3].pos.y = g_aEnemy[nCntEnemy].ScreenPos.y;

			pVtx[3].pos.z = 0.0f;

			//テクスチャ座標の設定

			pVtx[0].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_ENEMY) * (g_aEnemy[nCntEnemy].nPatternAnimEnemy % MAX_COLUMN_ENEMY), (1.0f / MAX_LINES_ENEMY) * (g_aEnemy[nCntEnemy].bRightMove));

			pVtx[1].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_ENEMY) * ((g_aEnemy[nCntEnemy].nPatternAnimEnemy % MAX_COLUMN_ENEMY) + 1.0f), (1.0f / MAX_LINES_ENEMY) * (g_aEnemy[nCntEnemy].bRightMove));

			pVtx[2].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_ENEMY) * (g_aEnemy[nCntEnemy].nPatternAnimEnemy % MAX_COLUMN_ENEMY), (1.0f / MAX_LINES_ENEMY) * (g_aEnemy[nCntEnemy].bRightMove) + (1.0f / MAX_LINES_ENEMY));

			pVtx[3].tex = D3DXVECTOR2((1.0f / MAX_COLUMN_ENEMY) * ((g_aEnemy[nCntEnemy].nPatternAnimEnemy % MAX_COLUMN_ENEMY) + 1.0f), (1.0f / MAX_LINES_ENEMY) * (g_aEnemy[nCntEnemy].bRightMove) + (1.0f / MAX_LINES_ENEMY));


			g_pVtxBuffEnemy->Unlock();
		}

	}

	//各オブジェクトの更新処理

	UpdateEnemyHp(); //hp

	UpdateEnemyWeapon(); //武器

	UpdateChargeEffect(); //チャージエフェクト
}

//==================
// 敵の状態処理
//==================
void StateEnemy(int nCnt)
{
	//ポリゴンの描画

	VERTEX_2D* pVtx; //forの前でロック、後でアンロック

	//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_aEnemy[nCnt].state) //敵の状態
	{
	case ENEMYSTATE_NORMAL: //通常

		pVtx += 4 * nCnt; //pVtxを合わせる

		break;

	case ENEMYSTATE_DAMAGE: //ダメージ

		g_aEnemy[nCnt].nCounterState--; //カウンターステートを減らす

		if (g_aEnemy[nCnt].nCounterState <= 0) //カウンターステートが0になった
		{
			g_aEnemy[nCnt].state = ENEMYSTATE_NORMAL; //敵の状態を通常にする

			//頂点カラーの設定

			pVtx += 4 * nCnt; //pVtxを合わせる

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		break;
	}

	g_pVtxBuffEnemy->Unlock();
}
//==================
// 敵の行動処理
//==================
void ActionEnemy(int nCnt)
{
	switch (g_aEnemy[nCnt].EnemyInfo.type) //タイプ１と２
	{
	case ENEMYTYPE_1:

		ActionType1(nCnt); //パターン１

		break;

	case ENEMYTYPE_2:

		ActionType2(nCnt); //パターン２

		break;

	//case ENEMYTYPE_3:

	//	ActionType3(nCnt); //パターン３

	//	break;

	//case ENEMYTYPE_4:

	//	ActionType4(nCnt); //パターン4

	//	break;
	}

	//移動処理

	MoveEnemy(nCnt);

}
//==================
// 敵の行動パターン１
//==================
void ActionType1(int nCnt)
{
	g_aEnemy[nCnt].nCounterAction++; //行動カウンターをプラス

	Player* pPlayer = GetPlayer();

	if (0 <= g_aEnemy[nCnt].nCounterAction && g_aEnemy[nCnt].nCounterAction < 300)
	{ //一段階目のアクション

		//移動量の決定

		if (pPlayer->pos.x <= g_aEnemy[nCnt].pos.x)
		{ //プレイヤーが左にいるとき

			g_aEnemy[nCnt].move.x = -3;
		}
		else
		{ //プレイヤーが右にいるとき

			g_aEnemy[nCnt].move.x = 3;
		}
	}
	else if (300 == g_aEnemy[nCnt].nCounterAction) //カウンターが480を超えた場合
	{ //２段階目の動き

		if (pPlayer->pos.y <= g_aEnemy[nCnt].pos.y)
		{ //プレイヤーが上にいるとき

			g_aEnemy[nCnt].move.y = -30; //ジャンプ
		}
		else if (g_aEnemy[nCnt].pos.y <= pPlayer->pos.y)
		{ //プレイヤーが下にいるとき

			g_aEnemy[nCnt].pos.y++; //足場を降りれるように
		}

		g_aEnemy[nCnt].move.x = 0; //しばらく制止する
	}
	else if (480 <= g_aEnemy[nCnt].nCounterAction)
	{ //一定期間たった時

		//行動カウンターをリセット

		g_aEnemy[nCnt].nCounterAction = 0;
	}

	//かくかくしないようにする

	float distanceX = pPlayer->pos.x - g_aEnemy[nCnt].pos.x;

	float threshold = 80.0f; // 位置を変えない距離

	// プレイヤーと敵の距離が向きを変えない距離より離れている場合
	if (fabsf(distanceX) < threshold)
	{
		g_aEnemy[nCnt].move.x = 0;
	}
}

//==================
// 敵の行動パターン2
//==================
void ActionType2(int nCnt)
{
	g_aEnemy[nCnt].nCounterAction++; //行動カウンターをプラス

	Player* pPlayer = GetPlayer();

	if (0 <= g_aEnemy[nCnt].nCounterAction && g_aEnemy[nCnt].nCounterAction < 300)
	{ //一段目の動き

		//移動量の決定

		if (pPlayer->pos.x <= g_aEnemy[nCnt].pos.x)
		{ //プレイヤーが左にいるとき

			g_aEnemy[nCnt].move.x = -3;
		}
		else
		{ //プレイヤーが右にいるとき

			g_aEnemy[nCnt].move.x = 3;
		}
	}
	else if (300 == g_aEnemy[nCnt].nCounterAction) //カウンターが480を超えた場合
	{ //二段目の動き

		if (pPlayer->pos.y <= g_aEnemy[nCnt].pos.y)
		{ //プレイヤーが上にいるとき

			g_aEnemy[nCnt].move.y = -30; //ジャンプ
		}
		else if (g_aEnemy[nCnt].pos.y <= pPlayer->pos.y)
		{ //プレイヤーが下にいるとき

			g_aEnemy[nCnt].pos.y++; //足場を降りれるように
		}

		g_aEnemy[nCnt].move.x = 0; //しばらく制止する

		
	}
	else if (360 == g_aEnemy[nCnt].nCounterAction)
	{ //3段目の動き

		//攻撃(後でパターン分けする)

		//チャージのカウンターを設定

		g_aEnemy[nCnt].nChargeCounter = MAX_CHARGECOUNTER;

		//カウンターをリセット

		g_aEnemy[nCnt].nCounterAction = 0;
	}

	//かくかくしないようにする

	float distanceX = pPlayer->pos.x - g_aEnemy[nCnt].pos.x;

	float threshold = 80.0f; // 位置を変えない距離

	// プレイヤーと敵の距離が向きを変えない距離より離れている場合
	if (fabsf(distanceX) < threshold)
	{
		g_aEnemy[nCnt].move.x = 0;
	}

}

//==========================
// 敵の移動処理
//==========================
void MoveEnemy(int nCnt)
{
	//重力

	g_aEnemy[nCnt].move.y += 2;

	//前フレームの位置を保存

	g_aEnemy[nCnt].posOld = g_aEnemy[nCnt].pos;

	//位置を更新

	g_aEnemy[nCnt].pos.x += g_aEnemy[nCnt].move.x;

	g_aEnemy[nCnt].pos.y += g_aEnemy[nCnt].move.y;

	//画面上の位置を更新

	g_aEnemy[nCnt].ScreenPos.x += g_aEnemy[nCnt].move.x;

	g_aEnemy[nCnt].ScreenPos.y += g_aEnemy[nCnt].move.y;


	g_aEnemy[nCnt].bJump = CollisionBlock(&g_aEnemy[nCnt].pos, &g_aEnemy[nCnt].posOld, &g_aEnemy[nCnt].move,
		PLAYER_SIZE, PLAYER_SIZE * 2); //着地時だけジャンプが可能

	//足場の判定

	if (g_aEnemy[nCnt].bJump == false)
	{
		CollisionScaffold(&g_aEnemy[nCnt].pos, &g_aEnemy[nCnt].posOld, &g_aEnemy[nCnt].move,
			PLAYER_SIZE, PLAYER_SIZE * 2, false);
	}

	//下に落ちたら破棄

	if (SCREEN_HEIGHT < g_aEnemy[nCnt].pos.y + ENEMY_SIZE) //一番下にあたった場合
	{
		g_aEnemy[nCnt].bUse = false;
	}
}

//==================================
// プレイヤーの頂点情報の更新処理
//==================================
void AnimEnemy(int nCnt)
{
	//カウンターをプラス

	g_aEnemy[nCnt].nCountterAnimEnemy++;

	if (g_aEnemy[nCnt].nCountterAnimEnemy == ANIM_INTERVAL_ENEMY)
	{ //カウンターがアニメーション間隔分進んだ場合

		//カウンターを初期化

		g_aEnemy[nCnt].nCountterAnimEnemy = 0;

		//アニメーションのパターンをプラス

		g_aEnemy[nCnt].nPatternAnimEnemy++;

		//アニメーションのパターンがリセットされるようにする

		g_aEnemy[nCnt].nPatternAnimEnemy %= MAX_ANIM_ENEMY;
	}
}

//===================
// 敵のチャージの処理
//===================
void Charge(int nCnt)
{
	//チャージカウントを減らす

	g_aEnemy[nCnt].nChargeCounter--;

	//ランダムで３２方向からチャージエフェクトを出す(D3DX_PI / 32 * rand)

	if (g_aEnemy[nCnt].nChargeCounter % CHARGEEFFECT_INTERVAL == 0)
	{
		//位置を敵の中心にそろえる

		D3DXVECTOR3 pos;

		pos = D3DXVECTOR3(g_aEnemy[nCnt].pos.x, g_aEnemy[nCnt].pos.y - ENEMY_SIZE, g_aEnemy[nCnt].pos.z);

		//チャージエフェクトの設定

		SetChargeEffect(pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 25.0f, ENEMY_SIZE * 2);
	}
}

//===================
// 敵のチャージ攻撃の処理
//===================
void ChargeAttack(int nCnt)
{

}

//===================
// 敵の描画処理
//===================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	int nCntEnemy;

	//デバイスの取得

	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定

	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定

	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{ //敵が使用されている

		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//テクスチャの設定(種類に合わせて設定)

			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].EnemyInfo.type]);

			//ポリゴンの描画

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);

		}
	}

	//各オブジェクトの描画処理

	DrawEnemyHp(); //hp

	DrawEnemyWeapon(); //武器
}

//===================
// 敵の設定処理
//===================
void SetEnemy(D3DXVECTOR3 pos, int type)
{
	int nCntEnemy;

	//ポリゴンの描画

	VERTEX_2D* pVtx; //forの前でロック、後でアンロック

	//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pVtx += 4)
	{

		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			//敵の情報の設定(Enemy構造体)

			g_aEnemy[nCntEnemy].pos = pos; //位置

			g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //移動量

			g_aEnemy[nCntEnemy].nCounterAction = 0; //アニメーション

			g_aEnemy[nCntEnemy].EnemyInfo = g_aEnemyInfo[type];

			//敵を使用している状態にする

			g_aEnemy[nCntEnemy].bUse = true;

			//頂点座標の設定

			pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE;

			pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE * 2);

			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE;

			pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE * 2);

			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE;

			pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y;

			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE;

			pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y;

			pVtx[3].pos.z = 0.0f;

			//頂点カラー

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_nNumEnemy++;

			//各項目の設定

			SetEnemyHp(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].EnemyInfo.nLife); //hp

			SetEnemyWeapon(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].EnemyInfo.nWeapon);

			break;
		}
	}

	g_pVtxBuffEnemy->Unlock();
}

//===================
// 敵のヒット処理
//===================
void HitEnemy(int nCntEnemy, int nDamage)
{
	//ヒット音

	PlaySound(SOUND_LABEL_SE_HIT000);

	//コンボ数を加算

	AddCombo();

	//ライフを減らす
	
	g_aEnemy[nCntEnemy].EnemyInfo.nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].EnemyInfo.nLife <= 0) //敵の体力がなくなった
	{
		//爆発

		SetExplosion(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		g_aEnemy[nCntEnemy].bUse = false; //敵を使用していない状態にする

		g_nNumEnemy--; //敵の総数を減らす
	}
 	else
	{
		//ダメージ

		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE; //ダメージを与える

		g_aEnemy[nCntEnemy].nCounterState = 5; //状態カウンターを設定

		VERTEX_2D* pVtx; //forの前でロック、後でアンロック

	//頂点バッファをロックし、頂点データへのポインタを取得

		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定

		pVtx += nCntEnemy * 4; //頂点データへのポインタを敵に合わせる

		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		g_pVtxBuffEnemy->Unlock();
	}
}
//==================
// プレイヤーとの当たり判定
//==================
void CollisionPlayer(Enemy* pEnemy)
{
	//プレイヤーの取得

	//Player* pPlayer = GetPlayer(); //敵の情報が代入される

	//中心座標の距離（ｘ、ｙ）

	//float fDistance_x = pPlayer->pos.x - pEnemy->pos.x, //距離ｘ

	//	fDistance_y = pPlayer->pos.y - pEnemy->pos.y; //距離y

	//二つの中心中心座標の距離の2乗

	//float fDistanceSquared = (fDistance_x * fDistance_x) + (fDistance_y * fDistance_y);

	////２つの円の半径の合計

	//float fTotalRadius = (ENEMY_SIZE + PLAYER_SIZE); //半径の合計の長さ

	//if (fDistanceSquared <= fTotalRadius * fTotalRadius) //半径の合計の２乗より距離の２乗が小さい場合
	//{//敵とプレイヤーが当たった

	//	HitPlayer(HIT_ENEMY, pEnemy->nAttack); //敵のヒット処理
	//}
}

//===================
// 敵の取得処理
//===================
Enemy* GetEnemy(void)
{
	return &g_aEnemy[0]; //敵の情報を返す
}
//===================
// プレイヤーとの角度計算
//===================
//float CalcAngle(D3DXVECTOR3 ppos)
//{
//	//Player* pPlayer = GetPlayer(); //プレイヤーの情報
//
//	//float dx = pPlayer->pos.x - ppos.x; //x座標の差
//
//	//float dy = pPlayer->pos.y - ppos.y; //y座標の差
//
//	//float angle = (float)atan2(dy, dx); //角度
//
//	//return angle; //角度を返す
//}

//===================
// 敵の総数取得処理
//===================
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}
