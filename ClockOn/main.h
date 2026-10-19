//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================

#ifndef _MAIN_H_ //このマクロ定義がされてなかったら

#define _MAIN_H_ //２重インクルード防止のマクロを定義する。

#include<windows.h>

#include "d3dx9.h" //描画処理に必要

#define DIRECTINPUT_VERSION (0x0800)//ビルド時の警告対処用マクロ

#include "dinput.h" //入力処理に必要

#include "xaudio2.h" //サウンド処理に必要

#include "xinput.h" //ジョイパッド処理に必要

//ライブラリのリンク

#pragma comment(lib, "d3d9.lib") //描画処理に必要

#pragma comment(lib, "d3dx9.lib") //[d3d9.lib]の拡張ライブラリ

#pragma comment(lib, "dxguid.lib") //DirectXコンポーネント（部品）使用に必要

#pragma comment(lib, "winmm.lib") //システム時刻取得に必要

#pragma comment(lib, "dinput8.lib") //入力処理に必要

#pragma comment(lib, "xinput.lib") //ジョイパッド処理に必要

//マクロ定義

#define SCREEN_WIDTH (1280) //ウインドウの幅

#define SCREEN_HEIGHT (720) //ウインドウの高さ

#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//頂点情報【２Ｄ】の構造体を定義

typedef struct
{
	D3DXVECTOR3 pos; //頂点座標（ｘｙｚ）

	float rhw; //座標変換用係数（1.0fで固定）

	D3DCOLOR col; //頂点カラー（r、g、b、a）

	D3DXVECTOR2 tex; //テクスチャ座標（ｘ、ｙ）

}VERTEX_2D;

//画面（モード）の種類

typedef enum
{
	MODE_TITLE = 0, //タイトル

	MODE_GAME, //ゲーム

	MODE_RESULT, //リザルト

	MODE_MAX,
}MODE;

//プロトタイプ宣言

LPDIRECT3DDEVICE9 GetDevice(void); //デバイスの取得

void SetMode(MODE mode); //モードの設定

MODE GetMode(void); //モードの取得

LPD3DXFONT GetFont();

#endif