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
CModel mBackGlound; //背景
CModel mModelC5;	//C5モデル
CModel mModelSphere;

//モデルからコライダを生成
CColliderMesh mColliderMesh;

CMatrix mBackGroundMatrix;

void CSceneGame::Init() {
	//mEye = CVector(1.0f, 2.0f, 3.0f);

	//モデルファイルの入力
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

	//ビルボード作成
	new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);

	//背景モデルから三角コライダを生成
	//親インスタンスと親行列は無し
	mColliderMesh.Set(NULL, &mBackGroundMatrix, &mBackGlound);

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

	//視点を求める
	e = CVector(0.0f, 3.0f, -10.0) * mPlayer.mMatrix;
	//注視点を求める
	c = CVector(0.0f, 0.0f, 20.0) * mPlayer.mMatrix;
	//上方向を求める
	u = CVector(0, 1, 0);

	//カメラの設定
	//gluLookAt(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
	//カメラクラスの設定
	Camera.Set(e, c, u);
	Camera.Render();

	mBackGlound.Render(mBackGroundMatrix);

	//タスクリストの削除
	CTaskManager::Get()->Delete();
	//描画
	CTaskManager::Get()->Render();

	CCollisionManager::Get()->Render();
}