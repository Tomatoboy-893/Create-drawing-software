#pragma once

#pragma region ARGB�񋓑�
enum ARGB { BLUE, GREEN, RED, AFFINE, MAX_ARGB };//0930�ǉ�
#pragma endregion


#pragma region �O�����J
extern bool break_point_true(bool b);//�^�̎��ɒ�~
extern bool break_point_false(bool b);//�U�̎��ɒ�~
extern unsigned char color_r(unsigned color, ARGB argb);//�J���[�ǂݏo���֐�
extern void color_w(unsigned& color, unsigned char n, ARGB argb);//�J���[�������݁iRGB�l���w��ŏ㏑���j�֐�
extern void color_w(unsigned& haikei, unsigned color);//�J���[�������݁i�A���t�@�u�����h�j�֐�

extern void bound(int& val, int shou, int dai);//�o�E���h�֐�
#pragma endregion

