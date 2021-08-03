#include"CEnemy.h"
#include"CSound.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CEffect.h"
#include"CBullet.h"

#define OBJ "Enemy.obj"	//モデルのファイル
#define MTL "Enemy.mtl"	//モデルのマテリアルファイル
#define VELOCITYX 0.5f	//X速度
#define VELOCITYY 0.5f	//Y速度
#define VELOCITYZ 0.5f	//Z速度
#define GLAVITY -0.02f //重力

#define JUMPPOWER 0.7f	//ジャンプ力

#define FIRERATE 6	//攻撃の連射速度
#define BULLETNUMBER 50	//装弾数
#define BULLETTORTALNUM 450
#define RELOAD 320

#define EHP 150	//耐久値

CModel CEnemy::mModel;	//モデルデータ作成
int CEnemy::mHp = EHP;
int CEnemy::mEBulletTortalNum = BULLETTORTALNUM;
int CEnemy::mEBulletNum = BULLETNUMBER;

extern CSound EnemyFire;
extern CSound EnemyJump;

extern CSound Hit;
extern CSound HP0;

//デフォルトコンストラクタ
CEnemy::CEnemy()
: mELine(this, &mMatrix, CVector(0.0f, 0.0f, -6.0f), CVector(0.0f, 0.0f, 8.0f))
, mELine2(this, &mMatrix, CVector(0.0f, 5.0f, 2.0f), CVector(0.0f, -5.0f, 2.0f))
, mELine3(this, &mMatrix, CVector(7.0f, 0.0f, 0.0f), CVector(-7.0f, 0.0f, 0.0f))
, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 4.0f), 5.0f)
, mColSearch1(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 150.0f)
, mpPlayer(0)
, mEFireRate(0)
, mMoveSpeedX(0)
, mMoveSpeedY(0)
, mMoveSpeedZ(0)
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


void CEnemy::Update(){
	//重力
	mMoveSpeedY += GLAVITY;
	mPosition.mY += mMoveSpeedY;
	//疑似着地
	if (mPosition.mY < 3.0f){
		mEJump = true;
		if (mPosition.mY < 0){
			mMoveSpeedY = 0.1;
		}
	}

	//自動減速
	if (mMoveSpeedX > 0){
		mMoveSpeedX -= 0.1f;
	}
	else if (mMoveSpeedX < 0){
		mMoveSpeedX += 0.1f;
	}
	if (mMoveSpeedZ > 0){
		mMoveSpeedZ -= 0.1f;
	}
	else if (mMoveSpeedZ < 0){
		mMoveSpeedZ += 0.1f;
	}

	if (CEnemy::mHp > 0){

	//移動
	mPosition = mPosition + CVector(mMoveSpeedX, 0.0f, mMoveSpeedZ) * mMatrixRotate;

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
	//発射間隔
	if (mEFireRate >= 0)mEFireRate--;


		int r = rand() % 100;	//rand()は整数の乱数を返す
		//100で割った余りを求める
		if (r > 60){
			if (mpPlayer){
				mPoint = mpPlayer->mPosition;
			}
		}

		//ジャンプ
		if (mEJump == true && r == 40){
			mMoveSpeedY = JUMPPOWER;
			EnemyJump.Play();
			mEJump = false;
		}

		//左向き(X軸)のベクトルを求める
		CVector vx = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
		//上向き(Y軸)のベクトルを求める
		CVector vy = CVector(0.0f, 1.0f, 0.0f) * mMatrixRotate;
		//前方向(Z軸)のベクトルを求める
		CVector vz = CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;

		if (mpPlayer){
			CVector vp = mpPlayer->mPosition - mPosition;
			float dx = vp.Dot(vx);	//左ベクトルとの内積を求める
			float dy = vp.Dot(vy);	//上ベクトルとの内積を求める
			float dz = vp.Dot(vz);	//前ベクトルとの内積を求める

			if (mHp > 106){
				if (mMoveSpeedX < 0.5)
				mMoveSpeedX += 0.2;
				if (mMoveSpeedZ > -0.2)
				mMoveSpeedZ += -0.1;
			}
			else if (mHp < 106 && mHp >= 82){
				if (mMoveSpeedX > -0.3f)
					mMoveSpeedX -= 0.2f;
				if (mMoveSpeedZ < 0.2f)
					mMoveSpeedZ += 0.04f;
			}
			else if(mHp < 82 && mHp >= 54){
				if (mMoveSpeedX > -0.5f)
				mMoveSpeedX -= 0.2f;
				if (mMoveSpeedZ < -0.1f)
				mMoveSpeedZ -= 0.01f;
			}
			else if (mHp < 54 && mHp >= 38){
				if (mMoveSpeedX > -0.2f)
					mMoveSpeedX -= 0.2f;
				if (mMoveSpeedZ < 0.1f)
					mMoveSpeedZ = 0.01f;
			}
			if (mHp < 38){
				if (mMoveSpeedX < 0.6f)
				mMoveSpeedX += 0.6f;
				if (mMoveSpeedZ > 0.4f)
				mMoveSpeedZ -= 0.2f;
			}


			if (-2.0f < dx && dx < 2.0f){
				if (-2.0f < dy && dy < 2.0f){
					if (dz > 0.0f){

							if (mEBulletTortalNum > 0){
								if (mEFireRate < 0 && mEBulletNum > 0 && mEReloadTime < 0){
									//弾を発射する
									CBullet *bullet = new CBullet();
									bullet->mTag = CCharacter::EBULLETENEMY;
									bullet->Set(0.5f, 2.5f);
									bullet->mPosition = CVector(-3.0f, 2.5f, 10.0f) * mMatrix;
									bullet->mRotation = mRotation;
									bullet->Update();
									EnemyFire.Play();
									mEBulletNum--;
									mEFireRate = FIRERATE;
								}
							}

					}
				}
			}

		}

		CVector vp = mPoint - mPosition;
		float dx = vp.Dot(vx); //左ベクトルとの内積
		float dy = vp.Dot(vy); //上ベクトルとの内積

		float margin = 0.1f;

		//左右方向へ回転
		if (dx > margin){
			mRotation.mY += 4.5f;
		}
		else if (dx < -margin){
			mRotation.mY -= 4.5f;
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
		if (mHp > -4){
			new CEffect(mPosition, 15.0f, 15.0f, "exp.tga", 4, 4, 2);
			HP0.Play();
		}
		mHp--;
		if (mHp % 25 == 0){
			//エフェクト生成
			new CEffect(mPosition, 8.0f, 8.0f, "exp.tga", 4, 4, 2);
		}
	}
	CTransform::Update();	//行列更新
}


//衝突処理
//Collision(コライダ1,コライダ2)
void CEnemy::Collision(CCollider *m, CCollider *o){

	CVector adjust; //調整値

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
					Hit.Play();
					//エフェクト生成
					new CEffect(o->mpParent->mPosition, 2.0f, 2.0f, "exp.tga", 4, 4, 2);
				}
			}
		}
		break;
		
	case CCollider::ETRIANGLE: //三角コライダ
		if (m->mType == CCollider::ESPHERE){
			CVector adjust; //調整値
			//三角コライダと球コライダの判定
			if (CCollider::CollisionTriangleSphere(o, m, &adjust)){
				//衝突しない位置まで戻す
				mPosition = mPosition + adjust;
				break;
			}
		}
	}

}

void CEnemy::TaskCollision(){
	//コライダの優先度変更
	mELine.ChangePriority();
	mELine2.ChangePriority();
	mELine3.ChangePriority();
	mCollider.ChangePriority();
	mColSearch1.ChangePriority();
	//衝突処理 実行
	CCollisionManager::Get()->Collision(&mELine, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mELine2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mELine3, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mColSearch1, COLLISIONRANGE);
}