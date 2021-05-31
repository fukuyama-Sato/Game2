#ifndef CCHARACTER_H
#define CCHARACTER_H
//�ϊ��s��N���X�̃C���N���[�h
#include"CTransform.h"
//���f���N���X�̃C���N���[�h
#include"CModel.h"
#include"CTask.h"
/*�L�����N�^�[�N���X
�@�Q�[���L�����N�^�̊�{�I�ȋ@�\���`����*/
class CCollider;

class CCharacter : public CTransform,public CTask{
public:
	enum ETag{
		EZERO,
		EPLAYER,
		EENEMY,
		EBULLETPLAYER,
		EBULLETENEMY,
		EBLOCK,
	};
	ETag mTag;

	CModel *mpModel;	//���f���̃|�C���^

	//�f�t�H���g�R���X�g���N�^
	CCharacter();
	//�f�X�g���N�^
	~CCharacter();

	//�`�揈��
	void Render();

	virtual void Collision(CCollider *m, CCollider *o){}
};


#endif