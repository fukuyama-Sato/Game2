#ifndef CSPHERE_H
#define CSPHERE_H

#include "CCharacter.h"
#include"CCollider.h"

class CSphere : public CCharacter{
public:
	//コライダ
	CCollider mCollider;
	//プレイヤーのポインタ
	CCharacter *mpPlayer;

	CSphere(CModel *model, CVector position, CVector rotation, CVector scale);

	void Update();

	//衝突処理
	//Collider(コライダ1,コライダ2)
	void Collision(CCollider *m, CCollider *o);

	void TaskCollision();
};
#endif