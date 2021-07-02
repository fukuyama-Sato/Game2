#include "CPillar.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CEffect.h"

CPillar::CPillar()
:mCollider1a(this, &mMatrix,
CVector(0.0f, -4.0f, 10.0f),
CVector(5.0f, -6.0f, 0.0f),
CVector(-5.0f, -6.0f, 0.0f))
, mCollider1b(this, &mMatrix,
CVector(0.0f, -4.0f, 10.0f),
CVector(5.0f, -6.0f, 0.0f),
CVector(-5.0f, -6.0f, 0.0f))

, mCollider2a(this, &mMatrix,
CVector(0.0f, -4.0f, 10.0f),
CVector(5.0f, -6.0f, 0.0f),
CVector(-5.0f, -6.0f, 0.0f))
, mCollider2b(this, &mMatrix,
CVector(0.0f, -4.0f, 10.0f),
CVector(5.0f, -6.0f, 0.0f),
CVector(-5.0f, -6.0f, 0.0f))

, mCollider3a(this, &mMatrix,
CVector(0.0f, -4.0f, 10.0f),
CVector(5.0f, -6.0f, 0.0f),
CVector(-5.0f, -6.0f, 0.0f))
, mCollider3b(this, &mMatrix,
CVector(0.0f, -4.0f, 10.0f),
CVector(5.0f, -6.0f, 0.0f),
CVector(-5.0f, -6.0f, 0.0f))

, mCollider4a(this, &mMatrix,
CVector(0.0f, -4.0f, 10.0f),
CVector(5.0f, -6.0f, 0.0f),
CVector(-5.0f, -6.0f, 0.0f))
, mCollider4b(this, &mMatrix,
CVector(0.0f, -4.0f, 10.0f),
CVector(5.0f, -6.0f, 0.0f),
CVector(-5.0f, -6.0f, 0.0f))
{
	mTag = EBLOCK;
}

CPillar::CPillar(CModel *model, CVector position, CVector rotation, CVector scale)
: CPillar()
{
	//���f��,�ʒu,��],�g�k��ݒ�
	mpModel = model;		//���f���̐ݒ�
	mPosition = position;	//�ʒu�̐ݒ�
	mRotation = rotation;	//��]�̐ݒ�
	mScale = scale;			//�g�k�̐ݒ�
	CTransform::Update();

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

	//�������T�[�`�p�̎�
	if (m->mTag == CCollider::ESEARCH){
		//���肪���R���C�_
		if (o->mType == CCollider::ESPHERE){
			if (o->mpParent->mTag == EPLAYER){
				//�Փ˒�
				if (CCollider::Collision(m, o)){
					//�v���C���[�̃|�C���^�ݒ�
					mpPlayer = o->mpParent;
				}
			}
		}
		return;
	}

	//����̃R���C�_�^�C�v�̔���
	switch (o->mType){
	case CCollider::ESPHERE: //���R���C�_
		//�R���C�_��m��o���Փ˂��Ă��邩����
		if (CCollider::Collision(m, o)){
			//�G�t�F�N�g����
			new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		}
		break;
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