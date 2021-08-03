#include "CSceneGame.h"
//OpenGL
#include"glut.h"
#include"CVector.h"
#include"CTriangle.h"
#include"CMatrix.h"
#include"CTransform.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CColliderTriangle.h"
#include"CColliderMesh.h"

#include"CPlayer.h"
#include"CEnemy.h"
#include"CBillBoard.h"
#include"CSphere.h"
#include"CBarricade.h"
#include"CPillar.h"

#include"CInput.h"
#include"CKey.h"
#include"CCamera.h"
#include"CSound.h"

CVector mEye;

CSound Hit;
CSound Damage;
CSound Caution;
CSound HP0;

CSound Reload;
CSound Fire;
CSound NoAmmo;
CSound Jump;

CSound EnemyFire;
CSound EnemyJump;

//CSound Bgm;

CModel mBackGround; //背景

CModel mPillar;
CModel mBarricade;

//モデルからコライダを生成
CColliderMesh mColliderMesh;

CMatrix mBackGroundMatrix;

void CSceneGame::Init() {

	//モデルファイルの入力
	mModel.Load("Character.obj", "Character.mtl");
	mPillar.Load("Pillar.obj","Pillar.mtl");
	mBarricade.Load("barricade.obj", "barricade.mtl");
	mBackGround.Load("stage.obj", "stage.mtl");

	//wav読み込み
	//Bgm.Load(".wav");

	Hit.Load("Hit.wav");
	Damage.Load("Damaged.wav");
	Caution.Load("Caution.wav");
	HP0.Load("HP0.wav");

	Fire.Load("Fire.wav");
	Reload.Load("Reload.wav");
	NoAmmo.Load("NoAmmo.wav");
	Jump.Load("Jump.wav");

	EnemyFire.Load("EnemyFire.wav");
	EnemyJump.Load("EnemyJump.wav");

	CMatrix matrix;
	matrix.Print();

	mBackGroundMatrix.Translate(0.0f, -2.0f, -500.0f);

	mPlayer.mpModel = &mModel;
	mPlayer.mScale = CVector(0.7f, 0.7f, 0.7f);
	mPlayer.mPosition = CVector(0.0f, 5.0f, -60.0f) * mBackGroundMatrix;
	mPlayer.mRotation = CVector(0.0f, 0.0f, 0.0f);

	new CEnemy(CVector(5.0f, 1.0f, 50.0f) * mBackGroundMatrix,
		CVector(0.0f,180.0f,0.0f),
		CVector(0.7f, 0.7f, 0.7f));

	new CPillar(&mPillar,
		CVector(-50.0f, 0.0f, 0.0f) * mBackGroundMatrix,
		CVector(),
		CVector(2.0f, 10.0f, 2.0f));

	new CPillar(&mPillar,
		CVector(50.0f, 0.0f, 20.0f) * mBackGroundMatrix,
		CVector(),
		CVector(2.0f, 10.0f, 2.0f));

	new CBarricade(&mBarricade,
		CVector(-40.0f, 0.0f, -20.0f) * mBackGroundMatrix,
		CVector(0.0f, 50.0f, 0.0f),
		CVector(3.0f, 2.0f, 1.0f));

	new CBarricade(&mBarricade,
		CVector(50.0f, 0.0f, 0.0f) * mBackGroundMatrix,
		CVector(),
		CVector(4.5f, 4.0f, 4.0f));

	new CBarricade(&mBarricade,
		CVector(-70.0f, 0.0f, 40.0f) * mBackGroundMatrix,
		CVector(0.0f, 90.0f, 0.0f),
		CVector(4.5f, 8.0f, 4.0f));

	//背景モデルから三角コライダを生成
	//親インスタンスと親行列は無し
	mColliderMesh.Set(NULL, &mBackGroundMatrix, &mBackGround);


	mThurdPerson = false;

	mCamZ = 2;
	mFcamZ = 40.0f;

}

void CSceneGame::Update() {

	CTaskManager::Get()->Update();
	//コリジョンマネージャの衝突処理
	CTaskManager::Get()->TaskCollision();

	//頂点1,頂点2,頂点3,法線データの作成
	CVector v0, v1, v2, n;
	//法線を上向きで設定
	n.mX = 0.0f; n.mY = 1.0f; n.mZ = 0.0f;
	//頂点1の座標を設定
	v0.mX = 0.0f; v0.mY = 0.0f; v0.mZ = 0.5f;
	//頂点2の座標を設定
	v1.mX = 1.0f; v1.mY = 0.0f; v1.mZ = 0.0f;
	//頂点3の座標を設定
	v2.mX = 0.0f; v2.mY = 0.0f; v2.mZ = -0.5f;

	//カメラのパラメータを作成する
	CVector e, c, u;	//視点,注視点,上方向

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
		//カメラ位置
		mCamX = 0.0f;
		mCamY = 1.0f;
		mFcamX = 0.0f;
		mFcamY = -0.5f;
		if (CKey::Push(VK_RBUTTON) && mCamZ < 9){
			mCamZ += 0.4f;
		}
		else if (mCamZ > 2){
			mCamZ -= 0.4f;
		}
	}
	else{
		//カメラ位置
		mCamX = -6.0f;
		mCamY = 5.0f;
		mFcamX = -5.0f;
		mFcamY = 2.0f;
		if (CKey::Push(VK_RBUTTON) && mCamZ < 6){
			mCamZ += 0.4f;
		}
		else if (mCamZ > -10){
			mCamZ -= 0.4f;
		}
	}

	//視点を求める
	e = CVector(mCamX, mCamY, mCamZ) * mPlayer.mMatrix;
	//注視点を求める
	c = CVector(mFcamX, mFcamY, mFcamZ) * mPlayer.mMatrix;
	//上方向を求める
	u = CVector(0, 1, 0);

	//カメラクラスの設定
	Camera.Set(e, c, u);
	Camera.Render();

	mBackGround.Render(mBackGroundMatrix);

	//タスクリストの削除
	CTaskManager::Get()->Delete();
	//描画
	CTaskManager::Get()->Render();

	//CCollisionManager::Get()->Render();
}