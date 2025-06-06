#pragma once

#pragma region ARGB列挙体
enum ARGB { BLUE, GREEN, RED, AFFINE, MAX_ARGB };//0930追加
#pragma endregion


#pragma region 外部公開
extern bool break_point_true(bool b);//真の時に停止
extern bool break_point_false(bool b);//偽の時に停止
extern unsigned char color_r(unsigned color, ARGB argb);//カラー読み出し関数
extern void color_w(unsigned& color, unsigned char n, ARGB argb);//カラー書き込み（RGB値を指定で上書き）関数
extern void color_w(unsigned& haikei, unsigned color);//カラー書き込み（アルファブレンド）関数

extern void bound(int& val, int shou, int dai);//バウンド関数
#pragma endregion

