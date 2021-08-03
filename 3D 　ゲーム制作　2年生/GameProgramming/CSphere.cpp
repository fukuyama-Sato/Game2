#include"CSphere.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CEffect.h"

CSphere::CSphere()
:mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 10.0f)
{
	mTag = EBLOCK;
}

CSphere::CSphere(CModel *model, CVector position, CVector rotation, CVector scale)
: CSphere()
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

void CSphere::Update(){
	CTransform::Update();
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CSphere::Collision(CCollider *m, CCollider *o){
	//���肪�T�[�`�̎��͖߂�
	if (o->mTag == CCollider::ESEARCH){
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

void CSphere::TaskCollision(){
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	//�Փˏ��� ���s
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}