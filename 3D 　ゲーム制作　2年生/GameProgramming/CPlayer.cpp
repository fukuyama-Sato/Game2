//�v���C���[�N���X�̃C���N���[�h
#include"CPlayer.h"
#include"CKey.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CText.h"
#include"CUtil.h"
#include"CEnemy.h"
#include"CInput.h"

#define GLAVITY -0.02 //�d��
#define JUMPPOWER 1	//�W�����v��
#define VELOCITY 0.8	//�ړ���
#define SPEEDREMIT 5	//���x����
#define SUBERI 2	//����Ղ�
#define FIRERATE 6	//�U���̘A�ˑ��x

CText mText;
CEnemy Enemy;

CPlayer::CPlayer()
: mLine(this, &mMatrix, CVector(0.0f, 0.0f, -14.0f), CVector(0.0f, 0.0f, 17.0f))
, mLine2(this, &mMatrix, CVector(0.0f, 3.0f, -8.0f), CVector(0.0f, -2.0f, -8.0f))
, mLine3(this, &mMatrix, CVector(9.0f, 0.0f, -8.0f), CVector(-9.0f, 0.0f, -8.0f))
, mCollider(this, &mMatrix, CVector(0.0f,0.0f,0.0f),0.5f)
{
	mText.LoadTexture("FontWhite.tga", 1, 64);
	mTag = EPLAYER; //�^�O�̐ݒ�
	mSpeedX = NULL;
	mSpeedY = NULL;
	mSpeedZ = NULL;
	mJump = true;
	mFireRate = 0;

	mBeforMouseX = 0;
	mBeforMouseY = 0;
	mMouseMoveX = 0;
	mMouseMoveY = 0;

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

	if (CKey::Push('C')){
		//�}�E�X���W�R���\�[���o��
		printf("%d,%d\n", mMousePosX, mMousePosY);
	}

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
	//�㉺
	//if (mRotation.mX < 90 && mRotation.mX > -90){
		mRotation.mX -= mMouseMoveY;
	//}
	mRotation.mY -= mMouseMoveX;

	//���N���b�N�Œe�𔭎�
	if (CKey::Push(VK_LBUTTON) && mFireRate-- < 0){
		CBullet *bullet = new CBullet();
		bullet->Set(0.1f, 0.8f);
		bullet->mPosition = CVector(-2.0f, -1.0f, 10.0f) * mMatrix;
		bullet->mRotation = mRotation;
		bullet->Update();
		mFireRate = FIRERATE;
	}
	//�����܂Ń}�E�X�̑���

	//�ʒu�̈ړ�
	mPosition = CVector(mSpeedX,0.0f,mSpeedZ) * mMatrix;

	//�^�����n
	if (mPosition.mY < 1){
		mPosition.mY = 1;
		mSpeedY = 0;
		mJump = true;
		//�����[��
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

void CPlayer::Collision(CCollider *m, CCollider *o){
	//���g�̃R���C�_�^�C�v�̔���
	switch (m->mType){
	case CCollider::ELINE: //�����R���C�_
		if (o->mTag == CCollider::EBLOCK){
			CVector adjust; //	�����p�x�N�g��
			//�O�p�`�Ɛ����̏Փ˔���
			CCollider::CollisionTriangleLine(o, m, &adjust);
			//�ʒu�̍X�V(mPosition + adjust)
			mPosition = mPosition - adjust * -1;
			CTransform::Update();
		}
		break;

		//����̃R���C�_���O�p�R���C�_�̎�
		if (o->mType == CCollider::ETRIANGLE){
			CVector adjust; //	�����p�x�N�g��
			//�O�p�`�Ɛ����̏Փ˔���
			CCollider::CollisionTriangleLine(o, m, &adjust);
			//�ʒu�̍X�V(mPosition + adjust)
			mPosition = mPosition - adjust * -1;
			CTransform::Update();
		}
		break;
	//case CCollider::ESPHERE:
	//	if (o->mTag == CCharacter::EBULLETENEMY){

	//	}
	}
}

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
	sprintf(buf, "EHP:%d", Enemy.mHp);
	//������̕`��
	mText.DrawString(buf, -300, 0, 8, 16);

	//������̐ݒ�
	sprintf(buf, "+");
	//������̕`��
	mText.DrawString(buf, 0, 20, 20, 20);


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
	//�Փˏ��� ���s
	CCollisionManager::Get()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine3, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}