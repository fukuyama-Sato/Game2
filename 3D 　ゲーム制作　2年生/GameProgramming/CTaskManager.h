#ifndef CTASKMANAGER_H
#define CTASKMANAGER_H

#include"CTask.h"

/*タスクマネージャー
　タスクリストの管理*/
class CTaskManager{
private:
	//タスクマネージャのインスタンス
	static CTaskManager* mpInstance;

protected:
	//デフォルトコンストラクタ
	CTaskManager();

public:
	//インスタンスの取得
	static CTaskManager* Get();

	CTask mHead;//先頭タスク
	CTask mTail;//最終タスク

	//デストラクタ
	virtual ~CTaskManager();

	//リストに追加
	//Add(タスクのポインタ)
	void Add(CTask *addTask);
	//更新
	void Update();
	//描画
	void Render();
	//衝突処理
	void TaskCollision();

	//リストから削除
	//Remove(タスクのポインタ)
	void Remove(CTask *task);
	//タスクの削除
	void Delete();
};

#endif