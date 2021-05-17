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

CVector mEye;
CModel mBackGlound; //�w�i
CModel mModelC5;	//C5���f��
CModel mModelSphere;

//���f������R���C�_�𐶐�
CColliderMesh mColliderMesh;

CMatrix mBackGroundMatrix;

void CSceneGame::Init() {
	//mEye = CVector(1.0f, 2.0f, 3.0f);

	//���f���t�@�C���̓���
	mModel.Load("f14.obj", "f14.mtl");
	mBackGlound.Load("sky.obj", "sky.mtl");
	mModelSphere.Load("sphere.obj", "sphere.mtl");

	CMatrix matrix;
	matrix.Print();

	mBackGroundMatrix.Translate(0.0f, 0.0f, -500.0f);

	mPlayer.mpModel = &mModel;
	mPlayer.mScale = CVector(0.1f, 0.1f, 0.1f);
	mPlayer.mPosition = CVector(0.0f, 15.0f, -3.0f) * mBackGroundMatrix;
	mPlayer.mRotation = CVector(0.0f, 180.0f, 0.0f);

	/*new CEnemy(CVector(5.0f, 1.0f, -10.0f) * mBackGroundMatrix,
		CVector(), CVector(0.1f, 0.1f, 0.1f));*/

	new CSphere(&mModelSphere,
		CVector(50.0f, 0.0f, -3.0f) * mBackGroundMatrix,
		CVector(),
		CVector(10.0f, 10.0f, 10.0f));

	//�r���{�[�h�쐬
	new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);

	//�w�i���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X�Ɛe�s��͖���
	mColliderMesh.Set(NULL, &mBackGroundMatrix, &mBackGlound);

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

	//���_�����߂�
	e = CVector(0.0f, 3.0f, -10.0) * mPlayer.mMatrix;
	//�����_�����߂�
	c = CVector(0.0f, 0.0f, 20.0) * mPlayer.mMatrix;
	//����������߂�
	u = CVector(0, 1, 0);

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