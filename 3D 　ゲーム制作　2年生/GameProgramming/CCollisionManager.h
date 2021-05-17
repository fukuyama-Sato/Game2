#ifndef CCOLLISIONMANAGER_H
#define CCOLLISIONMANAGER_H

#include"CTaskManager.h"
#include"CCollider.h"

#define COLLISIONRANGE 30

class CCollisionManager : public CTaskManager
{
	//マネージャのインスタンス
	static CCollisionManager* mpInstance;
public:
	//インスタンスの取得
	static CCollisionManager* Get();

	//衝突処理
	void Collision();

	//衝突処理
	void Collision(CCollider *c, int range);

};

#endif