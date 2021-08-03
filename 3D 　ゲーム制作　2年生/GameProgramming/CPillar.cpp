#include "CPillar.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CEffect.h"

CModel CPillar::mModel;

CPillar::CPillar(CModel *model, CVector position, CVector rotation, CVector scale)
:mCollider1a(this, &mMatrix,
CVector(-2.0f, 0.0f, 2.0f),
CVector(2.0f, 0.0f, 2.0f),
CVector(-2.0f, 20.0f, 2.0f))
, mCollider1b(this, &mMatrix,
CVector(-2.0f, 20.0f, 2.0f),
CVector(2.0f, 0.0f, 2.0f),
CVector(2.0f, 20.0f, 2.0f))

, mCollider2a(this, &mMatrix,
CVector(2.0f, 0.0f, -2.0f),
CVector(-2.0f, 0.0f, -2.0f),
CVector(-2.0f, 20.0f, -2.0f))
, mCollider2b(this, &mMatrix,
CVector(-2.0f, 20.0f, -2.0f),
CVector(2.0f, 20.0f, -2.0f),
CVector(2.0f, 0.0f, -2.0f))

, mCollider3a(this, &mMatrix,
CVector(2.0f, 0.0f, 2.0f),
CVector(2.0f, 0.0f, -2.0f),
CVector(2.0f, 20.0f, 2.0f))
, mCollider3b(this, &mMatrix,
CVector(2.0f, 20.0f, 2.0f),
CVector(2.0f, 0.0f, -2.0f),
CVector(2.0f, 20.0f, -2.0f))

, mCollider4a(this, &mMatrix,
CVector(-2.0f, 0.0f, -2.0f),
CVector(-2.0f, 0.0f, 2.0f),
CVector(-2.0f, 20.0f, 2.0f))
, mCollider4b(this, &mMatrix,
CVector(-2.0f, 20.0f, 2.0f),
CVector(-2.0f, 20.0f, -2.0f),
CVector(-2.0f, 0.0f, -2.0f))
{
	//���f��,�ʒu,��],�g�k��ݒ�
	mpModel = model;		//���f���̐ݒ�
	mPosition = position;	//�ʒu�̐ݒ�
	mRotation = rotation;	//��]�̐ݒ�
	mScale = scale;			//�g�k�̐ݒ�
	CTransform::Update();

	mTag = EBLOCK;

	mPriority = 1; //�D��x1
	CTaskManager::Get()->Remove(this);
	CTaskManager::Get()->Add(this);
}

void CPillar::Update(){
	CTransform::Update();
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CPillar::Collision(CCollider *m, CCollider *o){
	//���肪�T�[�`�̎��͖߂�
	if (o->mTag == CCollider::ESEARCH){
		return;
	}

	if (o->mTag == CCollider::ESPHERE){
		if (o->mTag == CCharacter::EBULLETPLAYER || o->mTag == CCharacter::EBULLETENEMY){
			if (CCollider::Collision(m, o)){
				//�G�t�F�N�g����
				new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			}
		}
	}
}

void CPillar::TaskCollision(){
	//�R���C�_�̗D��x�ύX
	mCollider1a.ChangePriority();
	mCollider1b.ChangePriority();

	mCollider2a.ChangePriority();
	mCollider2b.ChangePriority();

	mCollider3a.ChangePriority();
	mCollider3b.ChangePriority();

	mCollider4a.ChangePriority();
	mCollider4b.ChangePriority();

	//�Փˏ��� ���s
	CCollisionManager::Get()->Collision(&mCollider1a, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider1b, COLLISIONRANGE);

	CCollisionManager::Get()->Collision(&mCollider2a, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider2b, COLLISIONRANGE);

	CCollisionManager::Get()->Collision(&mCollider3a, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider3b, COLLISIONRANGE);

	CCollisionManager::Get()->Collision(&mCollider4a, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider4b, COLLISIONRANGE);
}