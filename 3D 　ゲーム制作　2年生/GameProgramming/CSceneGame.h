#ifndef CSCENEGAME_H
#define CSCENEGAME_H
#include"CScene.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CPlayer.h"
#include"CColliderTriangle.h"

/*
ゲームのシーン
*/
class CSceneGame : public CScene {
private:
	int camX;
	int camY;
	int camZ;

public:
	//モデルクラスのインスタンス作成
	CModel mModel;

	CPlayer mPlayer;

	//初期化処理のオーバーライド
	void Init();
	//更新処理のオーバーライド
	void Update();
};

#endif
