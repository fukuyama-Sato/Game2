#ifndef CEnemy_H
#define CEnemy_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter.h"
//�R���C�_�N���X�̃C���N���[�h
#include"CCollider.h"
#include"CVector.h"
/*�G�l�~�[�N���X
�@�L�����N�^�N���X���p��*/
class CEnemy : public CCharacter{
public:

	int mHp;	//�q�b�g�|�C���g
	//���f���f�[�^
	static CModel mModel;
	//�ڕW�n�_
	CVector mPoint;
	//�R���C�_
	CCollider mCollider;
	//�T�[�`�p�R���C�_ 
	CCollider mColSearch1;
	CCollider mColSearch2;
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