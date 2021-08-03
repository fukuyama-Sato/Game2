#include"CBarricade.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CEffect.h"

CModel CBarricade::mModel;

CBarricade::CBarricade(CModel *model, CVector position, CVector rotation, CVector scale)
:mCollider1a(this, &mMatrix,
CVector(10.0f, 0.0f, 1.5f),
CVector(9.0f, 7.0f, 0.8f),
CVector(-9.0f, 7.0f, 0.8f))
, mCollider1b(this, &mMatrix,
CVector(-9.0f, 7.0f, 0.8f),
CVector(-10.0f, 0.0f, 1.5f),
CVector(10.0f, 0.0f, 1.5f))

, mCollider2a(this, &mMatrix,
CVector(-9.0f, 7.0f, -0.8f),
CVector(9.0f, 7.0f, -0.8f),
CVector(10.0f, 0.0f, -1.5f))

, mCollider2b(this, &mMatrix,
CVector(10.0f, 0.0f, -1.5f),
CVector(-10.0f, 0.0f, -1.5f),
CVector(-9.0f, 7.0f, -0.8f))

{
	//���f��,�ʒu,��],�g�k��ݒ�
	mpModel = model;		//���f���̐ݒ�
	mPosition = position;	//�ʒu�̐ݒ�
	mRotation = rotation;	//��]�̐ݒ�
	mScale = scale;			//�g�k�̐ݒ�

	mTag = EBLOCK;

	mPriority = 1; //�D��x1
	CTaskManager::Get()->Remove(this);
	CTaskManager::Get()->Add(this);

}

void CBarricade::Update(){
	CTransform::Update();
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CBarricade::Collision(CCollider *m, CCollider *o){
	//���肪�T�[�`�̎��͖߂�
	if (o->mTag == CCollider::ESEARCH){
		return;
	}


	if (o->mEnabled == CCollider::ESPHERE){ //���R���C�_
		if (o->mTag == CCharacter::EBULLETPLAYER || o->mTag == CCharacter::EBULLETENEMY){
			//�R���C�_��m��o���Փ˂��Ă��邩����
			if (CCollider::Collision(m, o)){
				//�G�t�F�N�g����
				new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			}
		}
	}

}

void CBarricade::TaskCollision(){
	//�R���C�_�̗D��x�ύX
	mCollider1a.ChangePriority();
	mCollider1b.ChangePriority();

	mCollider2a.ChangePriority();
	mCollider2b.ChangePriority();
	//�Փˏ��� ���s
	CCollisionManager::Get()->Collision(&mCollider1a, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider1b, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider2a, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider2b, COLLISIONRANGE);
}