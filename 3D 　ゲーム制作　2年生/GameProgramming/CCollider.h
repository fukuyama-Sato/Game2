#ifndef CCLLIDER_H
#define CCLLIDER_H
#include "CCharacter.h"
#include "CTransform.h"
/*�R���C�_�N���X
�Փ˔���f�[�^*/
class CCollider : public CTransform,public CTask{
public:
	CCharacter *mpParent;//�e

	CMatrix *mpMatrix; //�e�s��

	//�R���C�_�^�C�v
	enum EType{
		ESPHERE,//���R���C�_
		ETRIANGLE,//�O�p�R���C�_
		ELINE	//�����R���C�_
	};
	EType mType;// �R���C�_�^�C�v

	enum ETag{
		EBODY,
		ESEARCH,
	};
	ETag mTag;	//�R���C�_����

	//���_
	CVector mV[3];
	//�f�t�H���g�R���X�g���N�^
	CCollider();

	float mRadius; //���a

	~CCollider();

	//�R���X�g���N�^
	//CCollider(�e,�e�s��,�ʒu,���a)
	CCollider(CCharacter *parent, CMatrix *matrix, CVector position, float radius);

	//�Փ˔���
	//Collision(�R���C�_1,�R���C�_2)
	//return:true(�Փ˂��Ă���)false(�Փ˂��Ă��Ȃ�)
	static bool Collision(CCollider *m, CCollider *o);

	//CollisionTriangleLine(�O�p�R���C�_,�����R���C�_,�����l)
	//return : true(�Փ˂��Ă���) false(�Փ˂��Ă��Ȃ�)
	//�����l : �Փ˂��Ȃ��ʒu�܂Ŗ߂��l
	static bool CollisionTriangleLine(CCollider *triangle, CCollider *line, CVector *adjust);

	//CollisionTriangleSphere(�O�p�R���C�_,���R���C�_,�����l)
	//return:true(�Փ˂��Ă���),falsse(�Փ˂��Ă��Ȃ�)
	//�����l:�Փ˂��Ȃ��ʒu�܂Ŗ߂��l
	static bool CollisionTriangleSphere(CCollider *triangle, CCollider *sphere, CVector *adjust);

	//�D��x�̕ύX
	void ChangePriority();

	//�`��
	void Render();
};

#endif