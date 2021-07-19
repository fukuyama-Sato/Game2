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
#define JUMPPOWER 2.0f	//�W�����v��
#define JUMPRECHARGE 70	//���W�����v�܂ł̑҂�����

#define VELOCITY 0.1f	//�ړ���
#define SPEEDREMIT 1	//���x����
#define MOUSESPEEDX 0.8f	//�}�E�X�����x
#define MOUSESPEEDY 0.8f	//�}�E�X�c���x
#define SUBERI 2	//����Ղ�

#define FIRERATE 6	//�U���̘A�ˑ��x
#define BULLETNUMBER 50	//���e��
#define BULLETTORTALNUM 450
#define RELOAD 260

#define PHP 150	//HP

CText mText;

extern CSound Fire;
extern CSound Reload;
extern CSound NoAmmo;
extern CSound Jump;

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
	mJump = true;
	mFireRate = 0;
	mPlayerHp = PHP;

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
			mSpeedY += JUMPPOWER;
			mJumpTimer = JUMPRECHARGE;
			Jump.Play();
			mJump = false;
		}


		//CTransform�̍X�V
		CTransform::Update();

		//�ړ�
		if (CKey::Push('A') && mSpeedX < SPEEDREMIT + 0.5f){
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
		if (CKey::Push('W') && mSpeedZ < SPEEDREMIT){
			//Z����+�ړ�
			mSpeedZ += VELOCITY + 0.2;
		}



		//��������}�E�X�ɂ�鑀��
		//�ړ���
		mMouseMoveX = mMousePosX - mBeforMouseX;
		mMouseMoveY = mMousePosY - mBeforMouseY;

		//���_����

		//if (mMousePosX > -400 && mMousePosX < 400 && mMousePosY > -300 && mMousePosY < 300){
		mRotation.mX -= mMouseMoveY / mMouseSpeedX;
		mRotation.mY -= mMouseMoveX / mMouseSpeedY;
		//}

		if (mRotation.mX < -90)
			mRotation.mX = -89;

		if (mRotation.mX > 85)
			mRotation.mX = 84;




		//���N���b�N�Œe�𔭎�
		if (mBulletTortalNum >= 0){
			if (CKey::Push(VK_LBUTTON) && mFireRate-- < 0 && mBulletNum > 0 && mReloadTime < 0){
				CBullet *bullet = new CBullet();
				bullet->mTag = CCharacter::EBULLETPLAYER;
				bullet->Set(1.0f, 2.8f);
				bullet->mPosition = CVector(-3.0f, 2.5f, 13.0f) * mMatrix;
				bullet->mRotation = mRotation;
				bullet->Update();
				Fire.Play();
				mBulletNum--;
				mFireRate = FIRERATE;
			}
			//�����[�h
			if (mBulletTortalNum > 0){
				if (CKey::Once('R') || mBulletNum <= 0){
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

	if (mPlayerHp == 0){
		HP0.Play();
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
	mText.DrawString(buf, -300, 50, 8, 16);

	////�G��HP
	////������̐ݒ�
	//sprintf(buf, "EHP:%d", CEnemy::mHp);
	////������̕`��
	//mText.DrawString(buf, -300, 0, 8, 16);

	//�Ə�(��)
	//������̐ݒ�
	sprintf(buf, "[+]");
	//������̕`��
	mText.DrawString(buf, -30, 20, 15, 15);

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
	sprintf(buf, "%d", mReloadTime);
	//������̕`��
	mText.DrawString(buf, 100, -170, 8, 16);

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
	//mTriangleCol.ChangePriority();
	//�Փˏ��� ���s
	CCollisionManager::Get()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine3, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine4, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine5, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
	//CCollisionManager::Get()->Collision(&mTriangleCol,COLLISIONRANGE);
}