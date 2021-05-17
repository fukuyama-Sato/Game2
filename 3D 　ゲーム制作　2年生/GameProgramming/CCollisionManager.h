#ifndef CCOLLISIONMANAGER_H
#define CCOLLISIONMANAGER_H

#include"CTaskManager.h"
#include"CCollider.h"

#define COLLISIONRANGE 30

class CCollisionManager : public CTaskManager
{
	//�}�l�[�W���̃C���X�^���X
	static CCollisionManager* mpInstance;
public:
	//�C���X�^���X�̎擾
	static CCollisionManager* Get();

	//�Փˏ���
	void Collision();

	//�Փˏ���
	void Collision(CCollider *c, int range);

};

#endif