//=============================
// 
//アクションゲームプロトタイプ版
// Author森田煌也
// 
//=============================
#include "input.h"

//マクロ定義

#define NUM_KEY_MAX (256) //キーの最大数

#define NUM_MOUSE_BUTTON_MAX (8) // マウスボタンの最大数

//グローバル変数

LPDIRECTINPUT8 g_pInput = NULL; //DirectInputオブジェクトへのポインタ

//キーボード

LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL; //入力デバイス（キーボード）へのポインタ

BYTE g_aKeyState[NUM_KEY_MAX]; //キーボードのプレス情報

BYTE g_aKeyStateTrigger[NUM_KEY_MAX]; //キーボードのトリガー情報

//マウス

LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL; // 入力デバイス（マウス）へのポインタ

DIMOUSESTATE2 g_mouseState;             // マウスのプレス・移動情報

DIMOUSESTATE2 g_mouseStateTrigger;      // マウスのトリガー情報

//ジョイパッド

XINPUT_STATE g_joyKeyState; //ジョイパッドのプレス情報

XINPUT_STATE g_joyKeyStateTrigger; //ジョイパッドのプレス情報

//===============================
// キーボードの初期化処理
//===============================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//Directinputオブジェクトの生成

	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,

		IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス（キーボード）の生成

	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定

	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定

	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得

	g_pDevKeyboard->Acquire();

	return S_OK;
}

//===============================
// キーボードの終了処理
//===============================
void UninitKeyboard(void)
{
	//入力デバイス（キーボード）の破棄

	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire(); //キーボードへのアクセス権を放棄

		g_pDevKeyboard->Release();

		g_pDevKeyboard = NULL;
	}

	//DirectInputオブジェクトの破棄

	if (g_pInput != NULL)
	{
		g_pInput->Release();

		g_pInput = NULL;
	}
}

//===============================
// キーボードの更新処理
//===============================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX]; //キーボードの入力情報

	int nCntKey;

	//入力デバイスからデータを取得

	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			//キーボードのトリガー情報を保存

			g_aKeyState[nCntKey] = aKeyState[nCntKey]; //キーボードのプレス情報を保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire(); //キーボードへのアクセス権を獲得
	}
}

//===============================
// キーボードのプレス情報を取得
//===============================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true: false;
	                        //3項演算子　真ならA（true)　偽ならB（false）を返す
}

//===============================
// キーボードのトリガー情報を取得
//===============================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//===============================
// マウスの初期化処理
//===============================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトが未生成の場合は作成
	if (g_pInput == NULL)
	{
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&g_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	// 入力デバイス（マウス）の生成（拡張ボタン対応のためc_dfDIMouse2を使用）
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定 (8ボタン対応)
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// 協調モードを設定（非排他・フォアグラウンド）
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	// メモリのクリア
	memset(&g_mouseState, 0, sizeof(DIMOUSESTATE2));
	memset(&g_mouseStateTrigger, 0, sizeof(DIMOUSESTATE2));

	// マウスへのアクセス権を獲得
	g_pDevMouse->Acquire();

	return S_OK;
}

//===============================
// マウスの終了処理
//===============================
void UninitMouse(void)
{
	// 入力デバイス（マウス）の破棄
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire(); // マウスへのアクセス権を放棄
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}
}

//===============================
// マウスの更新処理
//===============================
void UpdateMouse(void)
{
	DIMOUSESTATE2 mouseState;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState)))
	{
		// ボタンのトリガー情報を取得
		for (int nCntBtn = 0; nCntBtn < NUM_MOUSE_BUTTON_MAX; nCntBtn++)
		{
			g_mouseStateTrigger.rgbButtons[nCntBtn] = (g_mouseState.rgbButtons[nCntBtn] ^ mouseState.rgbButtons[nCntBtn]) & mouseState.rgbButtons[nCntBtn];
		}

		// 現在の状態を保持（移動量 X, Y, Z やボタン状態）（プレス）

		g_mouseState = mouseState;
	}
	else
	{
		// アクセス権が失われている場合は再獲得
		g_pDevMouse->Acquire();
	}
}

//===============================
// マウスボタンのプレス情報を取得
// nButton: 0 = 左, 1 = 右, 2 = 中央ホイールクリック
//===============================
bool GetMousePress(int nButton)
{
	if (nButton < 0 || nButton >= NUM_MOUSE_BUTTON_MAX) return false;
	return (g_mouseState.rgbButtons[nButton] & 0x80) ? true : false;
}

//===============================
// マウスボタンのトリガー情報を取得
// nButton: 0 = 左, 1 = 右, 2 = 中央ホイールクリック
//===============================
bool GetMouseTrigger(int nButton)
{
	if (nButton < 0 || nButton >= NUM_MOUSE_BUTTON_MAX) return false;
	return (g_mouseStateTrigger.rgbButtons[nButton] & 0x80) ? true : false;
}

//===============================
// マウスの移動量（相対値）を取得
// X: 左右移動, Y: 上下移動, Z: ホイール回転
//===============================
D3DXVECTOR3 GetMouseVelocity(void)
{
	return D3DXVECTOR3((float)g_mouseState.lX, (float)g_mouseState.lY, (float)g_mouseState.lZ);
}

//===============================
// ジョイパッドの初期化処理
//===============================
HRESULT InitJoypad(void)
{
	//メモリのクリア

	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));

	//Xinputのステートを設定（有効にする）
	
	XInputEnable(true);

	return S_OK;
}

//===============================
// ジョイパッドの終了処理
//===============================
void UninitJoypad(void)
{
	//Xinputのステートを設定（無効にする）

	XInputEnable(false);
}

//===============================
// ジョイパッドの更新処理
//===============================
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;

	//ジョイパッドの情報を取得

	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		g_joyKeyStateTrigger.Gamepad.wButtons = (g_joyKeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & joykeyState.Gamepad.wButtons;
		//ジョイパッドのトリガー情報を保存

		g_joyKeyStateTrigger.Gamepad.bRightTrigger = (g_joyKeyState.Gamepad.bRightTrigger ^ joykeyState.Gamepad.bRightTrigger) & joykeyState.Gamepad.bRightTrigger;

		g_joyKeyStateTrigger.Gamepad.bLeftTrigger = (g_joyKeyState.Gamepad.bLeftTrigger ^ joykeyState.Gamepad.bLeftTrigger) & joykeyState.Gamepad.bLeftTrigger;

		g_joyKeyState = joykeyState; //ジョイパッドのプレス情報を保存
	}
}

//===============================
// ジョイパッドのプレス情報を取得
//===============================
bool GetJoypadPress(JOYKEY key)
{
	return (g_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//===============================
// ジョイパッドのトリガー情報を取得
//===============================
bool GetJoypadTrigger(JOYKEY Key)
{
	return (g_joyKeyStateTrigger.Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//===============================
//ジョイパッドの左スティックを取得
//===============================
D3DXVECTOR3 GetLeftStick(void)
{
	D3DXVECTOR3 stick = D3DXVECTOR3( 0.0f, 0.0f,0.0f );

	//ジョイパッドの情報を取得

	float x = (float)g_joyKeyState.Gamepad.sThumbLX;

	float y = (float)g_joyKeyState.Gamepad.sThumbLY;

	//中心からの距離を計算

	float magnitude = sqrtf(x * x + y * y);

	//方向を計算

	float dirX = 0.0f;

	float dirY = 0.0f;

	if (magnitude > 0.0f) 
	{
		dirX = x / magnitude;

		dirY = y / magnitude;
	}

	//デッドゾーンの適用

	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		// 最大値を制限（32767を超えないように）
	
		if (magnitude > 32767.0f) magnitude = 32767.0f;

		// デッドゾーンより外側の部分の有効な長さを調整
		
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		//０から１の範囲に正規化

		float normalizedMagnitude = magnitude / (32767.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		//正規化された大きさを代入


		if (1.0f < normalizedMagnitude) //正規化された大きさが１を超えた場合
		{
			normalizedMagnitude = 1.0f;
		}

		// 方向と大きさを掛け合わせて最終的な入力を決定

		stick.x = dirX * normalizedMagnitude; //x

		
		stick.y = dirY * normalizedMagnitude; //y
	}
	else 
	{
		// デッドゾーン内の場合は0にする

		stick.x = 0.0f;

		stick.y = 0.0f;
	}

	return stick;
}

//===============================
//ジョイパッドの右スティックを取得
//===============================
D3DXVECTOR3 GetRightStick(void)
{
	D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//ジョイパッドの情報を取得

	float x = (float)g_joyKeyState.Gamepad.sThumbRX;

	float y = (float)g_joyKeyState.Gamepad.sThumbRY;

	//中心からの距離を計算

	float magnitude = sqrtf(x * x + y * y);

	//方向を計算

	float dirX = 0.0f;

	float dirY = 0.0f;

	if (magnitude > 0.0f)
	{
		dirX = x / magnitude;

		dirY = y / magnitude;
	}

	//デッドゾーンの適用

	if (magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		// 最大値を制限（32767を超えないように）

		if (magnitude > 32767.0f) magnitude = 32767.0f;

		// デッドゾーンより外側の部分の有効な長さを調整

		magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

		//０から１の範囲に正規化

		float normalizedMagnitude = magnitude / (32767.0f - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		//正規化された大きさを代入


		if (1.0f < normalizedMagnitude) //正規化された大きさが１を超えた場合
		{
			normalizedMagnitude = 1.0f;
		}

		// 方向と大きさを掛け合わせて最終的な入力を決定

		stick.x = dirX * normalizedMagnitude; //x


		stick.y = dirY * normalizedMagnitude; //y
	}
	else
	{
		// デッドゾーン内の場合は0にする

		stick.x = 0.0f;

		stick.y = 0.0f;
	}

	return stick;
}

//===============================
//ジョイパッドのRTボタンを取得
//===============================
bool GetRT(void)
{
	if (g_joyKeyState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{ //RTボタンを押していた場合

		return true;
	}
	else
	{ //押していなかった場合
		return false;
	}
}

//===============================
//ジョイパッドのRTボタンのトリガー情報を取得
//===============================
bool GetRTTrigger(void)
{
	if (g_joyKeyStateTrigger.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{ //RTボタンを押していた場合

		return true;
	}
	else
	{ //押していなかった場合
		return false;
	}
}

//===============================
//ジョイパッドのLTボタンのトリガー情報を取得
//===============================
bool GetLTTrigger(void)
{
	if (g_joyKeyStateTrigger.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{ //RTボタンを押していた場合

		return true;
	}
	else
	{ //押していなかった場合
		return false;
	}
}