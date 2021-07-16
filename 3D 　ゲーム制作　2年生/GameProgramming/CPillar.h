#ifndef CPILLAR_H
#define CPILLAR_H

#include "CCharacter.h"
#include"CColliderTriangle.h"
#include"CModel.h"

class CPillar : public CCharacter{
public:
	//�R���C�_
	CColliderTriangle mCollider1a;
	CColliderTriangle mCollider1b;

	CColliderTriangle mCollider2a;
	CColliderTriangle mCollider2b;

	CColliderTriangle mCollider3a;
	CColliderTriangle mCollider3b;

	CColliderTriangle mCollider4a;
	CColliderTriangle mCollider4b;

	static CModel mModel;

	//�v���C���[�̃|�C���^
	CCharacter *mpPlayer;

	CPillar(CModel *model, CVector position, CVector rotation, CVector scale);

	void Update();

	//�Փˏ���
	//Collider(�R���C�_1,�R���C�_2)
	void Collision(CCollider *m, CCollider *o);

	void TaskCollision();
};

#endif