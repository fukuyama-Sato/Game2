#ifndef CINPUT_H
#define CINPUT_H
#include"GLFW/glfw3.h"
class CInput{
	//ウィンドウポインタ
	static GLFWwindow* mpWindow;

public:
	/*
	Init(ウィンドウポインタ)*/
	static void Init(GLFWwindow* w);
	/*
	GetMousePos(X座標ポインタ,Y座標ポインタ)
	マウスカーソルのウィンドウ上の座標を引数に設定する
	ウィンドウ上の座標の原点は左上*/
	static void GetMousePos(float *px, float *py);
};

#endif