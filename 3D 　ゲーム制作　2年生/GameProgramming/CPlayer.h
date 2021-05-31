#ifndef CPLAYER_H
#define CPLAYER_H
//�L�����N�^�[�N���X�̃C���N���[�h
#include"CCharacter.h"
//�e�N���X�̃C���N���[�h
#include"CBullet.h"
#include"CColliderLine.h"
/*�v���C���[�N���X
�@�L�����N�^�N���X���p��*/
class CPlayer : public CCharacter{
private:
	int mJump;
	int mHp;	//�̗�
	int mFireRate;
	float mSpeedX;	//���E�����x
	float mSpeedY;	//�㉺�����x
	float mSpeedZ;	//�O������x
	float mBeforMouseX;
	float mBeforMouseY;
	float mMouseMoveX;
	float mMouseMoveY;

public:
	CColliderLine mLine; //�����R���C�_
	CColliderLine mLine2; //�����R���C�_
	CColliderLine mLine3; //�����R���C�_
	CCollider mCollider; //�{�̗p�R���C�_

	CPlayer();
	//�X�V����
	void Update();

	//�Փˏ���
	void Collision(CCollider *m, CCollider *o);

	//�`�揈��
	void Render();

	//�Փˏ���
	void TaskCollision();

};

#endif