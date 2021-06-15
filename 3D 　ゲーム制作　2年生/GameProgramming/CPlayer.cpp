//プレイヤークラスのインクルード
#include"CPlayer.h"
#include"CKey.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include"CText.h"
#include"CUtil.h"
#include"CEnemy.h"
#include"CInput.h"

#define GLAVITY -0.02f //重力
#define JUMPPOWER 1	//ジャンプ力
#define VELOCITY 0.2f	//移動力
#define SPEEDREMIT 2	//速度制限
#define SUBERI 2	//滑り易さ
#define FIRERATE 6	//攻撃の連射速度
#define BULLETNUMBER 50	//装弾数
#define RELOAD 150

CText mText;
CEnemy Enemy;

CPlayer::CPlayer()
: mLine(this, &mMatrix, CVector(0.0f, 0.0f, -14.0f), CVector(0.0f, 0.0f, 17.0f))
, mLine2(this, &mMatrix, CVector(0.0f, 3.0f, -8.0f), CVector(0.0f, -2.0f, -8.0f))
, mLine3(this, &mMatrix, CVector(9.0f, 0.0f, -8.0f), CVector(-9.0f, 0.0f, -8.0f))
, mCollider(this, &mMatrix, CVector(0.0f,0.0f,0.0f),1.5f)
{
	mText.LoadTexture("FontWhite.tga", 1, 64);
	mTag = EPLAYER; //タグの設定
	mSpeedX = NULL;
	mSpeedY = NULL;
	mSpeedZ = NULL;
	mJump = true;
	mFireRate = 0;
	mHp = 50;

	mBeforMouseX = 0;
	mBeforMouseY = 0;
	mMouseMoveX = 0;
	mMouseMoveY = 0;

	mBulletTortalNum = 600;
	mBulletNum = BULLETNUMBER;
	mReloadTime = 0;
}

//更新処理
void CPlayer::Update(){
  //マウス設定
	float mMousePosX, mMousePosY;	//マウスカーソル座標取得用
	//マウスカーソル座標の取得
	CInput::GetMousePos(&mMousePosX, &mMousePosY);

	//ゲーム画面中心からの座標へ変換
	mMousePosX -= 400;
	mMousePosY = 300 - mMousePosY;

	if (CKey::Push('C')){
		//マウス座標コンソール出力
		printf("%d,%d\n", mMousePosX, mMousePosY);
	}

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



	//ここからマウスによる操作
	mMouseMoveX = mMousePosX - mBeforMouseX;
	mMouseMoveY = mMousePosY - mBeforMouseY;
	//視点操作
	//上下
	//if (mRotation.mX < 90 && mRotation.mX > -90){
	//if (mMousePosX > -400 && mMousePosX < 400 && mMousePosY > -300 && mMousePosY < 300){
		mRotation.mX -= mMouseMoveY/1.5;
		mRotation.mY -= mMouseMoveX/1.5;
	//}
	//}
	

	//左クリックで弾を発射
		if (mBulletTortalNum > 0){
			if (CKey::Push(VK_LBUTTON) && mFireRate-- < 0 && mBulletNum > 0 && mReloadTime < 0){
				CBullet *bullet = new CBullet();
				bullet->mTag = CCharacter::EBULLETPLAYER;
				bullet->Set(1.1f, 2.8f);
				bullet->mPosition = CVector(-2.0f, -1.0f, 10.0f) * mMatrix;
				bullet->mRotation = mRotation;
				bullet->Update();
				mBulletNum--;
				mFireRate = FIRERATE;
			}
			//リロード
			if (CKey::Once('R') || mBulletNum <= 0){
				mReloadTime = RELOAD;
			}
			if (mReloadTime-- > 0){
					mBulletTortalNum -= BULLETNUMBER - mBulletNum;
					mBulletNum = BULLETNUMBER;
			}
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

	mBeforMouseX = mMousePosX;
	mBeforMouseY = mMousePosY;
}

void CPlayer::Collision(CCollider *m, CCollider *o){
	//自身のコライダタイプの判定
	switch (m->mType){
	case CCollider::ELINE: //線分コライダ
		if (o->mType == CCollider::EBLOCK){
			CVector adjust; //	調整用ベクトル
			//三角形と線分の衝突判定
			CCollider::CollisionTriangleLine(o, m, &adjust);
			//位置の更新(mPosition + adjust)
			mPosition = mPosition - adjust * -1;
			CTransform::Update();
		}
		break;

		//相手のコライダが三角コライダの時
		if (o->mType == CCollider::ETRIANGLE){
			CVector adjust; //	調整用ベクトル
			//三角形と線分の衝突判定
			CCollider::CollisionTriangleLine(o, m, &adjust);
			//位置の更新(mPosition + adjust)
			mPosition = mPosition - adjust * -1;
			CTransform::Update();
		}
		break;
	}
}

void CPlayer::Render(){
	//親の描画処理
	CCharacter::Render();

	//2Dの描画開始
	CUtil::Start2D(-400, 400, -300, 300);
	//描画色の設定
	glColor4f(0.1f, 0.8f, 0.1f, 1.0f);
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

	//文字列の設定
	sprintf(buf, "HP:%d", mHp);
	//文字列の描画
	mText.DrawString(buf, -300, 50, 8, 16);

	//文字列の設定
	sprintf(buf, "EHP:%d", Enemy.mHp);
	//文字列の描画
	mText.DrawString(buf, -300, 0, 8, 16);

	//文字列の設定
	sprintf(buf, "+");
	//文字列の描画
	mText.DrawString(buf, 0, 20, 20, 20);

	//文字列の設定
	sprintf(buf, "BULLET:%d",mBulletNum);
	//文字列の描画
	mText.DrawString(buf, -300, -100, 8, 16);

	//文字列の設定
	sprintf(buf, "%d", mBulletTortalNum);
	//文字列の描画
	mText.DrawString(buf, -300, -170, 8, 16);


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