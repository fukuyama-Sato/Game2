#ifndef CMATRIX_H
#define CMATRIX_H

/*�}�g���N�X�N���X
�@4�s4��̍s��f�[�^�������܂�*/
class CMatrix{
public:
	 //4�~4�̍s��f�[�^��ݒ�
	float mM[4][4];
	 //�\���m�F
	 //4�~4�̍s�����ʏo��
	void Print();
	 //�f�t�H���g�R���X�g���N�^
	CMatrix();
	 //�P�ʍs��̍쐬
	CMatrix Identity();
	 //�g��k���s��̍쐬
	 //Scale(�{��X,�{��Y,�{��Z)
	CMatrix Scale(float sx, float sy, float sz);

	 //��]�s��(Y��,Z��,X��)�̍쐬
	 //RotateY(�p�x)
	CMatrix RotateY(float degree);
	 //RotateZ(�p�x)
	CMatrix RotateZ(float degree);
	 //RotateX(�p�x)
	CMatrix RotateX(float degree);

	 //�ړ��s��̍쐬
	 //Translate(�ړ���X,�ړ���Y,�ړ���Z)
	CMatrix Translate(float mx, float my, float mz);

	//*���Z�q�̃I�[�o�[���[�h
	//CMatrix * CMatrix �̉��Z�q��Ԃ�
	CMatrix operator*(const CMatrix &m);

};
#endif