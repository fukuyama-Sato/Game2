#ifndef CINPUT_H
#define CINPUT_H
#include"GLFW/glfw3.h"
class CInput{
	//�E�B���h�E�|�C���^
	static GLFWwindow* mpWindow;

public:
	/*
	Init(�E�B���h�E�|�C���^)*/
	static void Init(GLFWwindow* w);
	/*
	GetMousePos(X���W�|�C���^,Y���W�|�C���^)
	�}�E�X�J�[�\���̃E�B���h�E��̍��W�������ɐݒ肷��
	�E�B���h�E��̍��W�̌��_�͍���*/
	static void GetMousePos(float *px, float *py);
};

#endif