#include"CEnemy.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CEffect.h"
#include"CBullet.h"

#define OBJ "f16.obj"	//���f���̃t�@�C��
#define MTL "f16.mtl"	//���f���̃}�e���A���t�@�C��
#define HP 300	//�ϋv�l
#define VELOCITYX 0.2f	//X���x
#define VELOCITYY 0.2f	//Y���x
#define VELOCITYZ 0.2f	//Z���x

CModel CEnemy::mModel;	//���f���f�[�^�쐬

//�f�t�H���g�R���X�g���N�^
CEnemy::CEnemy()
:mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)
, mColSearch1(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 70.0f)
, mpPlayer(0)
, mHp(HP)
, mMoveSpeedX(0)
, mMoveSpeedY(0)
, mMoveSpeedZ(0)
{
	//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0){
		mModel.Load(OBJ, MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	mColSearch1.mTag = CCollider::ESEARCH;
	//�ڕW�n�_�̐ݒ�
	mPoint = mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
}

//CEnemy(�ʒu,��],�g��)
CEnemy::CEnemy(const CVector& position, const CVector& rotation, const CVector& scale)
: CEnemy()
{
	//�ʒu,��],�g�k��ݒ�
	mPosition = position;	//�ʒu
	mRotation = rotation;	//��]
	mScale = scale;			//�g�k
	CTransform::Update();	//�s��
	//�D��x��1
	mPriority = 1;
	CTaskManager::Get()->Remove(this);	//�폜
	CTaskManager::Get()->Add(this);		//�ǉ�

	mMoveControl = 0;
}

void CEnemy::MoveMane(){
	//������(X��)�̃x�N�g�������߂�
	vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
	//�����(Y��)�̃x�N�g�������߂�
	vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
	//�O����(Z��)�̃x�N�g�������߂�
	vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;

	CVector vp = mpPlayer->mPosition - mPosition;
	dx = vp.Dot(vx);	//���x�N�g���Ƃ̓��ς����߂�
	dy = vp.Dot(vy);	//��x�N�g���Ƃ̓��ς����߂�
	dz = vp.Dot(vz);	//�O�x�N�g���Ƃ̓��ς����߂�

	//X���̂��ꂪ10.0�ȉ�
	if (-50.0f > dx && dx > 50.0f){
		//Y���̂��ꂪ5.0�ȉ�
		if (-30.0f > dy && dy > 30.0f){
			if (-50.0f > dz && dz > 50.0f){
				mMoveControl = 2;
			}
		}
	}

	//X���̂��ꂪ10.0�ȏ�,50�ȉ�
	else if (-10.0f > dx && dx > 10.0f && -50.0f < dx && dx < 50.0f){
		//Y���̂��ꂪ10�ȏ�,30�ȉ�
		if (-10.0f > dy && dy > 10.0f && -30.0f < dy && dy < 30.0f){
			if (-10.0f > dz && dz > 10.0f && -50.0f < dz && dz < 50.0f){
				mMoveControl = 1;
			}
		}
	}

	//X���̂��ꂪ10.0�ȉ�
	else if (-10.0f < dx && dx < 10.0f){
		//Y���̂��ꂪ5.0�ȉ�
		if (-10.0f < dy && dy < 10.0f){
			if (-10.0f < dz && dz < 10.0f){
				mMoveControl = 0;
			}
		}
	}

}

//�ߋ���
void CEnemy::NearMove(){
	if (mMoveControl == 0){
		mMoveSpeedZ++;
	}
}

//������
void CEnemy::MediumMove(){
	if (mMoveControl == 1){
		mMoveSpeedY++;
	}
}

//������
void CEnemy::DistantMove(){
	if (mMoveControl == 2){
		mMoveSpeedX++;
	}
}


void CEnemy::Update(){

	//���悻3�b���ɖڕW�n�_���X�V
	int r = rand() % 180;	//rand()�͐����̗�����Ԃ�
	//%180 �� 180�Ŋ������]������߂�
	if (r == 0){
		if (mpPlayer){
			mPoint = mpPlayer->mPosition;
		}
		else{
			mPoint = mPoint * CMatrix().RotateY(45);
		}
	}

	if (-10.0f < dx && dx < 10.0f){
		if (-5.0f < dy && dy < 5.0f){
			if (dz > 0.0f){
				//�e�𔭎˂���
				CBullet *bullet = new CBullet();
				bullet->Set(0.1f, 1.5f);
				bullet->mPosition = CVector(0.0f, 0.0f, 10.0f) * mMatrix;
				bullet->mRotation = mRotation;
				bullet->Update();
			}
		}
	}

	mpPlayer = 0;
	CVector vp = mPoint - mPosition;
	float dx = vp.Dot(vx); //���x�N�g���Ƃ̓���
	float dy = vp.Dot(vy); //��x�N�g���Ƃ̓���
	float margin = 1.0f;

		//���E�����։�]
		if (dx > margin){
			mRotation.mY += 4.0f;
		}
		else if (dx < -margin){
			mRotation.mY -= 4.0f;
		}

		//�㉺�����։�]
		if (dy > margin){
			mRotation.mX -= 2.0f;
		}
		else if (dy < -margin){
			mRotation.mX += 2.0f;
		}

	//�ړ�
	mPosition = mPosition + CVector(mMoveSpeedX, mMoveSpeedY, mMoveSpeedZ) * mMatrixRotate;
	CTransform::Update();	//�s��X�V
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CEnemy::Collision(CCollider *m, CCollider *o){
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
		if (o->mpParent->mTag == EBULLETPLAYER){
		//�R���C�_��m��o���Փ˂��Ă��邩����
			if (CCollider::Collision(m, o)){
				mHp--; //HP���Z
				//�G�t�F�N�g����
				new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			}
		}
		break;
	case CCollider::ETRIANGLE: //�O�p�R���C�_
		CVector adjust; //�����l
		//�O�p�R���C�_�Ƌ��R���C�_�̔���
		if (CCollider::CollisionTriangleSphere(o, m, &adjust)){
			//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
			mPosition = mPosition + adjust;
		}
		break;
	}

}



void CEnemy::TaskCollision(){
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	mColSearch1.ChangePriority();
	//�Փˏ��� ���s
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSearch1, COLLISIONRANGE);
}