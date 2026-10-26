//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#include "main.h"
#include "input.h"
#include "title.h"
#include "selectstage.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "sound.h"

//マクロ定義

#define CLASS_NAME "WindowClass" //ウインドウクラスの名前

#define WINDOW_NAME "Clock On" //ウインドウの名前（キャプションに表示）

//プロトタイプ宣言

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //ウインドウプロシージャ

HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow); //初期化

void Uninit(void); //終了

void Update(void); //更新

void Draw(void); //描画

void DrawFPS(void);

//グローバル変数

LPDIRECT3D9 g_pD3D = NULL; //Direct3Dオブジェクトへのポインタ

LPDIRECT3DDEVICE9 g_pD3DDevice = NULL; //Direct3Dデバイスへのポインタ

bool g_bDispDebug = false; //デバッグ表示のON/OFF

MODE g_mode = MODE_TITLE; //現在のモード

LPD3DXFONT g_pFont = NULL; //フォントへのポインタ

int g_nCountFPS = 0; //ＦＰＳのカウンタ

//======================================
// メイン関数
//======================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX), //WNDCLASSEXのメモリサイズ

		CS_CLASSDC, //ウインドウのスタイル

		WindowProc, //ウインドウプロシージャ

		0, //ゼロにする（通常は使用しない）

		0,//ゼロにする（通常は使用しない）

		hInstance, //インスタンスハンドル

		LoadIcon(NULL, IDI_APPLICATION), //タスクバーのアイコン

		LoadCursor(NULL, IDC_ARROW), //マウスのカーソル

		(HBRUSH)(COLOR_WINDOW + 1), //クライアント領域の背景色

		NULL, //メニューバー

		CLASS_NAME, //ウインドウクラスの名前

		LoadIcon(NULL, IDI_APPLICATION), //ファイルのアイコン
	};

	HWND hWnd; //ウインドウハンドル（識別子）

	MSG msg; //メッセージを格納する変数

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; //画面サイズの構造体

	//ウインドウクラスの登録

	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウの生成

	hWnd = CreateWindowEx(

		0, //拡張ウインドウスタイル

		CLASS_NAME, //ウインドウクラスの名前

		WINDOW_NAME, //ウインドウの名前

		WS_OVERLAPPEDWINDOW, //ウインドウスタイル

		CW_USEDEFAULT, //ウインドウの左上X座標

		CW_USEDEFAULT, //ウインドウの左上Y座標

		(rect.right - rect.left), //ウインドウの幅

		(rect.bottom - rect.top), //ウインドウの高さ

		NULL, //親ウインドウのハンドル

		NULL, //メニューハンドルまたは子ウインドウID

		hInstance, //インスタンスハンドル

		NULL //ウインドウ作成データ
	);

	DWORD dwCurrentTime; //現在時刻

	DWORD dwExecLastTime; //最後に処理した時刻

	//初期化処理

	if (FAILED(Init(hInstance, hWnd, TRUE))) //初期化処理が失敗した場合
	{
		return -1;
	}

	//分解能を定義

	timeBeginPeriod(1);

	dwCurrentTime = 0; //初期化する

	dwExecLastTime = timeGetTime(); //現在時刻を取得（保存）

	DWORD dwFrameCount = 0;

	DWORD dwFPSLastTime = timeGetTime();

	//ウインドウの表示

	ShowWindow(hWnd, nCmdShow); //ウインドウの表示状態を設定

	UpdateWindow(hWnd);

	//メッセージループ

	while (1) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{ //↑メッセージキューからメッセージを取得＜WM_QUITメッセージを取得した場合０で返す
			//windowsの処理
		
			if (msg.message == WM_QUIT)
			{
				//WM_QUITメッセージを受け取ったらメッセージループを抜ける

				break;
			}
			else
			{
				//メッセージの設定

				TranslateMessage(&msg); //仮想キーメッセージを文字メッセージへ転換

				DispatchMessage(&msg); //ウインドウプロシージャへメッセージを創出
			}

		}
		else
		{
			//DirectXの処理
			
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{ //0.5秒経過

				//FPSを計算

				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;

				dwFrameCount = 0;
			}

			if (dwCurrentTime - dwExecLastTime >= (1000 / 60))
			{//60分の1秒の経過

				dwExecLastTime = dwCurrentTime; //処理開始の時刻「現在時刻」を保存
			
					//更新処理

					Update();

					//描画処理

					Draw();

					dwFrameCount++;
			}
		}
		
	}

	//終了処理

	Uninit();

	//分解能を戻す

	timeEndPeriod(1);

	//ウインドウクラスの登録を解除

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=======================
//ウインドウプロシージャ
//=======================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:

		//WM_QULTメッセージを送る

		PostQuitMessage(0);

		break;

	case WM_KEYDOWN: //キー押下のメッセージ

		switch (wParam)
		{
		case VK_ESCAPE: //【ESC】キーが押された

			//ウインドウを破棄する（WM_DESTROYメッセージを送る）

			DestroyWindow(hWnd);
		
			break;

		}

		break;

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); //既定の処理を返す
}

//===========================
// 初期化処理
//===========================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{                                               //↑TRUE;ウインドウ　FALSE;フルスクリーン
	D3DDISPLAYMODE d3ddm;

	D3DPRESENT_PARAMETERS d3dpp;

	//Direct3Dオブジェクトの生成

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得

	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定

	ZeroMemory(&d3dpp, sizeof(d3dpp)); //パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH; //ゲーム画面のサイズ（幅）

	d3dpp.BackBufferHeight = SCREEN_HEIGHT; //ゲーム画面のサイズ（高さ）

	d3dpp.BackBufferFormat = d3ddm.Format; //バックバッファの形式

	d3dpp.BackBufferCount = 1; //バックバッファの数

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; //ダブルバッファの切り替え（映像信号に同期）

	d3dpp.EnableAutoDepthStencil = TRUE; //デスクバッファとしてステンシルバッファを作成

	d3dpp.AutoDepthStencilFormat = D3DFMT_D16; //デプスバッファとして16bitを使う

	d3dpp.Windowed = bWindow; //ウインドウモード

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; //リフレッシュレート

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; //インターバル

	//Direct3Dデバイスの生成

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, //描画処理と頂点処理をハードウェアで行う

		D3DDEVTYPE_HAL,

		hWnd,

		D3DCREATE_HARDWARE_VERTEXPROCESSING,

		&d3dpp,

		&g_pD3DDevice)))
	{

		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, //描画処理とハードウェア、頂点処理はCPUで行う

			D3DDEVTYPE_HAL,

			hWnd,

			D3DCREATE_SOFTWARE_VERTEXPROCESSING,

			&d3dpp,

			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, //描画処理と頂点処理をcpu行う

				D3DDEVTYPE_REF,

				hWnd,

				D3DCREATE_SOFTWARE_VERTEXPROCESSING,

				&d3dpp,

				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}



	//レンダーステートの設定

	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); //カリングの設定 不要なポリゴンを描画しないことで処理負荷を軽減する

	//アルファブレンドの設定

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); //画面に描画されている部分とこれから描画するポリゴンの色を混ぜ合わせる

	//サンプラーステートの設定 テクスチャの拡縮保管の設定

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	 
	//テクスチャステージステートの設定 テクスチャのアルファブレンドの設定

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//デバッグ表示用フォントを生成

	D3DXCreateFont(g_pD3DDevice, 18, 0, FW_BOLD, 0, FALSE, SHIFTJIS_CHARSET,

		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		
		"Terminal", &g_pFont);

	// マウスカーソルを隠す
	
	ShowCursor(FALSE);

	//+--------------------------------+
	//|  各種オブジェクトの初期化処理  |
	//+--------------------------------+

	

	//キーボードの初期化処理

	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	if (FAILED(InitMouse(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//ジョイパッドの初期化処理

	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	//サウンドの初期化処理

	InitSound(hWnd);

	//フェードの設定

	InitFade(g_mode);

	return S_OK;

}

//=========================
//終了処理
//=========================
void Uninit(void)
{

	// マウスカーソルを表示する（元に戻す）
	
	ShowCursor(TRUE);

	//+--------------------------------+
	//|   各種オブジェクトの終了処理   |
	//+--------------------------------+



	//サウンドの停止

	StopSound();

	//サウンドの終了処理

	UninitSound();

	//マウスの終了処理

	UninitMouse();

	//キーボードの終了処理

	UninitKeyboard();

	//ジョイパッドの終了処理

	UninitJoypad();

	//デバッグ表示用フォントの破棄

	if (g_pFont != NULL)
	{
		g_pFont->Release();

		g_pFont = NULL;
	}

	//フェードの終了処理

	UninitFade();

	//Direct3Dデバイスの破棄

	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();

		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄

	if (g_pD3D != NULL)
	{
		g_pD3D->Release();

		g_pD3D = NULL;
	}
}

//==============================
//更新処理
//==============================
void Update(void)
{
	//キーボードの更新処理

	UpdateKeyboard();

	//マウスの更新処理

	UpdateMouse();
	
	//ジョイパッドの更新処理
	
	UpdateJoypad();

	switch (g_mode)
	{
	case MODE_TITLE:

		UpdateTitle();

		break;

	case MODE_SELECTSTAGE:

		UpdateSelectStage();

		break;

	case MODE_GAME:

		UpdateGame();

		break;

	case MODE_RESULT:

		UpdateResult();

		break;
	}

	//フェードの更新処理

	UpdateFade();
}

//======================================
//描画処理 
//======================================
void Draw(void)
{
	//画面クリア（バックバッファとZバッファのクリア）

	g_pD3DDevice->Clear(0,NULL,
		
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),

		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始

	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
	
	//+--------------------------------+
	//|   各種オブジェクトの描画処理   |
	//+--------------------------------+

		switch (g_mode)
		{
		case MODE_TITLE:

			DrawTitle();

			break;

		case MODE_SELECTSTAGE:

			DrawSelectStage();

			break;

		case MODE_GAME:

			DrawGame();

			break;

		case MODE_RESULT:

			DrawResult();

			break;
		}

		//フェードの描画処理

		DrawFade();

#ifdef _DEBUG

		//FPSの描画

		DrawFPS();
#endif

		//描画終了

		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	
}

//==========================
// デバイスの取得
//==========================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//==========================
// モードの設定
//==========================
void SetMode(MODE mode)
{
	//現在のモードの終了処理

	switch (g_mode)
	{
	case MODE_TITLE:

		UninitTitle(); //タイトル画面

		break;

	case MODE_SELECTSTAGE:

		UninitSelectStage();

		break;

	case MODE_GAME:

		UninitGame(); //ゲーム画面

		break;

	case MODE_RESULT:

		UninitResult(); //リザルト画面

		break;
	}

	g_mode = mode; //モードを保存する

	//新しいモードの初期化処理

	switch (g_mode)
	{
	case MODE_TITLE:

		InitTitle(); //タイトル画面

		break;

	case MODE_SELECTSTAGE:

		InitSelectStage();

		break;

	case MODE_GAME:

		InitGame(); //ゲーム画面

		break;

	case MODE_RESULT:

		InitResult(); //リザルト画面

		break;
	}
}

//==========================
// モードの取得
//==========================
MODE GetMode(void)
{
	return g_mode;
}

//==========================
// FPSの表示処理
//==========================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aStr[256];

	//文字列に代入

	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//テキストの描画

	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}