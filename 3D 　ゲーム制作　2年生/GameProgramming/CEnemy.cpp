#include"CEnemy.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CEffect.h"
#include"CBullet.h"

#define OBJ "Character.obj"	//モデルのファイル
#define MTL "Character.mtl"	//モデルのマテリアルファイル
#define EHP 20	//耐久値
#define VELOCITYX 0.2f	//X速度
#define VELOCITYY 0.2f	//Y速度
#define VELOCITYZ 0.2f	//Z速度
#define GLAVITY -0.02f //重力

#define JUMPPOWER 1	//ジャンプ力

#define FIRERATE 6	//攻撃の連射速度
#define BULLETNUMBER 50	//装弾数
#define BULLETTORTALNUM 600
#define RELOAD 260

CModel CEnemy::mModel;	//モデルデータ作成
int CEnemy::mHp = EHP;

//デフォルトコンストラクタ
CEnemy::CEnemy()
:mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 2.0f)
, mColSearch1(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 200.0f)
, mpPlayer(0)
, mEFireRate(0)
, mMoveSpeedX(0)
, mMoveSpeedY(0)
, mMoveSpeedZ(0)
, mEBulletTortalNum(BULLETTORTALNUM)
, mEBulletNum(BULLETNUMBER)
, mEReloadTime(0)
, mEJump(true)
{
	//モデルが無いときは読み込む
	if (mModel.mTriangles.size() == 0){
		mModel.Load(OBJ, MTL);
	}
	//モデルのポインタ設定
	mpModel = &mModel;
	mColSearch1.mTag = CCollider::ESEARCH;
	mCollider.mTag = CCollider::EBODY;
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

//移動関係
void CEnemy::Move(){
	//左向き(X軸)のベクトルを求める
	CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
	//上向き(Y軸)のベクトルを求める
	CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
	//前方向(Z軸)のベクトルを求める
	CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;

		CVector vp = mpPlayer->mPosition - mPosition;
		float dx = vp.Dot(vx);	//左ベクトルとの内積を求める
		float dy = vp.Dot(vy);	//上ベクトルとの内積を求める
		float dz = vp.Dot(vz);	//前ベクトルとの内積を求める

		if (-15.0f < dx && dx > 15.0f){
			if (-15.0f < dz && dz > 15.0f){
				mMoveControl = 2;
			}
		}

		//近
		if (mMoveControl == 0){
				mMoveSpeedX = 1.1f;
		}
		//中
		if (mMoveControl == 1){
				mMoveSpeedZ = 1.1f;
			}
		//遠
		if (mMoveControl == 2){
				mMoveSpeedZ = -1.1f;
		}
}

void CEnemy::Update(){
	//重力
	mMoveSpeedY += GLAVITY;
	mPosition.mY += mMoveSpeedY;
	//疑似着地
	if (mPosition.mY < 1){
		mEJump = true;
		if (mPosition.mY < 0){
			mMoveSpeedY = 0.08;
		}
	}

	//リロード
	if (mEBulletNum <= 0){
		mEReloadTime = RELOAD;
	}
	if (mEReloadTime > 0){
		mEBulletTortalNum -= BULLETNUMBER - mEBulletNum;
		mEBulletNum = BULLETNUMBER;
	}
	if (mEReloadTime > -1){
		mEReloadTime--;
	}
	if (mEFireRate >= 0)mEFireRate--;

	if (CEnemy::mHp > 0){
		//およそ1.5秒毎に目標地点を更新
		int r = rand() % 100;	//rand()は整数の乱数を返す
		//100で割った余りを求める
		if (r == 0){
			if (mpPlayer){
				mPoint = mpPlayer->mPosition;
			}
			else{
				mPoint = mPoint * CMatrix().RotateY(45);
			}
		}
		//ジャンプ
		if (mEJump == true && r == 98){
			mMoveSpeedY += JUMPPOWER;
			mEJump = false;
		}

		//左向き(X軸)のベクトルを求める
		CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
		//上向き(Y軸)のベクトルを求める
		CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
		//前方向(Z軸)のベクトルを求める
		CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;

		if (mpPlayer){
			Move();
			CVector vp = mpPlayer->mPosition - mPosition;
			float dx = vp.Dot(vx);	//左ベクトルとの内積を求める
			float dy = vp.Dot(vy);	//上ベクトルとの内積を求める
			float dz = vp.Dot(vz);	//前ベクトルとの内積を求める

			if (-10.0f < dx && dx < 10.0f){
				if (-5.0f < dy && dy < 5.0f){
					if (dz > 0.0f){

							if (mEBulletTortalNum > 0){
								if (mEFireRate < 0 && mEBulletNum > 0 && mEReloadTime < 0){
									//弾を発射する
									CBullet *bullet = new CBullet();
									bullet->mTag = CCharacter::EBULLETENEMY;
									bullet->Set(0.1f, 1.5f);
									bullet->mPosition = CVector(-3.0f, 2.5f, 13.0f) * mMatrix;
									bullet->mRotation = mRotation;
									bullet->Update();
									mEBulletNum--;
									mEFireRate = FIRERATE;
								}
							}

					}
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

	}

	else{
		//死亡時爆発エフェクト
		if (mHp > -4)
		new CEffect(mPosition, 15.0f, 15.0f, "exp.tga", 4, 4, 2);
		mHp--;
		if (mHp % 25 == 0){
			//エフェクト生成
			new CEffect(mPosition, 2.0f, 2.0f, "exp.tga", 4, 4, 2);
		}
	}

	//移動
	mPosition = mPosition + CVector(mMoveSpeedX, 0.0f, mMoveSpeedZ) * mMatrixRotate;
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

	//相手のコライダタイプで判定
	switch (o->mType){
	case CCollider::ESPHERE: //球コライダ
		if (m->mTag == CCollider::EBODY){
		//コライダのmとoが衝突しているか判定
		if (CCollider::Collision(m, o)){
			if (o->mpParent->mTag == EBULLETPLAYER){
					mHp --; //HP減算
					//エフェクト生成
					new CEffect(o->mpParent->mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
				}
			}
		}
		break;
		
	case CCollider::ETRIANGLE: //三角コライダ
		CVector adjust; //調整値
		//三角コライダと球コライダの判定
		if (CCollider::CollisionTriangleSphere(o, m, &adjust)){
			//衝突しない位置まで戻す
			mPosition = mPosition + adjust;
			break;
		}
		
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