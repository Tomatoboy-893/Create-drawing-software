#pragma once
#define NOMINMAX 
#pragma region �w�b�_
#include <d3dx9.h>//WinAPI&DirectX�p�w�b�_
#include <dinput.h>//dinput�w�b�_

//�}�N���萔
#define WIN_W 640 //�v���C��ʂ̕�
#define WIN_H 480 //�v���C��ʂ̍���
#define KEY_SIZE 256//�L�[�̌�
#define VRAM_SIZE WIN_W * WIN_H //VRAM�z��̃T�C�Y
#define SCALE 2 //�g�嗦
#pragma endregion


#pragma region �O�����J
extern HWND h_WIN;//�v���C���̃E�B���h�E
extern unsigned VRAM[VRAM_SIZE];//��ʃf�[�^�pRAM;//VRAM�z��̃T�C�Y
extern unsigned HAIKEI[VRAM_SIZE];//�w�i��ʗp�̈�m��
extern unsigned HAIKEI2[VRAM_SIZE];//�w�i��ʗp�̈�m��
extern bool finish;//�v���C�I���t���O
extern LPDIRECTINPUTDEVICE8 INPUT_DEV;//DirectX���̓f�o�C�X�C���^�[�t�F�[�X
extern void input_init();//���̓f�o�C�X�����ݒ�֐�
extern void key_read();//�L�[�{�[�h��ǂ�
extern bool key_on(BYTE key);//�L�[�I������
extern bool key_touch(BYTE key);//�L�[�I������
extern LPDIRECT3DDEVICE9 GRAPHIC_DEV;//DirectX�`��f�o�C�X�C���^�[�t�F�[�X
extern LPDIRECT3DSURFACE9  BACK;// �o�b�N�o�b�t�@�C���^�[�t�F�[�X
extern LPDIRECT3DSURFACE9  FRONT;// �t�����g�o�b�t�@�C���^�[�t�F�[�X
extern void graphic_init();//�O���t�B�b�N�f�o�C�X�����ݒ�֐�
extern void dx_draw(void);//DirectX�Ńs�N�Z���`��
#pragma endregion 