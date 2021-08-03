#ifndef CEnemy_H
#define CEnemy_H
//キャラクタクラスのインクルード
#include"CCharacter.h"
//コライダクラスのインクルード
#include"CCollider.h"
#include"CVector.h"
#include"CColliderLine.h"
/*エネミークラス
　キャラクタクラスを継承*/
class CEnemy : public CCharacter{
private:
	int mEFireRate;	//連射速度
	
	int mEReloadTime;

	bool mEJump;

	int mEspeedX;	//速度
	int mEspeedY;
	int mEspeedZ;
	float mMoveSpeedX;	//移動用
	float mMoveSpeedY;
	float mMoveSpeedZ;

	int mMoveControl;	//プレイヤーとの距離の判断

public:

	static int mHp;	//ヒットポイント
	static int mEBulletTortalNum;
	static int mEBulletNum;
	//モデルデータ
	static CModel mModel;
	//目標地点
	CVector mPoint;

	CColliderLine mELine; //線分コライダ
	CColliderLine mELine2; //線分コライダ
	CColliderLine mELine3; //線分コライダ
	//コライダ
	CCollider mCollider;
	//サーチ用コライダ 
	CCollider mColSearch1;
	//プレイヤーのポインタ
	CCharacter *mpPlayer;
	//コンストラクタ
	CEnemy();
	//CEnemy(位置,回転,拡張)
	CEnemy(const CVector& position, const CVector& rotation,
		const CVector& scale);

	//更新処理
	void Update();

	//衝突処理
	//Collider(コライダ1,コライダ2)
	void Collision(CCollider *m, CCollider *o);
	void TaskCollision();
};

#endif