#include"CSphere.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CEffect.h"

CSphere::CSphere(CModel *model, CVector position, CVector rotation, CVector scale)
:mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 10.0f)
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

//更新処理
void CSphere::Update(){
	//行列を更新
	CTransform::Update();
}

//衝突処理
//Collision(コライダ1,コライダ2)
void CSphere::Collision(CCollider *m, CCollider *o){
	//相手がサーチの時は戻る
	if (o->mTag == CCollider::ESEARCH){
		return;
	}
	//自分がサーチ用の時
	if (m->mTag == CCollider::ESEARCH){
		//相手が球コライダ
		if (o->mType == CCollider::ESPHERE){
			//相手がプレイヤー
			if (o->mpParent->mTag == EPLAYER){
				//衝突中
				if (CCollider::Collision(m, o)){
					//プレイヤーのポインタ設定
					mpPlayer = o->mpParent;
				}
			}
		}
		return;
	}
	//相手のコライダタイプの判定
	switch (o->mType){
	case CCollider::ESPHERE: //球コライダ
		//コライダのmとoが衝突しているか判定
		if (CCollider::Collision(m, o)){
			//エフェクト生成
			new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		}
		break;
	}
}

void CSphere::TaskCollision(){
	//コライダの優先度変更
	mCollider.ChangePriority();
	//衝突処理 実行
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}