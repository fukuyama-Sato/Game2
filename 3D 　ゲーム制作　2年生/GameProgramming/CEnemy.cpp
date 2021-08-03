#include"CEnemy.h"
#include"CSound.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CEffect.h"
#include"CBullet.h"

#define OBJ "Enemy.obj"	//���f���̃t�@�C��
#define MTL "Enemy.mtl"	//���f���̃}�e���A���t�@�C��
#define VELOCITYX 0.5f	//X���x
#define VELOCITYY 0.5f	//Y���x
#define VELOCITYZ 0.5f	//Z���x
#define GLAVITY -0.02f //�d��

#define JUMPPOWER 0.7f	//�W�����v��

#define FIRERATE 6	//�U���̘A�ˑ��x
#define BULLETNUMBER 50	//���e��
#define BULLETTORTALNUM 450
#define RELOAD 320

#define EHP 150	//�ϋv�l

CModel CEnemy::mModel;	//���f���f�[�^�쐬
int CEnemy::mHp = EHP;
int CEnemy::mEBulletTortalNum = BULLETTORTALNUM;
int CEnemy::mEBulletNum = BULLETNUMBER;

extern CSound EnemyFire;
extern CSound EnemyJump;

extern CSound Hit;
extern CSound HP0;

//�f�t�H���g�R���X�g���N�^
CEnemy::CEnemy()
: mELine(this, &mMatrix, CVector(0.0f, 0.0f, -6.0f), CVector(0.0f, 0.0f, 8.0f))
, mELine2(this, &mMatrix, CVector(0.0f, 5.0f, 2.0f), CVector(0.0f, -5.0f, 2.0f))
, mELine3(this, &mMatrix, CVector(7.0f, 0.0f, 0.0f), CVector(-7.0f, 0.0f, 0.0f))
, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 4.0f), 5.0f)
, mColSearch1(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 150.0f)
, mpPlayer(0)
, mEFireRate(0)
, mMoveSpeedX(0)
, mMoveSpeedY(0)
, mMoveSpeedZ(0)
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


void CEnemy::Update(){
	//�d��
	mMoveSpeedY += GLAVITY;
	mPosition.mY += mMoveSpeedY;
	//�^�����n
	if (mPosition.mY < 3.0f){
		mEJump = true;
		if (mPosition.mY < 0){
			mMoveSpeedY = 0.1;
		}
	}

	//��������
	if (mMoveSpeedX > 0){
		mMoveSpeedX -= 0.1f;
	}
	else if (mMoveSpeedX < 0){
		mMoveSpeedX += 0.1f;
	}
	if (mMoveSpeedZ > 0){
		mMoveSpeedZ -= 0.1f;
	}
	else if (mMoveSpeedZ < 0){
		mMoveSpeedZ += 0.1f;
	}

	if (CEnemy::mHp > 0){

	//�ړ�
	mPosition = mPosition + CVector(mMoveSpeedX, 0.0f, mMoveSpeedZ) * mMatrixRotate;

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
	//���ˊԊu
	if (mEFireRate >= 0)mEFireRate--;


		int r = rand() % 100;	//rand()�͐����̗�����Ԃ�
		//100�Ŋ������]������߂�
		if (r > 60){
			if (mpPlayer){
				mPoint = mpPlayer->mPosition;
			}
		}

		//�W�����v
		if (mEJump == true && r == 40){
			mMoveSpeedY = JUMPPOWER;
			EnemyJump.Play();
			mEJump = false;
		}

		//������(X��)�̃x�N�g�������߂�
		CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
		//�����(Y��)�̃x�N�g�������߂�
		CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
		//�O����(Z��)�̃x�N�g�������߂�
		CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;

		if (mpPlayer){
			CVector vp = mpPlayer->mPosition - mPosition;
			float dx = vp.Dot(vx);	//���x�N�g���Ƃ̓��ς����߂�
			float dy = vp.Dot(vy);	//��x�N�g���Ƃ̓��ς����߂�
			float dz = vp.Dot(vz);	//�O�x�N�g���Ƃ̓��ς����߂�

			if (mHp > 106){
				if (mMoveSpeedX < 0.5)
				mMoveSpeedX += 0.2;
				if (mMoveSpeedZ > -0.2)
				mMoveSpeedZ += -0.1;
			}
			else if (mHp < 106 && mHp >= 82){
				if (mMoveSpeedX > -0.3f)
					mMoveSpeedX -= 0.2f;
				if (mMoveSpeedZ < 0.2f)
					mMoveSpeedZ += 0.04f;
			}
			else if(mHp < 82 && mHp >= 54){
				if (mMoveSpeedX > -0.5f)
				mMoveSpeedX -= 0.2f;
				if (mMoveSpeedZ < -0.1f)
				mMoveSpeedZ -= 0.01f;
			}
			else if (mHp < 54 && mHp >= 38){
				if (mMoveSpeedX > -0.2f)
					mMoveSpeedX -= 0.2f;
				if (mMoveSpeedZ < 0.1f)
					mMoveSpeedZ = 0.01f;
			}
			if (mHp < 38){
				if (mMoveSpeedX < 0.6f)
				mMoveSpeedX += 0.6f;
				if (mMoveSpeedZ > 0.4f)
				mMoveSpeedZ -= 0.2f;
			}


			if (-2.0f < dx && dx < 2.0f){
				if (-2.0f < dy && dy < 2.0f){
					if (dz > 0.0f){

							if (mEBulletTortalNum > 0){
								if (mEFireRate < 0 && mEBulletNum > 0 && mEReloadTime < 0){
									//�e�𔭎˂���
									CBullet *bullet = new CBullet();
									bullet->mTag = CCharacter::EBULLETENEMY;
									bullet->Set(0.5f, 2.5f);
									bullet->mPosition = CVector(-3.0f, 2.5f, 10.0f) * mMatrix;
									bullet->mRotation = mRotation;
									bullet->Update();
									EnemyFire.Play();
									mEBulletNum--;
									mEFireRate = FIRERATE;
								}
							}

					}
				}
			}

		}

		CVector vp = mPoint - mPosition;
		float dx = vp.Dot(vx); //���x�N�g���Ƃ̓���
		float dy = vp.Dot(vy); //��x�N�g���Ƃ̓���

		float margin = 0.1f;

		//���E�����։�]
		if (dx > margin){
			mRotation.mY += 4.5f;
		}
		else if (dx < -margin){
			mRotation.mY -= 4.5f;
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
		if (mHp > -4){
			new CEffect(mPosition, 15.0f, 15.0f, "exp.tga", 4, 4, 2);
			HP0.Play();
		}
		mHp--;
		if (mHp % 25 == 0){
			//�G�t�F�N�g����
			new CEffect(mPosition, 8.0f, 8.0f, "exp.tga", 4, 4, 2);
		}
	}
	CTransform::Update();	//�s��X�V
}


//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CEnemy::Collision(CCollider *m, CCollider *o){

	CVector adjust; //�����l

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
					Hit.Play();
					//�G�t�F�N�g����
					new CEffect(o->mpParent->mPosition, 2.0f, 2.0f, "exp.tga", 4, 4, 2);
				}
			}
		}
		break;
		
	case CCollider::ETRIANGLE: //�O�p�R���C�_
		if (m->mType == CCollider::ESPHERE){
			CVector adjust; //�����l
			//�O�p�R���C�_�Ƌ��R���C�_�̔���
			if (CCollider::CollisionTriangleSphere(o, m, &adjust)){
				//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
				mPosition = mPosition + adjust;
				break;
			}
		}
	}

}

void CEnemy::TaskCollision(){
	//�R���C�_�̗D��x�ύX
	mELine.ChangePriority();
	mELine2.ChangePriority();
	mELine3.ChangePriority();
	mCollider.ChangePriority();
	mColSearch1.ChangePriority();
	//�Փˏ��� ���s
	CCollisionManager::Get()->Collision(&mELine, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mELine2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mELine3, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSearch1, COLLISIONRANGE);
}