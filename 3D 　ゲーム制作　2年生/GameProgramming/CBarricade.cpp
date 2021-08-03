#include"CBarricade.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CEffect.h"

CModel CBarricade::mModel;

CBarricade::CBarricade(CModel *model, CVector position, CVector rotation, CVector scale)
:mCollider1a(this, &mMatrix,
CVector(10.0f, 0.0f, 1.5f),
CVector(9.0f, 7.0f, 0.8f),
CVector(-9.0f, 7.0f, 0.8f))
, mCollider1b(this, &mMatrix,
CVector(-9.0f, 7.0f, 0.8f),
CVector(-10.0f, 0.0f, 1.5f),
CVector(10.0f, 0.0f, 1.5f))

, mCollider2a(this, &mMatrix,
CVector(-9.0f, 7.0f, -0.8f),
CVector(9.0f, 7.0f, -0.8f),
CVector(10.0f, 0.0f, -1.5f))

, mCollider2b(this, &mMatrix,
CVector(10.0f, 0.0f, -1.5f),
CVector(-10.0f, 0.0f, -1.5f),
CVector(-9.0f, 7.0f, -0.8f))

{
	//モデル,位置,回転,拡縮を設定
	mpModel = model;		//モデルの設定
	mPosition = position;	//位置の設定
	mRotation = rotation;	//回転の設定
	mScale = scale;			//拡縮の設定

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


	if (o->mEnabled == CCollider::ESPHERE){ //球コライダ
		if (o->mTag == CCharacter::EBULLETPLAYER || o->mTag == CCharacter::EBULLETENEMY){
			//コライダのmとoが衝突しているか判定
			if (CCollider::Collision(m, o)){
				//エフェクト生成
				new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			}
		}
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