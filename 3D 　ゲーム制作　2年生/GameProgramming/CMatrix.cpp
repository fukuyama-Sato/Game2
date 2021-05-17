#include"CMatrix.h"
//�W�����o�͊֐��̃C���N���[�h
#include<stdio.h>
//�~����M_PI��L���ɂ���
#define _USE_MATH_DEFINES
//���w�֐��̃C���N���[�h
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

//�f�t�H���g�R���X�g���N�^
CMatrix::CMatrix(){
	Identity();
}

//�P�ʍs��̍쐬
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
	//���̍s���Ԃ�
	return *this;
}

//�g��k���s��̍쐬
//Scale(�{��X,�{��Y,�{��Z)
CMatrix CMatrix::Scale(float sx, float sy, float sz){
	Identity();
		mM[0][0] = sx;
		mM[1][1] = sy;
		mM[2][2] = sz;
		//���̍s���Ԃ�
		return *this;
}

//��]�s��(Y��)�̍쐬
//RotateY(�p�x)
CMatrix CMatrix::RotateY(float degree){
	//�p�x���烉�W�A�������߂�
	float rad = degree / 180.0f * M_PI;
	//�P�ʍs��ɂ���
	Identity();
	//Y���ŉ�]����s��̐ݒ�
	mM[0][0] = mM[2][2] = cosf(rad);
	mM[0][2] = -sinf(rad);
	mM[2][0] = -mM[0][2];
	//�s���Ԃ�
	return *this;
}

//��]�s��(Z��)�̍쐬
//RotateZ(�p�x)
CMatrix CMatrix::RotateZ(float degree){
	//�p�x���烉�W�A�������߂�
	float rad = degree / 180.0f * M_PI;
	//�P�ʍs��ɂ���
	Identity();
	//Z���ŉ�]����s��̐ݒ�
	mM[0][0] = mM[1][1] = cosf(rad);
	mM[0][1] = sinf(rad);
	mM[1][0] = -mM[0][1];
	//�s���Ԃ�
	return *this;
}

//��]�s��(X��)�̍쐬
//RotateX(�p�x)
CMatrix CMatrix::RotateX(float degree){
	//�p�x���烉�W�A�������߂�
	float rad = degree / 180.0f * M_PI;
	//�P�ʍs��ɂ���
	Identity();
	//X���ŉ�]����s��̐ݒ�
	mM[1][1] = mM[2][2] = cosf(rad);
	mM[1][2] = sinf(rad);
	mM[2][1] = -mM[1][2];
	//�s���Ԃ�
	return *this;
}

//�ړ��s��̍쐬
//Translate(�ړ���X,�ړ���Y,�ړ���Z)
CMatrix CMatrix::Translate(float mx, float my, float mz){
	Identity();
	mM[3][0] = mx;
	mM[3][1] = my;
	mM[3][2] = mz;
	//���̍s���Ԃ�
	return *this;
}

//*���Z�q�̃I�[�o�[���[�h
//CMatrix * CMatrix �̉��Z�q��Ԃ�
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