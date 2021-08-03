#include"CBullet.h"
#include"CCollisionManager.h"

//�f�t�H���g�R���X�g���N�^
CBullet::CBullet()
:mLife(50)
,mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 1.1f)
{}

//���Ɖ��s���̐ݒ�
//Set(��,���s��)
void CBullet::Set(float w, float d){
	//�X�P�[���ݒ�
	mScale = CVector(1.0f, 1.0f, 1.0f);
	//�O�p�`�̒��_�ݒ�
	mT.SetVertex(CVector(-w, 0.5, 0.0),CVector(w, 0.5, 0.0),CVector(0.0, 0.0, d));
	//�O�p�`�̖@���ݒ�
	mT.SetNormal(CVector(0.0f, 1.0f, 0.0f));

	//�O�p�`�̒��_�ݒ�
	mT2.SetVertex(CVector(0.5, w, 0.0), CVector(0.5, -w, 0.0), CVector(0.0, 0.0, d));
	//�O�p�`�̖@���ݒ�
	mT2.SetNormal(CVector(1.0f, 0.0f, 0.0f));

	mT3.SetVertex(CVector(-0.5, -w, 0.0), CVector(-0.5, w, 0.0), CVector(0.0, 0.0, d));
	//�O�p�`�̖@���ݒ�
	mT3.SetNormal(CVector(-1.0f, 0.0f, 0.0f));

	mT4.SetVertex(CVector(w, -0.5, 0.0), CVector(-w, -0.5, 0.0), CVector(0.0, 0.0, d));
	//�O�p�`�̖@���ݒ�
	mT4.SetNormal(CVector(0.0f, -1.0f, 0.0f));
}

//�X�V
void CBullet::Update(){
	//�������Ԃ̔���
	if (mLife-- > 0){
		CTransform::Update();
		//�ʒu�X�V
		mPosition = CVector(0.0f, 0.0f, 4.5f) * mMatrix;
	}
	else{
		//�����ɂ���
		mEnabled = false;
	}
}

//�`��
void CBullet::Render(){
	//DIFFUSE���F�ݒ�
	float c[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//�O�p�`�`��
	mT.Render(mMatrix);
	mT2.Render(mMatrix);
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CBullet::Collision(CCollider *m, CCollider *o){
	//���肪�T�[�`�̎��͖߂�
	if (o->mTag == CCollider::ESEARCH){
		return;
	}

	//�R���C�_��m��o���Փ˂��Ă��邩����
	if (CCollider::Collision(m, o)){
		//�Փ˂����疳��
		mEnabled = false;
	}

	if (m->mType == CCollider::ESPHERE && o->mType == CCollider::ETRIANGLE){
		if (o->mpParent && o->mpParent->mTag == EBLOCK){
			CVector dummy;
			if (CCollider::CollisionTriangleSphere(o, m, &dummy)){
				//�Փ˂����疳��
				mEnabled = false;
			}
		}
	}

}

//�Փˏ���
void CBullet::TaskCollision(){
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	
	//�Փˏ��� ���s
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}