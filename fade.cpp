//=============================
// 
//アクションゲームmaster版
// Author森田煌也
// 
//=============================
#include "fade.h"

#include "sound.h"

//グローバル変数

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL; //頂点バッファのポインタ

FADE g_fade; //フェードの状態

MODE g_modeNext; //次の画面（モード）

D3DXCOLOR g_colorFade; //ポリゴン（フェード）の色

//フェードの初期化処理

void InitFade(MODE modeNext)
{
	g_fade = FADE_OUT; //フェードイン状態に

	g_modeNext = modeNext; //次の画面を設定

	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f); //黒いポリゴン（不透明）にしておく

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 , //確保するバッファのサイズ
													   //sizeof(VERTEX_2D)*必要な超点数

		D3DUSAGE_WRITEONLY,

		FVF_VERTEX_2D, //頂点フォーマット

		D3DPOOL_MANAGED,

		&g_pVtxBuffFade,

		NULL);

	//デバイスの取得

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);

	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);

	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定 1.0f固定

	pVtx[0].rhw = 1.0f;

	pVtx[1].rhw = 1.0f;

	pVtx[2].rhw = 1.0f;

	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定（０．０f～１．０ｆで設定）

	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	g_pVtxBuffFade->Unlock();

	//モードの設定　

	SetMode(g_modeNext);
}

//終了処理
void UninitFade(void)
{
	//頂点バッファの破棄


	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();

		g_pVtxBuffFade = NULL;
	}
}

//更新処理
void UpdateFade(void)
{
	if (g_fade != FADE_NONE)
	{

		if (g_fade == FADE_IN)
		{ //フェードイン状態

			g_colorFade.a -= 0.12f; //ポリゴンを透明にしていく

			if (g_colorFade.a <= 0.0f)
			{
				g_colorFade.a = 0.0f;

				g_fade = FADE_NONE; //何もしていない状態にする				
			}

		}
		else if (g_fade == FADE_OUT)
		{ //フェードアウト状態

			g_colorFade.a += 0.12f; //ポリゴンを不透明にしていく

			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;

				g_fade = FADE_IN; //フェードイン状態にする

				//モード設定（次の画面に移行）

				SetMode(g_modeNext);
			}
		}
	}

	//デバイスの取得

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定（０．０f～１．０ｆで設定）

	pVtx[0].col = g_colorFade;

	pVtx[1].col = g_colorFade;

	pVtx[2].col = g_colorFade;

	pVtx[3].col = g_colorFade;

	g_pVtxBuffFade->Unlock();
}

//フェードの描画処理
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定

	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定

	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定

	pDevice->SetTexture(0, NULL); //テクスチャを使用しないときはNULLを指定する

	//ポリゴンの描画

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//フェードの設定
void SetFade(MODE modeNext)
{
	StopSound(SOUND_LABEL_CLOCK); //時間停止のbgmを停止

	g_fade = FADE_OUT; //フェードアウト状態に

	g_modeNext = modeNext;

	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); //黒いポリゴン（透明）にしておく
}

//フェードの取得
FADE GetFade(void)
{
	return g_fade;
}