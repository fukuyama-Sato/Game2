#ifndef CPLAYER_H
#define CPLAYER_H
//キャラクタークラスのインクルード
#include"CCharacter.h"
//弾クラスのインクルード
#include"CBullet.h"
#include"CColliderLine.h"
/*プレイヤークラス
　キャラクタクラスを継承*/
class CPlayer : public CCharacter{
private:
	int mJump;
	int mHp;	//体力
	int mFireRate;
	float mSpeedX;	//左右加速度
	float mSpeedY;	//上下加速度
	float mSpeedZ;	//前後加速度
	float mBeforMouseX;
	float mBeforMouseY;
	float mMouseMoveX;
	float mMouseMoveY;

public:
	CColliderLine mLine; //線分コライダ
	CColliderLine mLine2; //線分コライダ
	CColliderLine mLine3; //線分コライダ
	CCollider mCollider; //本体用コライダ

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