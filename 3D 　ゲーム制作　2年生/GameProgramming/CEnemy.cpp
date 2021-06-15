#include"CEnemy.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CEffect.h"
#include"CBullet.h"

#define OBJ "f16.obj"	//モデルのファイル
#define MTL "f16.mtl"	//モデルのマテリアルファイル
#define HP 300	//耐久値
#define VELOCITYX 0.2f	//X速度
#define VELOCITYY 0.2f	//Y速度
#define VELOCITYZ 0.2f	//Z速度

CModel CEnemy::mModel;	//モデルデータ作成

//デフォルトコンストラクタ
CEnemy::CEnemy()
:mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)
, mColSearch1(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 70.0f)
, mpPlayer(0)
, mHp(HP)
, mMoveSpeedX(0)
, mMoveSpeedY(0)
, mMoveSpeedZ(0)
{
	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0){
		mModel.Load(OBJ, MTL);
	}
	//モデルのポインタ設定
	mpModel = &mModel;
	mColSearch1.mTag = CCollider::ESEARCH;
	//目標地点の設定
	mPoint = mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
}

//CEnemy(位置,回転,拡張)
CEnemy::CEnemy(const CVector& position, const CVector& rotation, const CVector& scale)
: CEnemy()
{
	//位置,回転,拡縮を設定
	mPosition = position;	//位置
	mRotation = rotation;	//回転
	mScale = scale;			//拡縮
	CTransform::Update();	//行列
	//優先度を1
	mPriority = 1;
	CTaskManager::Get()->Remove(this);	//削除
	CTaskManager::Get()->Add(this);		//追加

	mMoveControl = 0;
}

void CEnemy::MoveMane(){
	//左向き(X軸)のベクトルを求める
	vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
	//上向き(Y軸)のベクトルを求める
	vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
	//前方向(Z軸)のベクトルを求める
	vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;

	CVector vp = mpPlayer->mPosition - mPosition;
	dx = vp.Dot(vx);	//左ベクトルとの内積を求める
	dy = vp.Dot(vy);	//上ベクトルとの内積を求める
	dz = vp.Dot(vz);	//前ベクトルとの内積を求める

	//X軸のずれが10.0以下
	if (-50.0f > dx && dx > 50.0f){
		//Y軸のずれが5.0以下
		if (-30.0f > dy && dy > 30.0f){
			if (-50.0f > dz && dz > 50.0f){
				mMoveControl = 2;
			}
		}
	}

	//X軸のずれが10.0以上,50以下
	else if (-10.0f > dx && dx > 10.0f && -50.0f < dx && dx < 50.0f){
		//Y軸のずれが10以上,30以下
		if (-10.0f > dy && dy > 10.0f && -30.0f < dy && dy < 30.0f){
			if (-10.0f > dz && dz > 10.0f && -50.0f < dz && dz < 50.0f){
				mMoveControl = 1;
			}
		}
	}

	//X軸のずれが10.0以下
	else if (-10.0f < dx && dx < 10.0f){
		//Y軸のずれが5.0以下
		if (-10.0f < dy && dy < 10.0f){
			if (-10.0f < dz && dz < 10.0f){
				mMoveControl = 0;
			}
		}
	}

}

//近距離
void CEnemy::NearMove(){
	if (mMoveControl == 0){
		mMoveSpeedZ++;
	}
}

//中距離
void CEnemy::MediumMove(){
	if (mMoveControl == 1){
		mMoveSpeedY++;
	}
}

//遠距離
void CEnemy::DistantMove(){
	if (mMoveControl == 2){
		mMoveSpeedX++;
	}
}


void CEnemy::Update(){

	//およそ3秒毎に目標地点を更新
	int r = rand() % 180;	//rand()は整数の乱数を返す
	//%180 は 180で割った余りを求める
	if (r == 0){
		if (mpPlayer){
			mPoint = mpPlayer->mPosition;
		}
		else{
			mPoint = mPoint * CMatrix().RotateY(45);
		}
	}

	if (-10.0f < dx && dx < 10.0f){
		if (-5.0f < dy && dy < 5.0f){
			if (dz > 0.0f){
				//弾を発射する
				CBullet *bullet = new CBullet();
				bullet->Set(0.1f, 1.5f);
				bullet->mPosition = CVector(0.0f, 0.0f, 10.0f) * mMatrix;
				bullet->mRotation = mRotation;
				bullet->Update();
			}
		}
	}

	mpPlayer = 0;
	CVector vp = mPoint - mPosition;
	float dx = vp.Dot(vx); //左ベクトルとの内積
	float dy = vp.Dot(vy); //上ベクトルとの内積
	float margin = 1.0f;

		//左右方向へ回転
		if (dx > margin){
			mRotation.mY += 4.0f;
		}
		else if (dx < -margin){
			mRotation.mY -= 4.0f;
		}

		//上下方向へ回転
		if (dy > margin){
			mRotation.mX -= 2.0f;
		}
		else if (dy < -margin){
			mRotation.mX += 2.0f;
		}

	//移動
	mPosition = mPosition + CVector(mMoveSpeedX, mMoveSpeedY, mMoveSpeedZ) * mMatrixRotate;
	CTransform::Update();	//行列更新
}

//衝突処理
//Collision(コライダ1,コライダ2)
void CEnemy::Collision(CCollider *m, CCollider *o){
	if (o->mTag == CCollider::ESEARCH){
		return;
	}

	//自分がサーチ用の時
	if (m->mTag == CCollider::ESEARCH){
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
	}
	//相手のコライダタイプの判定
	switch (o->mType){
	case CCollider::ESPHERE: //球コライダ
		if (o->mpParent->mTag == EBULLETPLAYER){
		//コライダのmとoが衝突しているか判定
			if (CCollider::Collision(m, o)){
				mHp--; //HP減算
				//エフェクト生成
				new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			}
		}
		break;
	case CCollider::ETRIANGLE: //三角コライダ
		CVector adjust; //調整値
		//三角コライダと球コライダの判定
		if (CCollider::CollisionTriangleSphere(o, m, &adjust)){
			//衝突しない位置まで戻す
			mPosition = mPosition + adjust;
		}
		break;
	}

}



void CEnemy::TaskCollision(){
	//コライダの優先度変更
	mCollider.ChangePriority();
	mColSearch1.ChangePriority();
	//衝突処理 実行
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSearch1, COLLISIONRANGE);
}