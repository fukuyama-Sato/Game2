#include"CTaskManager.h"

//タスクマネージャのインスタンス
CTaskManager* CTaskManager::mpInstance = 0;

//インスタンスの取得
CTaskManager* CTaskManager::Get(){
	//インスタンスが無ければ
	if (mpInstance == 0){
		//インスタンスを作成
		mpInstance = new CTaskManager();
	}
	return mpInstance;
}

//デフォルトコンストラクタ
CTaskManager::CTaskManager()
{
	mHead.mpNext = &mTail;
	mTail.mpPrev = &mHead;
}

CTaskManager::~CTaskManager(){
}

//リストに追加
//Add(タスクのポイント)
void CTaskManager::Add(CTask *addTask){
	//mHeadの次から検索
	CTask *task = mHead.mpNext;

	//優先度の高い順に挿入
	//挿入位置の検索(優先度が同じ、または高くなった前)
	//mPriority>=0の事
	while(addTask->mPriority < task->mPriority){
		task = task->mpNext; //次へ
	}

	//addTaskの次をtask
	addTask->mpNext = task;
	//addTaskの前をtaskの前に
	addTask->mpPrev = task->mpPrev;
	//addTaskの前の次をaddTaskに
	addTask->mpPrev->mpNext = addTask;
	//taskの前をaddTaskに
	task->mpPrev = addTask;
}

//更新
void CTaskManager::Update(){
	//先頭から最後まで繰り返し
	CTask *task = mHead.mpNext;
	while (task->mpNext){
		//更新処理を呼ぶ
		task->Update();
		//次へ
		task = task->mpNext;
	}
}

//描画
void CTaskManager::Render(){
	//先頭から最後まで繰り返し
	CTask *task = mHead.mpNext;
	while (task->mpNext){
		//描画処理を呼ぶ
		task->Render();
		//次へ
		task = task->mpNext;
	}
}

void CTaskManager::TaskCollision(){
	//先頭から最後まで繰り返し
	CTask *task = mHead.mpNext;
	while (task->mpNext){
		//衝突処理を呼ぶ
		task->TaskCollision();
		//次へ
		task = task->mpNext;
	}
}



//リストから削除
//Remove(タスクのポインタ)
void CTaskManager::Remove(CTask *task){
	//タスクの前の次を,タスクの次にする
	task->mpPrev->mpNext = task->mpNext;
	//タスクの次の前を、タスクの前にする
	task->mpNext->mpPrev = task->mpPrev;
}

//タスクの削除
void CTaskManager::Delete(){
	//先頭から最後まで繰り返し
	CTask *task = mHead.mpNext;
	while (task->mpNext){
		CTask *del = task;
		//次へ
		task = task->mpNext;
		//mEnabledがfalseなら削除
		if (del->mEnabled == false){
			delete del;
		}
	}
}