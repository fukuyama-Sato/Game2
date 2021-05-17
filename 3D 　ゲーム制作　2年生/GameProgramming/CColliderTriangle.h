#ifndef CCOLLIDERTRIANGLE_H
#define CCOLLIDERTRIANGLE_H
#include "CCollider.h"

/*�O�p�R���C�_�̒�`*/
class CColliderTriangle : public CCollider{
public:
	CColliderTriangle(){}
	//�R���X�g���N�^(�O�p�R���C�_)
	//CColliderTriangle(�e,�e�s��,���_,���_2,���_3)
	CColliderTriangle(CCharacter *parent, CMatrix *matrix, const CVector &v0, const CVector &v1, const CVector &v2);
	//�O�p�R���C�_�̐ݒ�
	//Set(�e,�e�s��,���_,���_2,���_3)
	void Set(CCharacter *parent, CMatrix *matrix, const CVector &v0, const CVector &v1, const CVector &v2);
	//�D��x�̕ύX
	void ChangePriority();
	//�`��
	void Render();
};

#endif