#include "CSceneGame.h"
//OpenGL
#include"glut.h"
#include"CVector.h"
#include"CTriangle.h"
#include"CKey.h"
#include"CMatrix.h"
#include"CTransform.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CVector.h"
#include"CPlayer.h"
#include"CEnemy.h"
#include"CBillBoard.h"
#include"CSphere.h"
#include"CCamera.h"
#include"CColliderTriangle.h"
#include"CColliderMesh.h"
#include"CInput.h"
#include"CKey.h"

CVector mEye;
CModel mBackGlound; //�w�i
CModel mModelC5;	//C5���f��

//���f������R���C�_�𐶐�
CColliderMesh mColliderMesh;

CMatrix mBackGroundMatrix;

void CSceneGame::Init() {

	//���f���t�@�C���̓���
	mModel.Load("Character.obj", "Character.mtl");
	mBackGlound.Load("stage.obj", "stage2.mtl");

	CMatrix matrix;
	matrix.Print();

	mBackGroundMatrix.Translate(0.0f, -2.0f, -500.0f);

	mPlayer.mpModel = &mModel;
	mPlayer.mScale = CVector(0.7f, 0.7f, 0.7f);
	mPlayer.mPosition = CVector(0.0f, 15.0f, -3.0f) * mBackGroundMatrix;
	mPlayer.mRotation = CVector(0.0f, 180.0f, 0.0f);

	new CEnemy(CVector(5.0f, 1.0f, -50.0f) * mBackGroundMatrix,
		CVector(0.0f,0.0f,0.0f), 
		CVector(0.7f, 0.7f, 0.7f));

	//�w�i���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X�Ɛe�s��͖���
	mColliderMesh.Set(NULL, &mBackGroundMatrix, &mBackGlound);

	mThurdPerson = false;

	mCamZ = 2;
	mFcamZ = 50.0f;

}

void CSceneGame::Update() {

	CTaskManager::Get()->Update();
	//�R���W�����}�l�[�W���̏Փˏ���
	CTaskManager::Get()->TaskCollision();

	//���_1,���_2,���_3,�@���f�[�^�̍쐬
	CVector v0, v1, v2, n;
	//�@����������Őݒ�
	n.mX = 0.0f; n.mY = 1.0f; n.mZ = 0.0f;
	//���_1�̍��W��ݒ�
	v0.mX = 0.0f; v0.mY = 0.0f; v0.mZ = 0.5f;
	//���_2�̍��W��ݒ�
	v1.mX = 1.0f; v1.mY = 0.0f; v1.mZ = 0.0f;
	//���_3�̍��W��ݒ�
	v2.mX = 0.0f; v2.mY = 0.0f; v2.mZ = -0.5f;

	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;	//���_,�����_,�����

	if (CKey::Once('C')){
		if (mThurdPerson == false){
			mThurdPerson = true;
			mCamZ = -10;
		}
		else {
			mThurdPerson = false;
			mCamZ = 2;
		}
	}

	if (mThurdPerson == false){
		//�J�����ʒu
		mCamX = 0.0f;
		mCamY = 1.0f;
		mFcamX = 0.0f;
		mFcamY = 0.0f;
		if (CKey::Push(VK_RBUTTON) && mCamZ < 9){
			mCamZ++;
		}
		else if (mCamZ > 2){
			mCamZ--;
		}
	}
	else{
		//�J�����ʒu
		mCamX = -6.0f;
		mCamY = 5.0f;
		mFcamX = -5.0f;
		mFcamY = 3.0f;
		if (CKey::Push(VK_RBUTTON) && mCamZ < 9){
			mCamZ++;
		}
		else if (mCamZ > -10){
			mCamZ--;
		}
	}

	//���_�����߂�
	e = CVector(mCamX, mCamY, mCamZ) * mPlayer.mMatrix;
	//�����_�����߂�
	c = CVector(mFcamX, mFcamY, mFcamZ) * mPlayer.mMatrix;
	//����������߂�
	u = CVector(0, 1, 0);

	//�N���A����
	//if (CEnemy::mHp <= 0){

	//}

	//�J�����̐ݒ�
	//gluLookAt(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
	//�J�����N���X�̐ݒ�
	Camera.Set(e, c, u);
	Camera.Render();

	mBackGlound.Render(mBackGroundMatrix);

	//�^�X�N���X�g�̍폜
	CTaskManager::Get()->Delete();
	//�`��
	CTaskManager::Get()->Render();

	CCollisionManager::Get()->Render();
}