#ifndef CTASKMANAGER_H
#define CTASKMANAGER_H

#include"CTask.h"

/*�^�X�N�}�l�[�W���[
�@�^�X�N���X�g�̊Ǘ�*/
class CTaskManager{
private:
	//�^�X�N�}�l�[�W���̃C���X�^���X
	static CTaskManager* mpInstance;

protected:
	//�f�t�H���g�R���X�g���N�^
	CTaskManager();

public:
	//�C���X�^���X�̎擾
	static CTaskManager* Get();

	CTask mHead;//�擪�^�X�N
	CTask mTail;//�ŏI�^�X�N

	//�f�X�g���N�^
	virtual ~CTaskManager();

	//���X�g�ɒǉ�
	//Add(�^�X�N�̃|�C���^)
	void Add(CTask *addTask);
	//�X�V
	void Update();
	//�`��
	void Render();
	//�Փˏ���
	void TaskCollision();

	//���X�g����폜
	//Remove(�^�X�N�̃|�C���^)
	void Remove(CTask *task);
	//�^�X�N�̍폜
	void Delete();
};

#endif