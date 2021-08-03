//�v���C���[�N���X�̃C���N���[�h
#include"CPlayer.h"
#include"CKey.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CText.h"
#include"CUtil.h"
#include"CEnemy.h"
#include"CInput.h"
#include"CSound.h"

#define GLAVITY -0.02f //�d��
#define JUMPPOWER 0.7f	//�W�����v��
#define JUMPRECHARGE 70	//���W�����v�܂ł̑҂�����

#define STEPSPEED 2
#define STEPRECHARGE 120

#define VELOCITY 0.065f	//�ړ���
#define SPEEDREMIT 0.6	//���x����
#define MOUSESPEEDX 1.2f	//�}�E�X�����x
#define MOUSESPEEDY 1.2f	//�}�E�X�c���x
#define SUBERI 2	//����Ղ�

#define FIRERATE 6	//�U���̘A�ˑ��x
#define BULLETNUMBER 50	//���e��
#define BULLETTORTALNUM 350
#define RELOAD 320

#define PHP 150	//HP

int CPlayer::mPlayerHp = PHP;

CText mText;

extern CSound Fire;
extern CSound Reload;
extern CSound NoAmmo;
extern CSound Jump;

extern CSound Hit;
extern CSound Damage;
extern CSound Caution;
extern CSound HP0;


CPlayer::CPlayer()
: mLine(this, &mMatrix, CVector(0.0f, 0.0f, -6.0f), CVector(0.0f, 0.0f, 6.0f))
, mLine2(this, &mMatrix, CVector(0.0f, 6.0f, 0.0f), CVector(0.0f, -6.0f, 0.0f))
, mLine3(this, &mMatrix, CVector(6.0f, 0.0f, 0.0f), CVector(-6.0f, 0.0f, 0.0f))
, mLine4(this, &mMatrix, CVector(0.0f, 4.0f, 4.0f), CVector(0.0f, -4.0f, -4.0f))
, mLine5(this, &mMatrix, CVector(0.0f, -4.0f, 4.0f), CVector(0.0f, 4.0f, -4.0f))
, mCollider(this, &mMatrix, CVector(0.0f,0.0f,0.0f),3.5f)
{
	mText.LoadTexture("FontWhite.tga", 1, 64);
	mTag = EPLAYER; //�^�O�̐ݒ�
	mSpeedX = NULL;
	mSpeedY = NULL;
	mSpeedZ = NULL;
	mJumpTimer = 0;
	mJump = true;
	mStep = true;
	mFireRate = 0;

	mBeforMouseX = 0;
	mBeforMouseY = 0;
	mMouseMoveX = 0;
	mMouseMoveY = 0;

	mBulletTortalNum = BULLETTORTALNUM;
	mBulletNum = BULLETNUMBER;
	mReloadTime = 0;

	mMouseSpeedX = MOUSESPEEDX;
	mMouseSpeedY = MOUSESPEEDY;
}

//�X�V����
void CPlayer::Update(){
  //�}�E�X�ݒ�
	float mMousePosX, mMousePosY;	//�}�E�X�J�[�\�����W�擾�p
	//�}�E�X�J�[�\�����W�̎擾
	CInput::GetMousePos(&mMousePosX, &mMousePosY);

	//�Q�[����ʒ��S����̍��W�֕ϊ�
	mMousePosX -= 400;
	mMousePosY = 300 - mMousePosY;

	mSpeedY += GLAVITY;
	mPosition.mY += mSpeedY;

	if (mPlayerHp >= 0){
		//�X�y�[�X�L�[�ŃW�����v
		if (CKey::Once(VK_SPACE) && mJump == true){
			mSpeedY = JUMPPOWER;
			mJumpTimer = JUMPRECHARGE;
			Jump.Play();
			mJump = false;
		}

		//shift�L�[�ŉ��
		if (CKey::Once(VK_SHIFT) && mStep == true){
			if (CKey::Push('A')){
				//X����+�ړ�
				mSpeedX = STEPSPEED;
			}
			if (CKey::Push('D')){
				//X����-�ړ�
				mSpeedX = -STEPSPEED;
			}
			if (CKey::Push('S')){
				//Z����-�ړ�
				mSpeedZ = -STEPSPEED;
			}
			if (CKey::Push('W')){
				//Z����+�ړ�
				mSpeedZ = STEPSPEED;
			}
			Jump.Play();
			mStepTimer = STEPRECHARGE;
			mStep = false;
		}

		if (mStepTimer < 0){
			mStep = true;
		}

		//CTransform�̍X�V
		CTransform::Update();

		//�ړ�
		if (CKey::Push('A') && mSpeedX < SPEEDREMIT){
			//X����+�ړ�
			mSpeedX += VELOCITY;
		}
		if (CKey::Push('D') && mSpeedX > -SPEEDREMIT){
			//X����-�ړ�
			mSpeedX -= VELOCITY;
		}
		if (CKey::Push('S') && mSpeedZ > -SPEEDREMIT){
			//Z����-�ړ�
			mSpeedZ -= VELOCITY;
		}
		if (CKey::Push('W') && mSpeedZ < SPEEDREMIT + 0.3f){
			//Z����+�ړ�
			mSpeedZ += VELOCITY;
		}



		//��������}�E�X�ɂ�鑀��
		//�ړ���
		mMouseMoveX = mMousePosX - mBeforMouseX;
		mMouseMoveY = mMousePosY - mBeforMouseY;

		//���_����

		mRotation.mX -= mMouseMoveY / mMouseSpeedX;
		mRotation.mY -= mMouseMoveX / mMouseSpeedY;

		if (mRotation.mX < -90)
			mRotation.mX = -89;

		if (mRotation.mX > 85)
			mRotation.mX = 84;




		//���N���b�N�Œe�𔭎�
		if (mBulletTortalNum >= 0){
			if (CKey::Push(VK_LBUTTON) && mFireRate-- < 0 && mBulletNum > 0 && mReloadTime < 0){
				CBullet *bullet = new CBullet();
				bullet->mTag = CCharacter::EBULLETPLAYER;
				bullet->Set(0.5f, 4.0f);
				bullet->mPosition = CVector(-3.0f, 3.0f, 10.0f) * mMatrix;
				bullet->mRotation = mRotation;
				bullet->Update();
				Fire.Play();
				mBulletNum--;
				mFireRate = FIRERATE;
			}
			//�����[�h
			if (mBulletTortalNum > 49){
				if (CKey::Once('R') && mBulletNum != 50 || mBulletNum <= 0){
					mReloadTime = RELOAD;
				}
			}
			if (mReloadTime == RELOAD){
				if (mBulletTortalNum >= 50){
					mBulletTortalNum -= BULLETNUMBER - mBulletNum;
					mBulletNum = BULLETNUMBER;
					NoAmmo.Play();
				}
				else{
					mBulletNum += mBulletTortalNum;
					mBulletTortalNum = 0;
				}
			}

			if (mReloadTime > -1){
				mReloadTime--;
			}
			if (mReloadTime == 0){
				Reload.Play();
			}
		}

		if (CKey::Push(VK_RBUTTON) && mSpeedY < 0){
			mSpeedY += 0.018f;
			mSpeedX = mSpeedX / 1.4f;
			mSpeedZ = mSpeedZ / 1.4f;
		}
		//�����܂Ń}�E�X�̑���
	}

	//SE
	if (mPlayerHp == 50)Caution.Repeat();
	if (mPlayerHp == 50)Damage.Play();
	if (mPlayerHp == 0){
		HP0.Play();
		mPlayerHp--;
	}
	//�ʒu�̈ړ�
	mPosition = CVector(mSpeedX,0.0f,mSpeedZ) * mMatrix;

	//�����[��
	if (mPosition.mY < 5){
		//X
		if (mSpeedX >= 0.01){
			mSpeedX -= VELOCITY / SUBERI;
		}
		else if (mSpeedX <= -0.01){
			mSpeedX += VELOCITY / SUBERI;
		}
		//Z
		if (mSpeedZ >= 0.01){
			mSpeedZ -= VELOCITY / SUBERI;
		}
		else if (mSpeedZ <= -0.01){
			mSpeedZ += VELOCITY / SUBERI;
		}
	}

	mBeforMouseX = mMousePosX;
	mBeforMouseY = mMousePosY;

	if (mJumpTimer >= 0)
	mJumpTimer--;

	if (mStepTimer >= 0)
		mStepTimer--;
}

//�ڐG����
void CPlayer::Collision(CCollider *m, CCollider *o){
	//���g�̃R���C�_�^�C�v�Ŕ���
	switch (m->mType){
	case CCollider::ELINE: //�����R���C�_

		//����̃R���C�_���O�p�R���C�_�̎�
		if (o->mType == CCollider::ETRIANGLE){
			CVector adjust; //	�����p�x�N�g��
			//�O�p�`�Ɛ����̏Փ˔���
			CCollider::CollisionTriangleLine(o, m, &adjust);
			//�ʒu�̍X�V(mPosition + adjust)
			mPosition = mPosition - adjust * -1;
			//�^�����n
			if (mPosition.mY < 1.6f && mJumpTimer < 0){
				mJump = true;
				if (mPosition.mY < 0.5f)
					mSpeedY += 0.002f;
			}
			CTransform::Update();
			break;
		}

	case CCollider::ESPHERE:
		if (CCollider::Collision(m, o)){
			if (o->mpParent->mTag == EBULLETENEMY){
				mPlayerHp--;
				Hit.Play();
				new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
				break;
			}
		}
		
	}
}

//��ʏ�2D�`��
void CPlayer::Render(){
	//�e�̕`�揈��
	CCharacter::Render();

	//2D�̕`��J�n
	CUtil::Start2D(-400, 400, -300, 300);
	//�`��F�̐ݒ�
	glColor4f(0.1f, 0.8f, 0.1f, 1.0f);
	//������ҏW�G���A�̍쐬
	char buf[64];

	////���x�\��
	////������̐ݒ�
	//sprintf(buf, "PY:%5f", mPosition.mY);
	////������̕`��
	//mText.DrawString(buf, -300, 200, 8, 16);

	////���x�\��
	////������̐ݒ�
	//sprintf(buf, "VX:%f", mSpeedX);
	////������̕`��
	//mText.DrawString(buf, -300, 200, 8, 16);

	////������̐ݒ�
	//sprintf(buf, "VY:%f", mSpeedY);
	////������̕`��
	//mText.DrawString(buf, -300, 150, 8, 16);

	////������̐ݒ�
	//sprintf(buf, "VZ:%f", mSpeedZ);
	////������̕`��
	//mText.DrawString(buf, -300, 100, 8, 16);


	//������̐ݒ�
	sprintf(buf, "HP:%d", mPlayerHp);
	//������̕`��
	mText.DrawString(buf, -100, 250, 20, 20);


	//�Ə�(��)
	//������̐ݒ�
	if (mReloadTime < 0){
		sprintf(buf, "[+]");
		//������̕`��
		mText.DrawString(buf, -30, 20, 15, 15);
	}
	
	//�e��
	//������̐ݒ�
	sprintf(buf, "BULLET:%d",mBulletNum);
	//������̕`��
	mText.DrawString(buf, -300, -100, 8, 16);

	//������̐ݒ�
	sprintf(buf, "%d", mBulletTortalNum);
	//������̕`��
	mText.DrawString(buf, -300, -170, 8, 16);

	//�����[�h����
	//������̐ݒ�
	if (mReloadTime > 0){
		sprintf(buf, "RELOAD");
		//������̕`��
		mText.DrawString(buf, -70, 10, 12, 16);
		sprintf(buf, "%d", mReloadTime / 60);
		//������̕`��
		mText.DrawString(buf, -5, -15, 12, 16);
	}

	//�G��HP
	//������̐ݒ�
	if (CEnemy::mHp > 0){
		glColor4f(1.0f, 0.5f, 0.5f, 1.0f);
		sprintf(buf, "ENEMY HP");
		mText.DrawString(buf, -150, -200, 16, 16);

		sprintf(buf, "%d", CEnemy::mHp);
		mText.DrawString(buf, -50, -250, 16, 16);
	}

	//�I������
	glColor4f(0.4f, 0.6f, 0.9f, 1.0f);
	if (CEnemy::mHp <= 0 && CPlayer::mPlayerHp <= 0
		|| mBulletTortalNum <= 0 && mBulletNum <= 0 
		&& CEnemy::mEBulletTortalNum <= 0 && mBulletNum <= 0){
		sprintf(buf, "DRAW");
		//������̕`��
		mText.DrawString(buf, -100, 80, 30, 30);
	}
	else if (CEnemy::mHp <= 0){
		sprintf(buf, "YOU WIN");
		//������̕`��
		mText.DrawString(buf, -180, 80, 30, 30);
	}
	else if (CPlayer::mPlayerHp <= 0){
		sprintf(buf, "YOU DEAD");
		//������̕`��
		mText.DrawString(buf, -200, 80, 30, 30);
	}

	//2D�`��I��
	CUtil::End2D();
}

//�Փˏ���
void CPlayer::TaskCollision(){
	//�R���C�_�̗D��x�ύX
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	mLine4.ChangePriority();
	mLine5.ChangePriority();
	mCollider.ChangePriority();
	//�Փˏ��� ���s
	CCollisionManager::Get()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine3, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine4, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine5, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}