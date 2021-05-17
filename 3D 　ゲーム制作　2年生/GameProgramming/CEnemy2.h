#ifndef CENEMY2_H
#define CENEMY2_H
//�L�����N�^�N���X�̃C���N���[�h
#include"CCharacter.h"
//�R���C�_�N���X�̃C���N���[�h
#include"CCollider.h"
#include"CVector.h"
/*�G�l�~�[�N���X
�@�L�����N�^�N���X���p��*/
class CEnemy2 : public CCharacter{
public:

	int mHp;	//�q�b�g�|�C���g
	//���f���f�[�^
	static CModel mModel;
	//�ڕW�n�_
	CVector mPoint;
	//�R���C�_
	CCollider mCollider;
	//�T�[�`�p�R���C�_ 
	CCollider mColSearch;
	//�v���C���[�̃|�C���^
	CCharacter *mpPlayer;
	//�R���X�g���N�^
	CEnemy2();
	//CEnemy(�ʒu,��],�g��)
	CEnemy2(const CVector& position, const CVector& rotation,
		const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collider(�R���C�_1,�R���C�_2)
	void Collision(CCollider *m, CCollider *o);
	void TaskCollision();
};

#endif