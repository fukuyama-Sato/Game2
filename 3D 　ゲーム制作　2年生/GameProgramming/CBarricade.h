#ifndef CBARRICADE_H
#define CBARRICADE_H

#include "CCharacter.h"
#include"CColliderTriangle.h"

class CBarricade : public CCharacter{
public:
	//�R���C�_
	CColliderTriangle mCollider1a;
	CColliderTriangle mCollider1b;

	CColliderTriangle mCollider2a;
	CColliderTriangle mCollider2b;

	static CModel mModel;

	//�v���C���[�̃|�C���^
	CCharacter *mpPlayer;

	CBarricade(CModel *model, CVector position, CVector rotation, CVector scale);

	void Update();

	//�Փˏ���
	//Collider(�R���C�_1,�R���C�_2)
	void Collision(CCollider *m, CCollider *o);

	void TaskCollision();
};

#endif