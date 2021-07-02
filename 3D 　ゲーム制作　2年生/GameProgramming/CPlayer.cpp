//�v���C���[�N���X�̃C���N���[�h
#include"CPlayer.h"
#include"CKey.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CText.h"
#include"CUtil.h"
#include"CEnemy.h"
#include"CInput.h"

#define GLAVITY -0.02f //�d��
#define JUMPPOWER 1	//�W�����v��
#define VELOCITY 0.1f	//�ړ���
#define SPEEDREMIT 1	//���x����
#define SUBERI 2	//����Ղ�
#define FIRERATE 6	//�U���̘A�ˑ��x
#define BULLETNUMBER 50	//���e��
#define BULLETTORTALNUM 600
#define RELOAD 260
#define PHP 20

CText mText;

CPlayer::CPlayer()
: mLine(this, &mMatrix, CVector(0.0f, 0.0f, -6.0f), CVector(0.0f, 0.0f, 8.0f))
, mLine2(this, &mMatrix, CVector(0.0f, 5.0f, 2.0f), CVector(0.0f, -5.0f, 2.0f))
, mLine3(this, &mMatrix, CVector(7.0f, 0.0f, 0.0f), CVector(-7.0f, 0.0f, 0.0f))
, mCollider(this, &mMatrix, CVector(0.0f,0.0f,0.0f),2.5f)
, mTriangleCol(this, &mMatrix, 
	CVector(0.0f, -4.0f, 10.0f), CVector(5.0f, -6.0f, 0.0f),CVector(-5.0f, -6.0f, 0.0f))
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

	//�X�y�[�X�L�[�ŃW�����v
	if (CKey::Once(VK_SPACE) && mJump == true){
		mSpeedY += JUMPPOWER;
		mJump = false;
	}
	mSpeedY += GLAVITY;
	mPosition.mY += mSpeedY;


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
	if (CKey::Push('W')&& mSpeedZ < SPEEDREMIT){
		//Z����+�ړ�
		mSpeedZ += VELOCITY + 0.2;
	}



	//��������}�E�X�ɂ�鑀��
	mMouseMoveX = mMousePosX - mBeforMouseX;
	mMouseMoveY = mMousePosY - mBeforMouseY;
	//���_����

	//if (mMousePosX > -400 && mMousePosX < 400 && mMousePosY > -300 && mMousePosY < 300){
		mRotation.mX -= mMouseMoveY/1.5;
		mRotation.mY -= mMouseMoveX/1.5;
	//}
		//�㉺
		if (mRotation.mX > 90){

		}
		else if (mRotation.mX < -90){

		}

	

	//���N���b�N�Œe�𔭎�
		if (mBulletTortalNum > 0){
			if (CKey::Push(VK_LBUTTON) && mFireRate-- < 0 && mBulletNum > 0 && mReloadTime < 0){
				CBullet *bullet = new CBullet();
				bullet->mTag = CCharacter::EBULLETPLAYER;
				bullet->Set(1.0f, 2.8f);
				bullet->mPosition = CVector(-3.0f, 2.5f, 13.0f) * mMatrix;
				bullet->mRotation = mRotation;
				bullet->Update();
				mBulletNum--;
				mFireRate = FIRERATE;
			}
			//�����[�h
			if (CKey::Once('R') || mBulletNum <= 0){
				mReloadTime = RELOAD;
			}
			if (mReloadTime > 0){
				mBulletTortalNum -= BULLETNUMBER - mBulletNum;
				mBulletNum = BULLETNUMBER;
			}
			if (mReloadTime > -1){
				mReloadTime--;
			}
		}

		if (CKey::Push(VK_RBUTTON)&& mSpeedY < 0){
			mSpeedY += 0.018f;
			mSpeedX = mSpeedX / 1.4f;
			mSpeedZ = mSpeedZ / 1.4f;
		}
	//�����܂Ń}�E�X�̑���


	//�ʒu�̈ړ�
	mPosition = CVector(mSpeedX,0.0f,mSpeedZ) * mMatrix;

	////�^�����n
	//if (mPosition.mY < 1){
	//	mJump = true;
	//	if (mPosition.mY < 0){
	//		mSpeedY = 0;
	//	}
	//}

	//�����[��
	if (mPosition.mY < 10){
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
			if (mPosition.mY < 1){
				mJump = true;
				if (mPosition.mY < 0){
					mSpeedY = 0.1;
				}
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

	//Y���W�̕\��
	//������̐ݒ�
	sprintf(buf, "PY:%7.2f", mPosition.mY);
	//������̕`��
	mText.DrawString(buf, 100, 30, 8, 16);

	//X����]�l�̕\��
	//������̐ݒ�
	sprintf(buf, "RX:%7.2f", mRotation.mX);
	//������̕`��
	mText.DrawString(buf, 100, 0, 8, 16);

	//Y����]�l�̕\��
	//������̐ݒ�
	sprintf(buf, "RY:%7.2f", mRotation.mY);
	//������̕`��
	mText.DrawString(buf, 100, -100, 8, 16);



	//���x�\��
	//������̐ݒ�
	sprintf(buf, "VX:%f", mSpeedX);
	//������̕`��
	mText.DrawString(buf, -300, 200, 8, 16);

	//������̐ݒ�
	sprintf(buf, "VY:%f", mSpeedY);
	//������̕`��
	mText.DrawString(buf, -300, 150, 8, 16);

	//������̐ݒ�
	sprintf(buf, "VZ:%f", mSpeedZ);
	//������̕`��
	mText.DrawString(buf, -300, 100, 8, 16);


	//������̐ݒ�
	sprintf(buf, "HP:%d", mPlayerHp);
	//������̕`��
	mText.DrawString(buf, -300, 50, 8, 16);

	//�G��HP
	//������̐ݒ�
	sprintf(buf, "EHP:%d", CEnemy::mHp);
	//������̕`��
	mText.DrawString(buf, -300, 0, 8, 16);

	//�Ə�(��)
	//������̐ݒ�
	sprintf(buf, "+");
	//������̕`��
	mText.DrawString(buf, 0, 20, 20, 20);

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
	mCollider.ChangePriority();
	mTriangleCol.ChangePriority();
	//�Փˏ��� ���s
	CCollisionManager::Get()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine3, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mTriangleCol,COLLISIONRANGE);
}