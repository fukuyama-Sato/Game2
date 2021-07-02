#ifndef CCLLIDER_H
#define CCLLIDER_H
#include "CCharacter.h"
#include "CTransform.h"
/*コライダクラス
衝突判定データ*/
class CCollider : public CTransform,public CTask{
public:
	CCharacter *mpParent;//親

	CMatrix *mpMatrix; //親行列

	//コライダタイプ
	enum EType{
		ESPHERE,//球コライダ
		ETRIANGLE,//三角コライダ
		ELINE	//線分コライダ
	};
	EType mType;// コライダタイプ

	enum ETag{
		EBODY,
		ESEARCH,
	};
	ETag mTag;	//コライダ識別

	//頂点
	CVector mV[3];
	//デフォルトコンストラクタ
	CCollider();

	float mRadius; //半径

	~CCollider();

	//コンストラクタ
	//CCollider(親,親行列,位置,半径)
	CCollider(CCharacter *parent, CMatrix *matrix, CVector position, float radius);

	//衝突判定
	//Collision(コライダ1,コライダ2)
	//return:true(衝突している)false(衝突していない)
	static bool Collision(CCollider *m, CCollider *o);

	//CollisionTriangleLine(三角コライダ,線分コライダ,調整値)
	//return : true(衝突している) false(衝突していない)
	//調整値 : 衝突しない位置まで戻す値
	static bool CollisionTriangleLine(CCollider *triangle, CCollider *line, CVector *adjust);

	//CollisionTriangleSphere(三角コライダ,球コライダ,調整値)
	//return:true(衝突している),falsse(衝突していない)
	//調整値:衝突しない位置まで戻す値
	static bool CollisionTriangleSphere(CCollider *triangle, CCollider *sphere, CVector *adjust);

	//優先度の変更
	void ChangePriority();

	//描画
	void Render();
};

#endif