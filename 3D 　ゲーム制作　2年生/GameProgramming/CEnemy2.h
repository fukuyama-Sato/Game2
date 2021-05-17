#ifndef CENEMY2_H
#define CENEMY2_H
//キャラクタクラスのインクルード
#include"CCharacter.h"
//コライダクラスのインクルード
#include"CCollider.h"
#include"CVector.h"
/*エネミークラス
　キャラクタクラスを継承*/
class CEnemy2 : public CCharacter{
public:

	int mHp;	//ヒットポイント
	//モデルデータ
	static CModel mModel;
	//目標地点
	CVector mPoint;
	//コライダ
	CCollider mCollider;
	//サーチ用コライダ 
	CCollider mColSearch;
	//プレイヤーのポインタ
	CCharacter *mpPlayer;
	//コンストラクタ
	CEnemy2();
	//CEnemy(位置,回転,拡張)
	CEnemy2(const CVector& position, const CVector& rotation,
		const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collider(コライダ1,コライダ2)
	void Collision(CCollider *m, CCollider *o);
	void TaskCollision();
};

#endif