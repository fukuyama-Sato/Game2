#ifndef CEFFECT_H
#define CEFFECT_H

#include"CBillBoard.h"

class CEffect :public CBillBoard{
public:
	//�s��
	int mRows;
	//��
	int mCols;
	//1�R�}�̃t���[����
	int mFps;
	//�t���[���J�E���^
	int mFrame;
	//�}�e���A��
	static CMaterial sMaterial;
	//�R���X�g���N�^
	//CEffect(�ʒu,��,����,�e�N�X�`����,�s��,��,1�R�}������̃t���[����)�s��,��,�t���[�����̓f�t�H���g�����ŌĂяo�����ȗ��\
	CEffect(const CVector &pos, float w, float h, char *texture, int row = 1, int col = 1, int fps = 1);
	//�X�V
	void Update();
	//�`��
	void Render();
};

#endif