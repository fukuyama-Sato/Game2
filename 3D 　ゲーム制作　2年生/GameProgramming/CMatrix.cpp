#include"CMatrix.h"
//標準入出力関数のインクルード
#include<stdio.h>
//円周率M_PIを有効にする
#define _USE_MATH_DEFINES
//数学関数のインクルード
#include<math.h>

void CMatrix::Print(){
	printf("%10f%10f%10f%10f\n",
		mM[0][0], mM[0][1], mM[0][2], mM[0][3]);
	printf("%10f%10f%10f%10f\n",
		mM[1][0], mM[1][1], mM[1][2], mM[1][3]);
	printf("%10f%10f%10f%10f\n",
		mM[2][0], mM[2][1], mM[2][2], mM[2][3]);
	printf("%10f%10f%10f%10f\n",
		mM[3][0], mM[3][1], mM[3][2], mM[3][3]);
}

//デフォルトコンストラクタ
CMatrix::CMatrix(){
	Identity();
}

//単位行列の作成
CMatrix CMatrix::Identity(){
	for (int n = 0; n < 4; n++){
		for (int m = 0; m < 4; m++){
			if (n == m){
				mM[n][m] = 1;
			}
			else{ 
				mM[n][m] = 0; 
			}
		}
	}
	//この行列を返す
	return *this;
}

//拡大縮小行列の作成
//Scale(倍率X,倍率Y,倍率Z)
CMatrix CMatrix::Scale(float sx, float sy, float sz){
	Identity();
		mM[0][0] = sx;
		mM[1][1] = sy;
		mM[2][2] = sz;
		//この行列を返す
		return *this;
}

//回転行列(Y軸)の作成
//RotateY(角度)
CMatrix CMatrix::RotateY(float degree){
	//角度からラジアンを求める
	float rad = degree / 180.0f * M_PI;
	//単位行列にする
	Identity();
	//Y軸で回転する行列の設定
	mM[0][0] = mM[2][2] = cosf(rad);
	mM[0][2] = -sinf(rad);
	mM[2][0] = -mM[0][2];
	//行列を返す
	return *this;
}

//回転行列(Z軸)の作成
//RotateZ(角度)
CMatrix CMatrix::RotateZ(float degree){
	//角度からラジアンを求める
	float rad = degree / 180.0f * M_PI;
	//単位行列にする
	Identity();
	//Z軸で回転する行列の設定
	mM[0][0] = mM[1][1] = cosf(rad);
	mM[0][1] = sinf(rad);
	mM[1][0] = -mM[0][1];
	//行列を返す
	return *this;
}

//回転行列(X軸)の作成
//RotateX(角度)
CMatrix CMatrix::RotateX(float degree){
	//角度からラジアンを求める
	float rad = degree / 180.0f * M_PI;
	//単位行列にする
	Identity();
	//X軸で回転する行列の設定
	mM[1][1] = mM[2][2] = cosf(rad);
	mM[1][2] = sinf(rad);
	mM[2][1] = -mM[1][2];
	//行列を返す
	return *this;
}

//移動行列の作成
//Translate(移動量X,移動量Y,移動量Z)
CMatrix CMatrix::Translate(float mx, float my, float mz){
	Identity();
	mM[3][0] = mx;
	mM[3][1] = my;
	mM[3][2] = mz;
	//この行列を返す
	return *this;
}

//*演算子のオーバーロード
//CMatrix * CMatrix の演算子を返す
CMatrix CMatrix::operator*(const CMatrix &m){

/*	t.mM[0][0] = mM[0][0] * m.mM[0][0] + mM[0][1] * m.mM[1][0] + mM[0][2] * m.mM[2][0] + mM[0][3] * m.mM[3][0];
	t.mM[0][1] = mM[0][0] * m.mM[0][1] + mM[0][1] * m.mM[1][1] + mM[0][2] * m.mM[2][1] + mM[0][3] * m.mM[3][1];
	t.mM[0][2] = mM[0][0] * m.mM[0][2] + mM[0][1] * m.mM[1][2] + mM[0][2] * m.mM[2][2] + mM[0][3] * m.mM[3][2];
	t.mM[0][3] = mM[0][0] * m.mM[0][3] + mM[0][1] * m.mM[1][3] + mM[0][2] * m.mM[2][3] + mM[0][3] * m.mM[3][3];
	.
	.
	.	*/

	CMatrix t;
	for (int n = 0; n < 4; n++){
		for (int h = 0; h < 4; h++){
			t.mM[n][h] = mM[n][0] * m.mM[0][h] + mM[n][1] * m.mM[1][h] + mM[n][2] * m.mM[2][h] + mM[n][3] * m.mM[3][h];
		}
	}
	return t;
}