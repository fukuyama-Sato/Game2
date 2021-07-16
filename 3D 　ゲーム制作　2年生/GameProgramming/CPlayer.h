#ifndef CPLAYER_H
#define CPLAYER_H
//キャラクタークラスのインクルード
#include"CCharacter.h"
//弾クラスのインクルード
#include"CBullet.h"
#include"CColliderLine.h"
#include"CColliderTriangle.h"
#include"CEffect.h"
#include"CEnemy.h"
/*プレイヤークラス
　キャラクタクラスを継承*/
class CPlayer : public CCharacter{
private:
	bool mJump;
	int mJumpTimer;

	int mFireRate;	//連射速度
	int mBulletTortalNum;
	int mBulletNum;
	int mReloadTime;

	float mSpeedX;	//左右加速度
	float mSpeedY;	//上下加速度
	float mSpeedZ;	//前後加速度
	float mBeforMouseX;	//直前マウス位置
	float mBeforMouseY;
	float mMouseMoveX;	//マウス移動量
	float mMouseMoveY;
	float mMouseSpeedX;	//マウス感度
	float mMouseSpeedY;

public:

	int mPlayerHp;	//体力

	CColliderLine mLine; //線分コライダ
	CColliderLine mLine2; //線分コライダ
	CColliderLine mLine3; //線分コライダ
	CColliderLine mLine4; //線分コライダ
	CColliderLine mLine5; //線分コライダ
	CCollider mCollider; //本体用コライダ
	//CColliderTriangle mTriangleCol;

	CPlayer();
	//更新処理
	void Update();

	//衝突処理
	void Collision(CCollider *m, CCollider *o);

	//描画処理
	void Render();

	//衝突処理
	void TaskCollision();

};

#endif