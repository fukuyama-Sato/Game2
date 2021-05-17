#include"CEnemy.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CEffect.h"
#include"CBullet.h"

#define OBJ "f16.obj"	//���f���̃t�@�C��
#define MTL "f16.mtl"	//���f���̃}�e���A���t�@�C��
#define HP 3	//�ϋv�l
#define VELOCITY 0.11f	//���x

CModel CEnemy::mModel;	//���f���f�[�^�쐬

//�f�t�H���g�R���X�g���N�^
CEnemy::CEnemy()
:mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.4f)
, mColSearch1(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 30.0f)
, mColSearch2(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 10.0f)
, mpPlayer(0)
, mHp(HP)
{
	//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0){
		mModel.Load(OBJ, MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	mColSearch1.mTag = CCollider::ESEARCH1;
	mColSearch2.mTag = CCollider::ESEARCH2;
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
}

void CEnemy::Update(){
	//HP��0�ȉ��̎� ���j
	if (mHp <= 0){
		mHp--;
		//15�t���[�����ƂɃG�t�F�N�g
		if (mHp % 15 == 0){
			//�G�t�F�N�g����
			new CEffect(mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		}
		//���~������
		mPosition.mY -= 0.03f;
		CTransform::Update();
		return;
	}
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
	//������(X��)�̃x�N�g�������߂�
	CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
	//�����(Y��)�̃x�N�g�������߂�
	CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
	//�O����(Z��)�̃x�N�g�������߂�
	CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;
	//�v���C���[�̃|�C���g��0�ȊO�̎�
	if (mpPlayer){
		//�v���C���[�܂ł̃x�N�g�������߂�
		CVector vp = mpPlayer->mPosition - mPosition;
		float dx = vp.Dot(vx);	//���x�N�g���Ƃ̓��ς����߂�
		float dy = vp.Dot(vy);	//��x�N�g���Ƃ̓��ς����߂�
		float dz = vp.Dot(vz);	//�O�x�N�g���Ƃ̓��ς����߂�
		//X���̂��ꂪ2.0�ȉ�
		if (-2.0f < dx && dx < 2.0f){
			//Y���̂��ꂪ2.0�ȉ�
			if (-2.0f < dy && dy < 2.0f){
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
	}
	mpPlayer = 0;

	CVector vp = mPoint - mPosition;
	float dx = vp.Dot(vx); //���x�N�g���Ƃ̓���
	float dy = vp.Dot(vy); //��x�N�g���Ƃ̓���
	float margin = 0.1f;
	//���E�����։�]
	if (dx > margin){
		mRotation.mY += 1.0f;
	}
	else if (dx < -margin){
		mRotation.mY -= 1.0f;
	}
	//�㉺�����։�]
	if (dy > margin){
		mRotation.mX -= 1.0f;
	}
	else if (dy < -margin){
		mRotation.mX += 1.0f;
	}

	//�ړ�
	mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;
	CTransform::Update();	//�s��X�V
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CEnemy::Collision(CCollider *m, CCollider *o){
	//���肪�T�[�`�̎��͖߂�
	if (o->mTag == CCollider::ESEARCH1){
		return;
	}
	//�������T�[�`�p�̎�
	if (m->mTag == CCollider::ESEARCH1){
		//���肪���R���C�_
		if (o->mType == CCollider::ESPHERE){
			//���肪�v���C���[
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
			mHp--; //HP���Z
			//�G�t�F�N�g����
			new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		}
		break;
	case CCollider::ETRIANGLE: //�O�p�R���C�_
		CVector adjust; //�����l
		//�O�p�R���C�_�Ƌ��R���C�_�̔���
		if (CCollider::CollisionTriangleSphere(o, m, &adjust)){
			//���j�Œn�ʂɏՓ˂���Ɩ���
			if (mHp <= 0){
				mEnabled = false;
			}
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
	mColSearch2.ChangePriority();
	//�Փˏ��� ���s
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSearch1, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSearch2, COLLISIONRANGE);
}