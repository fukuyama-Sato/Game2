//プレイヤークラスのインクルード
#include"CPlayer.h"
#include"CKey.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CText.h"
#include"CUtil.h"

#define GLAVITY -0.05 //重力
#define JUMPPOWER 2	//ジャンプ力
#define VELOCITY 0.8	//移動力
#define SPEEDREMIT 5	//速度制限
#define SUBERI 2	//滑り易さ

CText mText;

CPlayer::CPlayer()
: mLine(this, &mMatrix, CVector(0.0f, 0.0f, -14.0f), CVector(0.0f, 0.0f, 17.0f))
, mLine2(this, &mMatrix, CVector(0.0f, 3.0f, -8.0f), CVector(0.0f, -2.0f, -8.0f))
, mLine3(this, &mMatrix, CVector(9.0f, 0.0f, -8.0f), CVector(-9.0f, 0.0f, -8.0f))
, mCollider(this, &mMatrix, CVector(0.0f,0.0f,0.0f),0.5f)
{
	mText.LoadTexture("FontWhite.tga", 1, 64);
	mTag = EPLAYER; //タグの設定
	mSpeedX = NULL;
	mSpeedY = NULL;
	mSpeedZ = NULL;
	mJump = true;
}

//更新処理
void CPlayer::Update(){
	//スペースキーでジャンプ
	if (CKey::Once(VK_SPACE) && mJump == true){
		mSpeedY += JUMPPOWER;
		mJump = false;
	}
	mSpeedY += GLAVITY;
	mPosition.mY += mSpeedY;


	//CTransformの更新
	CTransform::Update();

	//移動
	if (CKey::Push('A') && mSpeedX < SPEEDREMIT){
		//X軸の+移動
		mSpeedX += VELOCITY;
	}
	if (CKey::Push('D') && mSpeedX > -SPEEDREMIT){
		//X軸の-移動
		mSpeedX -= VELOCITY;
	}
	if (CKey::Push('S') && mSpeedZ > -SPEEDREMIT){
		//Z軸の-移動
		mSpeedZ -= VELOCITY;
	}
	if (CKey::Push('W')&& mSpeedZ < SPEEDREMIT){
		//Z軸の+移動
		mSpeedZ += VELOCITY + 0.2;
	}

	//ここからマウスの操作
	//視点操作
	if (CKey::Push(VK_UP) && mRotation.mX > -90){
		mRotation.mX -= 2;
	}
	if (CKey::Push(VK_DOWN) && mRotation.mX < 86){
		mRotation.mX += 2;
	}
	if (CKey::Push(VK_RIGHT)){
		mRotation.mY -= 2;
	}
	if (CKey::Push(VK_LEFT)){
		mRotation.mY += 2;
	}

	//Jキーで弾を発射
	if (CKey::Push('J')){
		CBullet *bullet = new CBullet();
		bullet->Set(0.1f, 0.8f);
		bullet->mPosition = CVector(-5.0f, -1.0f, 10.0f) * mMatrix;
		bullet->mRotation = mRotation;
		bullet->Update();
	}
	//ここまでマウスの操作

	//位置の移動
	mPosition = CVector(mSpeedX,0.0f,mSpeedZ) * mMatrix;


	//疑似着地
	if (mPosition.mY < 1){
		mPosition.mY = 1;
		mSpeedY = 0;
		mJump = true;
		//慣性擬き
		//X
		if (mSpeedX >= 0.01){
			mSpeedX -= VELOCITY / SUBERI;
		}
		else if (mSpeedX <= -0.01){
			mSpeedX += VELOCITY / SUBERI;
		}
		//Z
		if (mSpeedZ >= 0.01){
			mSpeedZ -= VELOCITY / SUBERI;
		}
		else if (mSpeedZ <= -0.01){
			mSpeedZ += VELOCITY / SUBERI;
		}
	}

}

void CPlayer::Collision(CCollider *m, CCollider *o){
	//自身のコライダタイプの判定
	//switch (m->mType){
	//case CCollider::ELINE: //線分コライダ
		//if (o->mTag == CCollider::EBLOCK){
		//	CVector adjust; //	調整用ベクトル
		//	//三角形と線分の衝突判定
		//	CCollider::CollisionTriangleLine(o, m, &adjust);
		//	//位置の更新(mPosition + adjust)
		//	mPosition = mPosition - adjust * -1;
		//	CTransform::Update();
		//}
		//break;

		////相手のコライダが三角コライダの時
		//if (o->mType == CCollider::ETRIANGLE){
		//	CVector adjust; //	調整用ベクトル
		//	//三角形と線分の衝突判定
		//	CCollider::CollisionTriangleLine(o, m, &adjust);
		//	//位置の更新(mPosition + adjust)
		//	mPosition = mPosition - adjust * -1;
		//	CTransform::Update();
		//}
		//break;
	//}
}

void CPlayer::Render(){
	//親の描画処理
	CCharacter::Render();

	//2Dの描画開始
	CUtil::Start2D(-400, 400, -300, 300);
	//描画色の設定
	glColor4f(0.1f, 0.1f, 0.1f, 0.7f);
	//文字列編集エリアの作成
	char buf[64];

	//Y座標の表示
	//文字列の設定
	sprintf(buf, "PY:%7.2f", mPosition.mY);
	//文字列の描画
	mText.DrawString(buf, 100, 30, 8, 16);

	//X軸回転値の表示
	//文字列の設定
	sprintf(buf, "RX:%7.2f", mRotation.mX);
	//文字列の描画
	mText.DrawString(buf, 100, 0, 8, 16);

	//Y軸回転値の表示
	//文字列の設定
	sprintf(buf, "RY:%7.2f", mRotation.mY);
	//文字列の描画
	mText.DrawString(buf, 100, -100, 8, 16);

	//速度表示
	//文字列の設定
	sprintf(buf, "VX:%f", mSpeedX);
	//文字列の描画
	mText.DrawString(buf, -300, 200, 8, 16);

	//文字列の設定
	sprintf(buf, "VY:%f", mSpeedY);
	//文字列の描画
	mText.DrawString(buf, -300, 150, 8, 16);

	//文字列の設定
	sprintf(buf, "VZ:%f", mSpeedZ);
	//文字列の描画
	mText.DrawString(buf, -300, 100, 8, 16);


	//2D描画終了
	CUtil::End2D();
}

//衝突処理
void CPlayer::TaskCollision(){
	//コライダの優先度変更
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	mCollider.ChangePriority();
	//衝突処理 実行
	CCollisionManager::Get()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine3, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}