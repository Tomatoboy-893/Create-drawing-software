#pragma region ヘッダ
#include "dx6.h"
#include "help4.h"
#include "pix2.h"
//c++ コンテナ
#include <string>//文字列
#include <vector>//動的配列 (スポイト機能で使用)
//名前空間
using namespace std;//標準名前空間
#pragma endregion


#pragma region 列挙・構造体
//BIT構造体
struct BIT //0930追加
{
	int x;//x座標(中心)
	int y;//y座標(中心)

	int w;//幅
	int h;//高さ

	int size;//ビットサイズ

	int vx;//速度x
	int vy;//速度y

	unsigned color;//色

	float theta;//角度

	char map[100][100];//マップ
};
#pragma endregion

#pragma region 物理定数定義
int time = 0;
#pragma endregion


#pragma region BIT用定数・変数定義
//1ビット
BIT one_bit = { 100, 100, 1, 1, 128 / 8, //位置x,位置y,幅,高さ,ビットサイズ，
0, 0, 0xffff0000, 0,//速度x,速度y,色,角度
{
	"1",
},
};

//十字
BIT cross = { 200, 200, 3, 3, 128 / 8, //位置x,位置y,幅,高さ,ビットサイズ，
4, 4, 0xffff0000, 0,//速度x,速度y,色,角度
{
	"111",
	"111",
	"111",
},
};

//数字 "0123456789. "
BIT suuji_bit = { 0, 0, 3, 4, 8, //位置x,位置y,幅,高さ,ビットサイズ，
 0, 0, 0, 0,//速度x,速度y,色,角度
{
	"111110111111101111111111111111000000",
	"101010001001101100100001101101000000",
	"101010100011111001101001111001000000",
	"111010111111001111111001111111100000",
},
};

//テキストビット
BIT text_bit = { WIN_W / 2, WIN_H - 10 * 2 / 2, 0, 4, 6, //位置x,位置y,幅,高さ,ビットサイズ，
0, 0, 0xffffffff, 0,//速度x,速度y,色,角度
{
	"",
	"",
	"",
	"",
},
};

// 消しゴム機能用の変数と定数
bool is_eraser_mode = false; // false: 描画モード, true: 消しゴムモード
const unsigned int ERASER_DRAW_COLOR = 0xff000000;      // 消しゴムが「実際に塗る色」(背景の黒)
const unsigned int ERASER_CURSOR_COLOR = 0xffffffff; // 消しゴムモード時の「カーソルの表示色」(白)

// 万華鏡モード用の変数
int kaleidoscope_mode = 0; // 0: オフ, 4: 4分割モード, 8: 8分割モード

const string SUUJI_TABLE = "0123456789. ";
const int SUUJI_WS[12] = { 3, 2, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1 };
string text_string = "255.255.000.000";
#pragma endregion


#pragma region 関数宣言
void bit_draw(BIT& bit);
void bit_draw_haikei(BIT& bit);
void bit_draw_haikei_alpha(BIT& bit);
void text_update(BIT& text, string mojis);
void cross_update(BIT& cross);
void flip_background_vertical();
void flip_background_horizontal();
#pragma endregion


#pragma region ゲームループ関数定義
//初期化
void initialize()
{
	input_init();
	graphic_init();

	cross_update(cross);
	text_update(text_bit, text_string);

	bit_draw(cross);
	bit_draw(text_bit);
}

//更新
void update()
{
	key_read();

	if (key_on(DIK_ESCAPE))
	{
		finish = true;
	}

	// Eキーで描画/消しゴムモードを切り替える
	static bool e_key_pressed_last_frame = false;
	if (key_on(DIK_E)) {
		if (!e_key_pressed_last_frame) {
			is_eraser_mode = !is_eraser_mode;
		}
		e_key_pressed_last_frame = true;
	}
	else {
		e_key_pressed_last_frame = false;
	}


	// Fキーで上下反転
	static bool f_key_pressed_last_frame = false;
	if (key_on(DIK_F)) {
		if (!f_key_pressed_last_frame) {
			flip_background_vertical();
		}
		f_key_pressed_last_frame = true;
	}
	else {
		f_key_pressed_last_frame = false;
	}

	// Mキーで左右反転
	static bool m_key_pressed_last_frame = false;
	if (key_on(DIK_M)) {
		if (!m_key_pressed_last_frame) {
			flip_background_horizontal();
		}
		m_key_pressed_last_frame = true;
	}
	else {
		m_key_pressed_last_frame = false;
	}

	// ★追加：Kキーで万華鏡モードを切り替える
	static bool k_key_pressed_last_frame = false;
	if (key_on(DIK_K)) { // Kキー (Kaleidoscope)
		if (!k_key_pressed_last_frame) {
			kaleidoscope_mode += 4;
			if (kaleidoscope_mode > 8) {
				kaleidoscope_mode = 0; // 8の次は0(オフ)に戻す
			}
		}
		k_key_pressed_last_frame = true;
	}
	else {
		k_key_pressed_last_frame = false;
	}


	cross_update(cross);

	time++;
}

//描画
void draw()
{
	memcpy(VRAM, HAIKEI, sizeof(VRAM));

	if (kaleidoscope_mode == 4 || kaleidoscope_mode == 8)
	{
		// --- 万華鏡モードのカーソル描画 ---
		int original_x = cross.x;
		int original_y = cross.y;
		const int cx = WIN_W / 2;
		const int cy = WIN_H / 2;
		int dx = original_x - cx;
		int dy = original_y - cy;

		int points_x[8];
		int points_y[8];
		int num_points = (kaleidoscope_mode == 4) ? 4 : 8;

		// 4分割と8分割の座標を一度に計算
		points_x[0] = cx + dx; points_y[0] = cy + dy;
		points_x[1] = cx - dx; points_y[1] = cy + dy;
		points_x[2] = cx + dx; points_y[2] = cy - dy;
		points_x[3] = cx - dx; points_y[3] = cy - dy;
		if (kaleidoscope_mode == 8) {
			points_x[4] = cx + dy; points_y[4] = cy + dx;
			points_x[5] = cx - dy; points_y[5] = cy + dx;
			points_x[6] = cx + dy; points_y[6] = cy - dx;
			points_x[7] = cx - dy; points_y[7] = cy - dx;
		}

		// カーソルの色を設定
		unsigned int original_pen_color = cross.color;
		if (is_eraser_mode) {
			cross.color = ERASER_CURSOR_COLOR; // 消しゴムモードなら白カーソル
		}

		// 全ての対称点にカーソルを描画
		for (int i = 0; i < num_points; ++i) {
			cross.x = points_x[i];
			cross.y = points_y[i];
			bit_draw(cross);
		}

		// 座標と色を元に戻す
		cross.x = original_x;
		cross.y = original_y;
		cross.color = original_pen_color;
	}
	else
	{
		// --- 通常モードのカーソル描画 ---
		if (is_eraser_mode)
		{
			unsigned int original_pen_color = cross.color;
			cross.color = ERASER_CURSOR_COLOR;
			bit_draw(cross);
			cross.color = original_pen_color;
		}
		else
		{
			bit_draw(cross);
		}
	}

	bit_draw(text_bit);
	dx_draw();
}

//片づけ
void clear()
{
	INPUT_DEV->Release();
	GRAPHIC_DEV->Release();
	BACK->Release();
	FRONT->Release();

	INPUT_DEV = NULL;
	GRAPHIC_DEV = NULL;
	BACK = NULL;
	FRONT = NULL;
}
#pragma endregion


#pragma region 描画関数定義
//BITオブジェクトの描画
void bit_draw(BIT& bit)
{
	int s = SCALE * bit.size;
	int n = 0;
	int X = 0;
	int Y = 0;

	int xx, yy, xx2, yy2;

	for (int x2 = 0; x2 < bit.w; x2++)		for (int y2 = 0; y2 < bit.h; y2++)
	{
		if (bit.map[y2][x2] == '0') continue;
		for (int x = 0; x < s; x++)		for (int y = 0; y < s; y++)
		{
			xx = x2 * s + x - bit.w * s / 2;
			yy = y2 * s + y - bit.h * s / 2;

			xx2 = xx * cos(bit.theta) + yy * sin(-bit.theta);
			yy2 = xx * sin(bit.theta) + yy * cos(bit.theta);
			xx = xx2;
			yy = yy2;

			xx = xx / SCALE;
			yy = yy / SCALE;

			X = xx + bit.x;
			Y = yy + bit.y;

			n = X + Y * WIN_W;

			if (0 <= X && X < WIN_W && 0 <= Y && Y < WIN_H)
				VRAM[n] = bit.color;
		}
	}
}

//BITオブジェクトを背景に描画
void bit_draw_haikei(BIT& bit)
{
	int s = SCALE * bit.size;
	int n = 0;
	int X = 0;
	int Y = 0;

	int xx, yy, xx2, yy2;

	for (int x2 = 0; x2 < bit.w; x2++)		for (int y2 = 0; y2 < bit.h; y2++)
	{
		if (bit.map[y2][x2] == '0') continue;
		for (int x = 0; x < s; x++)		for (int y = 0; y < s; y++)
		{
			xx = x2 * s + x - bit.w * s / 2;
			yy = y2 * s + y - bit.h * s / 2;

			xx2 = xx * cos(bit.theta) + yy * sin(-bit.theta);
			yy2 = xx * sin(bit.theta) + yy * cos(bit.theta);
			xx = xx2;
			yy = yy2;

			xx = xx / SCALE;
			yy = yy / SCALE;

			X = xx + bit.x;
			Y = yy + bit.y;

			n = X + Y * WIN_W;

			if (0 <= X && X < WIN_W && 0 <= Y && Y < WIN_H)
				HAIKEI[n] = bit.color;
		}
	}
}

//BITオブジェクトを背景にアルファブレンド描画
void bit_draw_haikei_alpha(BIT& bit)
{
	int s = SCALE * bit.size;
	int n = 0;
	int X = 0;
	int Y = 0;

	int xx, yy, xx2, yy2;

	for (int x2 = 0; x2 < bit.w; x2++)		for (int y2 = 0; y2 < bit.h; y2++)
	{
		if (bit.map[y2][x2] == '0') continue;
		for (int x = 0; x < s; x++)		for (int y = 0; y < s; y++)
		{
			xx = x2 * s + x - bit.w * s / 2;
			yy = y2 * s + y - bit.h * s / 2;

			xx2 = xx * cos(bit.theta) + yy * sin(-bit.theta);
			yy2 = xx * sin(bit.theta) + yy * cos(bit.theta);
			xx = xx2;
			yy = yy2;

			xx = xx / SCALE;
			yy = yy / SCALE;

			X = xx + bit.x;
			Y = yy + bit.y;

			n = X + Y * WIN_W;

			if (0 <= X && X < WIN_W && 0 <= Y && Y < WIN_H)
				color_w(HAIKEI[n], bit.color);
		}
	}
}
#pragma endregion


#pragma region 更新関数定義
//テキストの更新
void text_update(BIT& text, string mojis)
{
	int w = suuji_bit.w;
	int h = suuji_bit.h;
	int& text_w = text.w;
	text_w = 0;

	int n;

	for (int i = 0; i < mojis.size(); i++)
	{
		n = SUUJI_TABLE.find_first_of(mojis[i]);

		for (int x = w * n; x < w * n + SUUJI_WS[n]; x++)
		{
			for (int y = 0; y < h; y++)
			{
				text.map[y][text_w] = suuji_bit.map[y][x];
			}
			text_w++;
		}

		for (int y = 0; y < h; y++)
		{
			text.map[y][text_w] = '0';
		}
		text_w++;
	}
}

//BITの更新(描く十字本体の更新)
void cross_update(BIT& cross)
{
	//矢印キーで移動する
	if (key_on(DIK_RIGHT))
		cross.x += cross.vx;
	if (key_on(DIK_LEFT))
		cross.x -= cross.vx;
	if (key_on(DIK_DOWN))
		cross.y += cross.vy;
	if (key_on(DIK_UP))
		cross.y -= cross.vy;

	//画面内にバウンドする
	bound(cross.x, 0, WIN_W);
	bound(cross.y, 0, WIN_H);

	//Dキーで描画/消去を実行する
	if (key_on(DIK_D))
	{
		// 万華鏡モードの描画処理
		if (kaleidoscope_mode == 4 || kaleidoscope_mode == 8)
		{
			int original_x = cross.x;
			int original_y = cross.y;
			const int cx = WIN_W / 2;
			const int cy = WIN_H / 2;

			int dx = original_x - cx;
			int dy = original_y - cy;

			int points_x[8];
			int points_y[8];
			int num_points = 0;

			if (kaleidoscope_mode == 4) {
				num_points = 4;
				points_x[0] = cx + dx; points_y[0] = cy + dy;
				points_x[1] = cx - dx; points_y[1] = cy + dy;
				points_x[2] = cx + dx; points_y[2] = cy - dy;
				points_x[3] = cx - dx; points_y[3] = cy - dy;
			}
			else { // kaleidoscope_mode == 8
				num_points = 8;
				points_x[0] = cx + dx; points_y[0] = cy + dy;
				points_x[1] = cx - dx; points_y[1] = cy + dy;
				points_x[2] = cx + dx; points_y[2] = cy - dy;
				points_x[3] = cx - dx; points_y[3] = cy - dy;
				points_x[4] = cx + dy; points_y[4] = cy + dx;
				points_x[5] = cx - dy; points_y[5] = cy + dx;
				points_x[6] = cx + dy; points_y[6] = cy - dx;
				points_x[7] = cx - dy; points_y[7] = cy - dx;
			}

			if (is_eraser_mode)
			{
				unsigned int original_pen_color = cross.color;
				cross.color = ERASER_DRAW_COLOR;
				for (int i = 0; i < num_points; ++i) {
					cross.x = points_x[i];
					cross.y = points_y[i];
					bit_draw_haikei(cross);
				}
				cross.color = original_pen_color;
			}
			else
			{
				for (int i = 0; i < num_points; ++i) {
					cross.x = points_x[i];
					cross.y = points_y[i];
					bit_draw_haikei_alpha(cross);
				}
			}

			cross.x = original_x;
			cross.y = original_y;
		}
		else // 通常モード (kaleidoscope_mode == 0)
		{
			if (is_eraser_mode)
			{
				unsigned int original_pen_color = cross.color;
				cross.color = ERASER_DRAW_COLOR;
				bit_draw_haikei(cross);
				cross.color = original_pen_color;
			}
			else
			{
				bit_draw_haikei_alpha(cross);
			}
		}
	}


	//筆のサイズ変更
	if (key_on(DIK_Q))
	{
		if (key_on(DIK_SEMICOLON))
		{
			cross.size++;

		}
		if (key_on(DIK_MINUS))
		{
			cross.size--;
		}
		if (cross.size < 1)
		{
			cross.size = 1;
		}
	}


	// スポイト機能 (Pキー)
	static bool p_key_pressed_last_frame = false;
	if (key_on(DIK_P)) {
		if (!p_key_pressed_last_frame) {
			const int sample_area_size = 4;
			const int half_sample_area = sample_area_size / 2;
			long long sum_a = 0, sum_r = 0, sum_g = 0, sum_b = 0;
			int sampled_pixels_count = 0;

			for (int dy = -half_sample_area; dy < half_sample_area; ++dy) {
				for (int dx = -half_sample_area; dx < half_sample_area; ++dx) {
					int sample_x = cross.x + dx;
					int sample_y = cross.y + dy;

					if (sample_x >= 0 && sample_x < WIN_W && sample_y >= 0 && sample_y < WIN_H) {
						unsigned int pixel_color = HAIKEI[sample_y * WIN_W + sample_x];
						sum_a += (pixel_color >> 24) & 0xFF;
						sum_r += (pixel_color >> 16) & 0xFF;
						sum_g += (pixel_color >> 8) & 0xFF;
						sum_b += pixel_color & 0xFF;
						sampled_pixels_count++;
					}
				}
			}

			if (sampled_pixels_count > 0) {
				unsigned char avg_a = sum_a / sampled_pixels_count;
				unsigned char avg_r = sum_r / sampled_pixels_count;
				unsigned char avg_g = sum_g / sampled_pixels_count;
				unsigned char avg_b = sum_b / sampled_pixels_count;

				cross.color = (avg_a << 24) | (avg_r << 16) | (avg_g << 8) | avg_b;
				one_bit.color = cross.color;

				char temp_buffer[4];
				sprintf_s(temp_buffer, 4, "%03d", avg_a);
				text_string.replace(0, 3, temp_buffer);
				sprintf_s(temp_buffer, 4, "%03d", avg_r);
				text_string.replace(4, 3, temp_buffer);
				sprintf_s(temp_buffer, 4, "%03d", avg_g);
				text_string.replace(8, 3, temp_buffer);
				sprintf_s(temp_buffer, 4, "%03d", avg_b);
				text_string.replace(12, 3, temp_buffer);
				text_update(text_bit, text_string);
			}
		}
		p_key_pressed_last_frame = true;
	}
	else {
		p_key_pressed_last_frame = false;
	}


	// argbキーで色を変える
	bool argb_key[4] = { key_on(DIK_B), key_on(DIK_G), key_on(DIK_R), key_on(DIK_A) };
	for (int i = 0; i < MAX_ARGB; i++)
	{
		if (!argb_key[i]) continue;
		int dir = 0;
		if (key_on(DIK_SEMICOLON)) dir = 1;
		else if (key_on(DIK_MINUS)) dir = -1;
		if (dir == 0) continue;

		unsigned& c = cross.color;
		int c2 = color_r(c, (ARGB)i);

		if (dir != 0) c2 += dir;
		if (c2 < 0) c2 = 0;
		if (c2 > 255) c2 = 255;

		color_w(c, c2, (ARGB)i);
		color_w(one_bit.color, c2, (ARGB)i);

		char mojis[100] = "000";
		sprintf_s(mojis, sizeof(mojis), "%03d", c2);
		text_string.replace((3 - i) * 4, 3, mojis);
		text_update(text_bit, text_string);
	}
}

// 背景反転関数の定義
void flip_background_vertical() {
	unsigned int* temp_row = new unsigned int[WIN_W];
	for (int y = 0; y < WIN_H / 2; ++y) {
		memcpy(temp_row, &HAIKEI[y * WIN_W], WIN_W * sizeof(unsigned int));
		memcpy(&HAIKEI[y * WIN_W], &HAIKEI[(WIN_H - 1 - y) * WIN_W], WIN_W * sizeof(unsigned int));
		memcpy(&HAIKEI[(WIN_H - 1 - y) * WIN_W], temp_row, WIN_W * sizeof(unsigned int));
	}
	delete[] temp_row;
}

void flip_background_horizontal() {
	for (int y = 0; y < WIN_H; ++y) {
		for (int x = 0; x < WIN_W / 2; ++x) {
			unsigned int temp_pixel = HAIKEI[y * WIN_W + x];
			HAIKEI[y * WIN_W + x] = HAIKEI[y * WIN_W + (WIN_W - 1 - x)];
			HAIKEI[y * WIN_W + (WIN_W - 1 - x)] = temp_pixel;
		}
	}
}
#pragma endregion