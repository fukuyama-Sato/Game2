#ifndef CSPHERE_H
#define CSPHERE_H

#include "CCharacter.h"
#include"CCollider.h"

class CSphere : public CCharacter{
public:
	//�R���C�_
	CCollider mCollider;
	//�v���C���[�̃|�C���^
	CCharacter *mpPlayer;

	CSphere();

	CSphere(CModel *model, CVector position, CVector rotation, CVector scale);

	void Update();

	//�Փˏ���
	//Collider(�R���C�_1,�R���C�_2)
	void Collision(CCollider *m, CCollider *o);

	void TaskCollision();
};
#endif