#ifndef CCHARACTER_H
#define CCHARACTER_H
//変換行列クラスのインクルード
#include"CTransform.h"
//モデルクラスのインクルード
#include"CModel.h"
#include"CTask.h"
/*キャラクタークラス
　ゲームキャラクタの基本的な機能を定義する*/
class CCollider;

class CCharacter : public CTransform,public CTask{
public:
	enum ETag{
		EZERO,
		EPLAYER,
		EENEMY,
		EBULLETPLAYER,
		EBULLETENEMY,
		EBLOCK
	};
	ETag mTag;

	CModel *mpModel;	//モデルのポインタ

	//デフォルトコンストラクタ
	CCharacter();
	//デストラクタ
	~CCharacter();

	//描画処理
	void Render();

	virtual void Collision(CCollider *m, CCollider *o){}
};


#endif