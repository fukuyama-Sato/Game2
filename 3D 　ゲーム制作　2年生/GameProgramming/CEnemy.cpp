#include"CEnemy.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CEffect.h"
#include"CBullet.h"

#define OBJ "Character.obj"	//���f���̃t�@�C��
#define MTL "Character.mtl"	//���f���̃}�e���A���t�@�C��
#define EHP 20	//�ϋv�l
#define VELOCITYX 0.2f	//X���x
#define VELOCITYY 0.2f	//Y���x
#define VELOCITYZ 0.2f	//Z���x
#define GLAVITY -0.02f //�d��

#define JUMPPOWER 1	//�W�����v��

#define FIRERATE 6	//�U���̘A�ˑ��x
#define BULLETNUMBER 50	//���e��
#define BULLETTORTALNUM 600
#define RELOAD 260

CModel CEnemy::mModel;	//���f���f�[�^�쐬
int CEnemy::mHp = EHP;

//�f�t�H���g�R���X�g���N�^
CEnemy::CEnemy()
:mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)
, mColSearch1(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 200.0f)
, mpPlayer(0)
, mEFireRate(0)
, mMoveSpeedX(0)
, mMoveSpeedY(0)
, mMoveSpeedZ(0)
, mEBulletTortalNum(BULLETTORTALNUM)
, mEBulletNum(BULLETNUMBER)
, mEReloadTime(0)
, mEJump(true)
{
	//���f���������Ƃ��͓ǂݍ���
	if (mModel.mTriangles.size() == 0){
		mModel.Load(OBJ, MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	mColSearch1.mTag = CCollider::ESEARCH;
	mCollider.mTag = CCollider::EBODY;
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

//�ړ��֌W
void CEnemy::Move(){
	//������(X��)�̃x�N�g�������߂�
	CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
	//�����(Y��)�̃x�N�g�������߂�
	CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
	//�O����(Z��)�̃x�N�g�������߂�
	CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;

		CVector vp = mpPlayer->mPosition - mPosition;
		float dx = vp.Dot(vx);	//���x�N�g���Ƃ̓��ς����߂�
		float dy = vp.Dot(vy);	//��x�N�g���Ƃ̓��ς����߂�
		float dz = vp.Dot(vz);	//�O�x�N�g���Ƃ̓��ς����߂�

		if (-15.0f < dx && dx > 15.0f){
			if (-15.0f < dz && dz > 15.0f){
				mMoveControl = 2;
			}
		}

		//��
		if (mMoveControl == 0){
				mMoveSpeedX = 1.1f;
		}
		//��
		if (mMoveControl == 1){
				mMoveSpeedZ = 1.1f;
			}
		//��
		if (mMoveControl == 2){
				mMoveSpeedZ = -1.1f;
		}
}

void CEnemy::Update(){
	//�d��
	mMoveSpeedY += GLAVITY;
	mPosition.mY += mMoveSpeedY;
	//�^�����n
	if (mPosition.mY < 1){
		mEJump = true;
		if (mPosition.mY < 0){
			mMoveSpeedY = 0.08;
		}
	}

	//�����[�h
	if (mEBulletNum <= 0){
		mEReloadTime = RELOAD;
	}
	if (mEReloadTime > 0){
		mEBulletTortalNum -= BULLETNUMBER - mEBulletNum;
		mEBulletNum = BULLETNUMBER;
	}
	if (mEReloadTime > -1){
		mEReloadTime--;
	}
	if (mEFireRate >= 0)mEFireRate--;

	if (CEnemy::mHp > 0){
		//���悻1.5�b���ɖڕW�n�_���X�V
		int r = rand() % 100;	//rand()�͐����̗�����Ԃ�
		//100�Ŋ������]������߂�
		if (r == 0){
			if (mpPlayer){
				mPoint = mpPlayer->mPosition;
			}
			else{
				mPoint = mPoint * CMatrix().RotateY(45);
			}
		}
		//�W�����v
		if (mEJump == true && r == 98){
			mMoveSpeedY += JUMPPOWER;
			mEJump = false;
		}

		//������(X��)�̃x�N�g�������߂�
		CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
		//�����(Y��)�̃x�N�g�������߂�
		CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
		//�O����(Z��)�̃x�N�g�������߂�
		CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;

		if (mpPlayer){
			Move();
			CVector vp = mpPlayer->mPosition - mPosition;
			float dx = vp.Dot(vx);	//���x�N�g���Ƃ̓��ς����߂�
			float dy = vp.Dot(vy);	//��x�N�g���Ƃ̓��ς����߂�
			float dz = vp.Dot(vz);	//�O�x�N�g���Ƃ̓��ς����߂�

			if (-10.0f < dx && dx < 10.0f){
				if (-5.0f < dy && dy < 5.0f){
					if (dz > 0.0f){

							if (mEBulletTortalNum > 0){
								if (mEFireRate < 0 && mEBulletNum > 0 && mEReloadTime < 0){
									//�e�𔭎˂���
									CBullet *bullet = new CBullet();
									bullet->mTag = CCharacter::EBULLETENEMY;
									bullet->Set(0.1f, 1.5f);
									bullet->mPosition = CVector(-3.0f, 2.5f, 13.0f) * mMatrix;
									bullet->mRotation = mRotation;
									bullet->Update();
									mEBulletNum--;
									mEFireRate = FIRERATE;
								}
							}

					}
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

	}

	else{
		//���S�������G�t�F�N�g
		if (mHp > -4)
		new CEffect(mPosition, 15.0f, 15.0f, "exp.tga", 4, 4, 2);
		mHp--;
		if (mHp % 25 == 0){
			//�G�t�F�N�g����
			new CEffect(mPosition, 2.0f, 2.0f, "exp.tga", 4, 4, 2);
		}
	}

	//�ړ�
	mPosition = mPosition + CVector(mMoveSpeedX, 0.0f, mMoveSpeedZ) * mMatrixRotate;
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

	//����̃R���C�_�^�C�v�Ŕ���
	switch (o->mType){
	case CCollider::ESPHERE: //���R���C�_
		if (m->mTag == CCollider::EBODY){
		//�R���C�_��m��o���Փ˂��Ă��邩����
		if (CCollider::Collision(m, o)){
			if (o->mpParent->mTag == EBULLETPLAYER){
					mHp --; //HP���Z
					//�G�t�F�N�g����
					new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
				}
			}
		}
		break;
		
	case CCollider::ETRIANGLE: //�O�p�R���C�_
		CVector adjust; //�����l
		//�O�p�R���C�_�Ƌ��R���C�_�̔���
		if (CCollider::CollisionTriangleSphere(o, m, &adjust)){
			//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
			mPosition = mPosition + adjust;
			break;
		}
		
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