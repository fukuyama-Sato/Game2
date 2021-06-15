#ifndef CEnemy_H
#define CEnemy_H
//キャラクタクラスのインクルード
#include"CCharacter.h"
//コライダクラスのインクルード
#include"CCollider.h"
#include"CVector.h"
/*エネミークラス
　キャラクタクラスを継承*/
class CEnemy : public CCharacter{
private:
	int mSpeedX;	//速度
	int mSpeedY;
	int mSpeedZ;
	float mMoveSpeedX;	//移動用
	float mMoveSpeedY;
	float mMoveSpeedZ;

	float dx;
	float dy;
	float dz;
	CVector vx;
	CVector vy;
	CVector vz;

	int mMoveControl;	//プレイヤーとの距離の判断

public:

	int mHp;	//ヒットポイント
	//モデルデータ
	static CModel mModel;
	//目標地点
	CVector mPoint;
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


	void MoveMane();

	//更新処理
	void Update();

	void NearMove();	//近距離用動作
	void MediumMove();	//中距離用動作
	void DistantMove();	//遠距離用動作

	//衝突処理
	//Collider(コライダ1,コライダ2)
	void Collision(CCollider *m, CCollider *o);
	void TaskCollision();
};

#endif