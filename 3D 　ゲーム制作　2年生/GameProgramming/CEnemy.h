#ifndef CEnemy_H
#define CEnemy_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter.h"
//�R���C�_�N���X�̃C���N���[�h
#include"CCollider.h"
#include"CVector.h"
#include"CColliderLine.h"
/*�G�l�~�[�N���X
�@�L�����N�^�N���X���p��*/
class CEnemy : public CCharacter{
private:
	int mEFireRate;	//�A�ˑ��x
	
	int mEReloadTime;

	bool mEJump;

	int mEspeedX;	//���x
	int mEspeedY;
	int mEspeedZ;
	float mMoveSpeedX;	//�ړ��p
	float mMoveSpeedY;
	float mMoveSpeedZ;

	int mMoveControl;	//�v���C���[�Ƃ̋����̔��f

public:

	static int mHp;	//�q�b�g�|�C���g
	static int mEBulletTortalNum;
	static int mEBulletNum;
	//���f���f�[�^
	static CModel mModel;
	//�ڕW�n�_
	CVector mPoint;

	CColliderLine mELine; //�����R���C�_
	CColliderLine mELine2; //�����R���C�_
	CColliderLine mELine3; //�����R���C�_
	//�R���C�_
	CCollider mCollider;
	//�T�[�`�p�R���C�_ 
	CCollider mColSearch1;
	//�v���C���[�̃|�C���^
	CCharacter *mpPlayer;
	//�R���X�g���N�^
	CEnemy();
	//CEnemy(�ʒu,��],�g��)
	CEnemy(const CVector& position, const CVector& rotation,
		const CVector& scale);

	//�X�V����
	void Update();

	//�Փˏ���
	//Collider(�R���C�_1,�R���C�_2)
	void Collision(CCollider *m, CCollider *o);
	void TaskCollision();
};

#endif