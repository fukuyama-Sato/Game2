#ifndef CSCENEGAME_H
#define CSCENEGAME_H
#include"CScene.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CPlayer.h"
#include"CColliderTriangle.h"

/*
�Q�[���̃V�[��
*/
class CSceneGame : public CScene {
private:
	int camX;
	int camY;
	int camZ;

public:
	//���f���N���X�̃C���X�^���X�쐬
	CModel mModel;

	CPlayer mPlayer;

	//�����������̃I�[�o�[���C�h
	void Init();
	//�X�V�����̃I�[�o�[���C�h
	void Update();
};

#endif
