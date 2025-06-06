#pragma once
#define NOMINMAX 
#pragma region ヘッダ
#include <d3dx9.h>//WinAPI&DirectX用ヘッダ
#include <dinput.h>//dinputヘッダ

//マクロ定数
#define WIN_W 640 //プレイ画面の幅
#define WIN_H 480 //プレイ画面の高さ
#define KEY_SIZE 256//キーの個数
#define VRAM_SIZE WIN_W * WIN_H //VRAM配列のサイズ
#define SCALE 2 //拡大率
#pragma endregion


#pragma region 外部公開
extern HWND h_WIN;//プレイ中のウィンドウ
extern unsigned VRAM[VRAM_SIZE];//画面データ用RAM;//VRAM配列のサイズ
extern unsigned HAIKEI[VRAM_SIZE];//背景画面用領域確保
extern unsigned HAIKEI2[VRAM_SIZE];//背景画面用領域確保
extern bool finish;//プレイ終了フラグ
extern LPDIRECTINPUTDEVICE8 INPUT_DEV;//DirectX入力デバイスインターフェース
extern void input_init();//入力デバイス初期設定関数
extern void key_read();//キーボードを読む
extern bool key_on(BYTE key);//キーオン反応
extern bool key_touch(BYTE key);//キーオン反応
extern LPDIRECT3DDEVICE9 GRAPHIC_DEV;//DirectX描画デバイスインターフェース
extern LPDIRECT3DSURFACE9  BACK;// バックバッファインターフェース
extern LPDIRECT3DSURFACE9  FRONT;// フロントバッファインターフェース
extern void graphic_init();//グラフィックデバイス初期設定関数
extern void dx_draw(void);//DirectXでピクセル描画
#pragma endregion 