//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#include "enemy.h"

#include "block.h"
#include "camera.h"
#include "item.h"
#include "player.h"
#include "scaffold.h"
#include "sound.h"
#include "stoptime.h"

//グローバル変数

LPDIRECT3DTEXTURE9 g_apTextureEnemy[ENEMYTYPE_MAX] = {};

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy;

Enemy g_aEnemy[MAX_ENEMY]; //敵の情報

int g_nNumEnemy; //敵の数を代入する変数

const char* g_aEnemyTexture_Path[ENEMYTYPE_MAX] =
{
	"Data/TEXTURE/ENEMY/enemy000.png",

	"Data/TEXTURE/ENEMY/enemy001.png",

	"Data/TEXTURE/ENEMY/enemy002.png",

	"Data/TEXTURE/ENEMY/enemy003.png",

	"Data/TEXTURE/ENEMY/enemy004.png",
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

	//敵の情報の初期化

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //位置

		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //移動

		g_aEnemy[nCntEnemy].ScreenPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //画面上の位置を初期化

		g_aEnemy[nCntEnemy].type = ENEMYTYPE_0; //種類

		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL; //状態

		g_aEnemy[nCntEnemy].nCounterState = 0; //状態カウント

		g_aEnemy[nCntEnemy].nCounterAction = 0;

		g_aEnemy[nCntEnemy].nLife = 100; //体力

		g_aEnemy[nCntEnemy].fSpeed = 0.0f; //スピード

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

		//テクスチャ座標

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);

		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	g_pVtxBuffEnemy->Unlock();
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

			if (GetStopTime()->bStopTime == false)
			{ //時間停止中じゃない場合

				StateEnemy(nCntEnemy); //状態

				if (g_aEnemy[nCntEnemy].type != ENEMYTYPE_4)
				{

					CollisionPlayer(&g_aEnemy[nCntEnemy]); //当たり判定
				}

				ActionEnemy(nCntEnemy); //行動
			}

			//頂点情報の設定

			VERTEX_2D* pVtx;

			//頂点バッファをロックし、頂点データへのポインタを取得

			g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

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

			g_pVtxBuffEnemy->Unlock();
		}
	}
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
	switch (g_aEnemy[nCnt].type) //タイプ１と２
	{
	case ENEMYTYPE_0:

	case ENEMYTYPE_1:

		ActionType1(nCnt); //パターン１

		break;

	//case ENEMYTYPE_2:

	//	ActionType2(nCnt); //パターン２

	//	break;

	//case ENEMYTYPE_3:

	//	ActionType3(nCnt); //パターン３

	//	break;

	//case ENEMYTYPE_4:

	//	ActionType4(nCnt); //パターン4

	//	break;
	}
}
//==================
// 敵の行動パターン１
//==================
void ActionType1(int nCnt)
{
	g_aEnemy[nCnt].nCounterAction++; //行動カウンターをプラス

	Player* pPlayer = GetPlayer();

	if (0 <= g_aEnemy[nCnt].nCounterAction && g_aEnemy[nCnt].nCounterAction < 300)
	{
		if (pPlayer->pos.x <= g_aEnemy[nCnt].pos.x)
		{
			g_aEnemy[nCnt].move.x = -3;
		}
		else
		{
			g_aEnemy[nCnt].move.x = 3;
		}
	}
	else if (300 == g_aEnemy[nCnt].nCounterAction++) //カウンターが480を超えた場合
	{
		if (pPlayer->pos.y <= g_aEnemy[nCnt].pos.y)
		{
			g_aEnemy[nCnt].move.y = -30;
		}
		else if (g_aEnemy[nCnt].pos.y <= pPlayer->pos.y)
		{
			g_aEnemy[nCnt].pos.y++;
		}

		g_aEnemy[nCnt].move.x = 0;
	}
	else if (600 == g_aEnemy[nCnt].nCounterAction++)
	{
		//行動カウンターをリセット

		g_aEnemy[nCnt].nCounterAction = 0;
	}

	//移動処理

	MoveEnemy(nCnt);
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

			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].type]);

			//ポリゴンの描画

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);

		}
	}
}

//===================
// 敵の設定処理
//===================
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE type)
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

			g_aEnemy[nCntEnemy].type = type; //タイプ

			g_aEnemy[nCntEnemy].nCounterAction = 0; //アニメーション

			g_aEnemy[nCntEnemy].nLife = 100; //ライフ

			//後でファイル読み込みにする

			switch(g_aEnemy[nCntEnemy].type) //タイプで比較
			{
			case ENEMYTYPE_0:
				
				break;

			case ENEMYTYPE_1:

				
				break;

			case ENEMYTYPE_2:

				g_aEnemy[nCntEnemy].fSpeed = 5.0f;
				
				break;

			case ENEMYTYPE_3:

				g_aEnemy[nCntEnemy].fSpeed = 2.5f;

				break;

			case ENEMYTYPE_4:

				g_aEnemy[nCntEnemy].fSpeed = 5.0f;

				

				break;
			}

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

	//ライフを減らす
	
	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0) //敵の体力がなくなった
	{
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
