#include"CEnemy.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CEffect.h"
#include"CBullet.h"

#define OBJ "f16.obj"	//モデルのファイル
#define MTL "f16.mtl"	//モデルのマテリアルファイル
#define HP 3	//耐久値
#define VELOCITY 0.11f	//速度

CModel CEnemy::mModel;	//モデルデータ作成

//デフォルトコンストラクタ
CEnemy::CEnemy()
:mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.4f)
, mColSearch1(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 30.0f)
, mColSearch2(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 10.0f)
, mpPlayer(0)
, mHp(HP)
{
	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0){
		mModel.Load(OBJ, MTL);
	}
	//モデルのポインタ設定
	mpModel = &mModel;
	mColSearch1.mTag = CCollider::ESEARCH1;
	mColSearch2.mTag = CCollider::ESEARCH2;
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
}

void CEnemy::Update(){
	//HPが0以下の時 撃破
	if (mHp <= 0){
		mHp--;
		//15フレームごとにエフェクト
		if (mHp % 15 == 0){
			//エフェクト生成
			new CEffect(mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		}
		//下降させる
		mPosition.mY -= 0.03f;
		CTransform::Update();
		return;
	}
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
	//左向き(X軸)のベクトルを求める
	CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
	//上向き(Y軸)のベクトルを求める
	CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
	//前方向(Z軸)のベクトルを求める
	CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;
	//プレイヤーのポイントが0以外の時
	if (mpPlayer){
		//プレイヤーまでのベクトルを求める
		CVector vp = mpPlayer->mPosition - mPosition;
		float dx = vp.Dot(vx);	//左ベクトルとの内積を求める
		float dy = vp.Dot(vy);	//上ベクトルとの内積を求める
		float dz = vp.Dot(vz);	//前ベクトルとの内積を求める
		//X軸のずれが2.0以下
		if (-2.0f < dx && dx < 2.0f){
			//Y軸のずれが2.0以下
			if (-2.0f < dy && dy < 2.0f){
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
	}
	mpPlayer = 0;

	CVector vp = mPoint - mPosition;
	float dx = vp.Dot(vx); //左ベクトルとの内積
	float dy = vp.Dot(vy); //上ベクトルとの内積
	float margin = 0.1f;
	//左右方向へ回転
	if (dx > margin){
		mRotation.mY += 1.0f;
	}
	else if (dx < -margin){
		mRotation.mY -= 1.0f;
	}
	//上下方向へ回転
	if (dy > margin){
		mRotation.mX -= 1.0f;
	}
	else if (dy < -margin){
		mRotation.mX += 1.0f;
	}

	//移動
	mPosition = mPosition + CVector(0.0f, 0.0f, VELOCITY) * mMatrixRotate;
	CTransform::Update();	//行列更新
}

//衝突処理
//Collision(コライダ1,コライダ2)
void CEnemy::Collision(CCollider *m, CCollider *o){
	//相手がサーチの時は戻る
	if (o->mTag == CCollider::ESEARCH1){
		return;
	}
	//自分がサーチ用の時
	if (m->mTag == CCollider::ESEARCH1){
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
			mHp--; //HP減算
			//エフェクト生成
			new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		}
		break;
	case CCollider::ETRIANGLE: //三角コライダ
		CVector adjust; //調整値
		//三角コライダと球コライダの判定
		if (CCollider::CollisionTriangleSphere(o, m, &adjust)){
			//撃破で地面に衝突すると無効
			if (mHp <= 0){
				mEnabled = false;
			}
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
	mColSearch2.ChangePriority();
	//衝突処理 実行
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSearch1, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSearch2, COLLISIONRANGE);
}