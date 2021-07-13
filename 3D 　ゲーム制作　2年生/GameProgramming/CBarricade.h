#ifndef CBARRICADE_H
#define CBARRICADE_H

#include "CCharacter.h"
#include"CColliderTriangle.h"

class CBarricade : public CCharacter{
public:
	//コライダ
	CColliderTriangle mCollider1a;
	CColliderTriangle mCollider1b;

	CColliderTriangle mCollider2a;
	CColliderTriangle mCollider2b;

	static CModel mModel;

	//プレイヤーのポインタ
	CCharacter *mpPlayer;

	CBarricade(CModel *model, CVector position, CVector rotation, CVector scale);

	void Update();

	//衝突処理
	//Collider(コライダ1,コライダ2)
	void Collision(CCollider *m, CCollider *o);

	void TaskCollision();
};

#endif