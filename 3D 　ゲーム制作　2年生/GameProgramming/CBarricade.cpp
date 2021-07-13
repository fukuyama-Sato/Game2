#include"CBarricade.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CEffect.h"

#define OBJ "barricade.obj"
#define MTL "barricade.mtl"

CModel CBarricade::mModel;

CBarricade::CBarricade(CModel *model, CVector position, CVector rotation, CVector scale)
:mCollider1a(this, &mMatrix,
CVector(10.0f, 0.0f, 1.0f),
CVector(9.0f, 6.0f, 1.0f),
CVector(-9.0f, 6.0f, 1.0f))
, mCollider1b(this, &mMatrix,
CVector(-9.0f, 6.0f, 1.0f),
CVector(-10.0f, 0.0f, 1.0f),
CVector(10.0f, 0.0f, 1.0f))

, mCollider2a(this, &mMatrix,
CVector(-9.0f, 6.0f, -1.0f),
CVector(9.0f, 6.0f, -1.0f),
CVector(10.0f, 0.0f, -1.0f))

, mCollider2b(this, &mMatrix,
CVector(10.0f, 0.0f, -1.0f),
CVector(-10.0f, 0.0f, -1.0f),
CVector(-9.0f, 6.0f, -1.0f))
{
	//モデル,位置,回転,拡縮を設定
	mpModel = model;		//モデルの設定
	mPosition = position;	//位置の設定
	mRotation = rotation;	//回転の設定
	mScale = scale;			//拡縮の設定

	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0){
		mModel.Load(OBJ, MTL);
	}

	mTag = EBLOCK;

	mPriority = 1; //優先度1
	CTaskManager::Get()->Remove(this);
	CTaskManager::Get()->Add(this);

}

void CBarricade::Update(){
	CTransform::Update();
}

//衝突処理
//Collision(コライダ1,コライダ2)
void CBarricade::Collision(CCollider *m, CCollider *o){
	//相手がサーチの時は戻る
	if (o->mTag == CCollider::ESEARCH){
		return;
	}


		//相手が球コライダ
	if (o->mType == CCollider::ESPHERE){
		if (o->mpParent->mTag == EPLAYER){
			//衝突中
			if (CCollider::Collision(m, o)){
				//プレイヤーのポインタ設定
				mpPlayer = o->mpParent;
			}
		}
	}
		return;

	//相手のコライダタイプの判定
	switch (o->mType){
	case CCollider::ESPHERE: //球コライダ
		if (o->mTag == CCharacter::EBULLETPLAYER || o->mTag == CCharacter::EBULLETENEMY){
		//コライダのmとoが衝突しているか判定
			if (CCollider::Collision(m, o)){
				//エフェクト生成
				new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			}
		}
		break;
	}
}

void CBarricade::TaskCollision(){
	//コライダの優先度変更
	mCollider1a.ChangePriority();
	mCollider1b.ChangePriority();

	mCollider2a.ChangePriority();
	mCollider2b.ChangePriority();
	//衝突処理 実行
	CCollisionManager::Get()->Collision(&mCollider1a, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider1b, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider2a, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider2b, COLLISIONRANGE);
}