#ifndef CPLAYER_H
#define CPLAYER_H
//�L�����N�^�[�N���X�̃C���N���[�h
#include"CCharacter.h"
//�e�N���X�̃C���N���[�h
#include"CBullet.h"
#include"CColliderLine.h"
#include"CColliderTriangle.h"
#include"CEffect.h"
#include"CEnemy.h"
/*�v���C���[�N���X
�@�L�����N�^�N���X���p��*/
class CPlayer : public CCharacter{
private:
	bool mJump;
	int mJumpTimer;

	int mFireRate;	//�A�ˑ��x
	int mBulletTortalNum;
	int mBulletNum;
	int mReloadTime;

	float mSpeedX;	//���E�����x
	float mSpeedY;	//�㉺�����x
	float mSpeedZ;	//�O������x
	float mBeforMouseX;	//���O�}�E�X�ʒu
	float mBeforMouseY;
	float mMouseMoveX;	//�}�E�X�ړ���
	float mMouseMoveY;
	float mMouseSpeedX;	//�}�E�X���x
	float mMouseSpeedY;

public:

	int mPlayerHp;	//�̗�

	CColliderLine mLine; //�����R���C�_
	CColliderLine mLine2; //�����R���C�_
	CColliderLine mLine3; //�����R���C�_
	CColliderLine mLine4; //�����R���C�_
	CColliderLine mLine5; //�����R���C�_
	CCollider mCollider; //�{�̗p�R���C�_
	//CColliderTriangle mTriangleCol;

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