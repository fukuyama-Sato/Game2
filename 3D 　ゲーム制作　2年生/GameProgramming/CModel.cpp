#include"CModel.h"
//�W�����o�͂̃C���N���[�h
#include<stdio.h>
//������֐��̃C���N���[�h
#include<string.h>
//CVector�̃C���N���[�h
#include"CVector.h"

//�f�t�H���g�R���X�g���N�^
CModel::CModel()
: mpVertex(0), mpNormal(0), mpTextureCoord(0){

}

//�f�X�g���N�^
CModel::~CModel()
{
	for (int i = 0; i < mpMaterials.size(); i++)
	{
		delete mpMaterials[i];
	}
	//���_���W,�@��,�e�N�X�`���}�b�s���O�z��폜
	if (mpVertex){
		delete[]mpVertex;
	}
	if (mpNormal){
		delete[]mpNormal;
	}
	if (mpTextureCoord){
		delete[]mpTextureCoord;
	}
}

//���f���t�@�C���̓���
//Load(���f���t�@�C����,�}�e���A����)
void CModel::Load(char *obj, char *mtl){

	//���_�f�[�^�̕ۑ�(CVector�^)
	std::vector<CVector>normal;
	std::vector<CVector>vertex;

	//�e�N�X�`���}�b�s���O�̕ۑ�
	std::vector<CVector>uv;

	//�t�@�C���|�C���^�ϐ��̍쐬
	FILE *fp;

	/*�t�@�C���̃I�[�v��
	  fopen(�t�@�C����,���[�h)
	  �I�[�v���ł��Ȃ�����NULL��Ԃ�*/
	fp = fopen(mtl, "r");
	//�t�@�C���I�[�v���G���[�̔���
	//fp��NULL�̎��̓G���[
	if (fp == NULL){
		//�R���\�[���ɃG���[�o�͂��Ė߂�
		printf("%s file open error\n", obj);
		return;
	}
	//�t�@�C������f�[�^�����
	//���̓G���A���쐬����
	char buf[256];
	//�}�e���A���C���f�b�N�X
	int idx = 0;
	//�t�@�C�������s����
	//fgets(���̓G���A,�G���A�T�C�Y,�t�@�C���|�C���^)
	//�t�@�C���̍Ō�ɂȂ��NULL��Ԃ�
	while (fgets(buf, sizeof(buf), fp) != NULL){
		//�f�[�^�𕪊�����
		char str[4][64] = { "", "", "", "" };
		//�����񂩂�f�[�^��4�ϐ��֑������
		//sscanf(������,�ϊ��w��q,�ϐ�)
		sscanf(buf, "%s%s%s%s", str[0], str[1], str[2], str[3]);
		//�擪��newmtl�̎��A�}�e���A����ǉ�����
		if (strcmp(str[0], "newmtl") == 0){
			CMaterial *pm = new CMaterial();
			//�}�e���A�����̃R�s�[
			strncpy(pm->mName, str[1], sizeof(pm->mName) - 1);
			//�}�e���A���̉ϒ��z��ɒǉ�
			mpMaterials.push_back(pm);
			//�z��̒������擾
			idx = mpMaterials.size() - 1;
		}
		//�擪��Kd�̎��ADiffuse��ݒ肷��
		else if (strcmp(str[0], "Kd") == 0){
			mpMaterials[idx]->mDiffuse[0] = atof(str[1]);
			mpMaterials[idx]->mDiffuse[1] = atof(str[2]);
			mpMaterials[idx]->mDiffuse[2] = atof(str[3]);
		}
		//�擪��d�̎��Aa�l��ݒ肷��
		else if (strcmp(str[0], "d") == 0){
			mpMaterials[idx]->mDiffuse[3] = atof(str[1]);
		}
		//�擪��ma_Kd�̎��A�e�N�X�`������͂���
		else if (strcmp(str[0], "map_Kd") == 0){
			mpMaterials[idx]->mTexture.Load(str[1]);
		}
	}
//�t�@�C���̃N���[�Y
fclose(fp);

		fp = fopen(obj, "r");
		if (fp == NULL){
			//�R���\�[���ɃG���[�o�͂��Ė߂�
			printf("%s file open error\n", obj);
			return;
		}
		while (fgets(buf, sizeof(buf), fp) != NULL){
			//�f�[�^�𕪊�����
			char str[4][64] = { "", "", "", "" };
			//�����񂩂�f�[�^��4�ϐ��֑������
			//sscanf(������,�ϊ��w��q,�ϐ�)
			sscanf(buf, "%s%s%s%s", str[0], str[1], str[2], str[3]);
			//������̔�r
			//strcmp(������1,������2)
			//������1�ƕ�����2�������Ƃ�0�A�قȂ鎞0�ȊO��Ԃ�
			//�擪��v�̎��A���_��vertex�ɒǉ�����
			if (strcmp(str[0], "v") == 0){
				//�ϒ��z��vertex�ɒǉ�
				//atof(������) �����񂩂�float�^�̒l��Ԃ�
				vertex.push_back(CVector(atof(str[1]), atof(str[2]), atof(str[3])));
			}
			else if (strcmp(str[0], "usemtl") == 0){
				//�ϒ��z����ォ���r
				for (idx = mpMaterials.size() - 1; idx > 0; idx--){
					//�������O�̃}�e���A��������΃��[�v�I��
					if (strcmp(mpMaterials[idx]->mName, str[1]) == 0){
						break;
					}
				}
			}
			else if (strcmp(str[0], "vt") == 0){
				//�ϒ��z��uv�ɒǉ�
				//atof(������) �����񂩂�float�^�̒l��Ԃ�
				uv.push_back(CVector(atof(str[1]), atof(str[2]), 0.0));
			}
			//�擪��f�̎��A�O�p�`���쐬���Ēǉ�����
			else {
				if (strcmp(str[0], "vn") == 0){
					//�ϒ��z��normal�ɒǉ�
					//atof(������) �����񂩂�float�^�̒l��Ԃ�
					normal.push_back(CVector(atof(str[1]), atof(str[2]), atof(str[3])));
				}
				if (strcmp(str[0], "f") == 0){
					//���_�Ɩ@���̔ԍ��쐬
					int v[3], n[3];
					//�e�N�X�`���}�b�s���O�̗L���𔻒�
					if (strstr(str[1], "//")){
						//���_�Ɩ@���̔ԍ��擾
						sscanf(str[1], "%d//%d", &v[0], &n[0]);
						sscanf(str[2], "%d//%d", &v[1], &n[1]);
						sscanf(str[3], "%d//%d", &v[2], &n[2]);
						//�O�p�`�쐬
						CTriangle t;
						t.SetNormal(normal[n[0] - 1], normal[n[1] - 1], normal[n[2] - 1]);
						t.SetVertex(vertex[v[0] - 1], vertex[v[1] - 1], vertex[v[2] - 1]);
						//�}�e���A���ԍ��̐ݒ�
						t.mMaterialIdx = idx;
						//�ϒ��z��mTriangles�ɎO�p�`��ǉ�
						mTriangles.push_back(t);
					}
					else{
						//�e�N�X�`���}�b�s���O�L
						int u[3]; //�e�N�X�`���}�b�s���O�̔ԍ�
						//���_�Ɩ@���̔ԍ��擾�ƃ}�b�s���O�̔ԍ��擾
						sscanf(str[1], "%d/%d/%d", &v[0], &u[0], &n[0]);
						sscanf(str[2], "%d/%d/%d", &v[1], &u[1], &n[1]);
						sscanf(str[3], "%d/%d/%d", &v[2], &u[2], &n[2]);
						//�O�p�`�쐬
						CTriangle t;
						t.SetVertex(vertex[v[0] - 1], vertex[v[1] - 1], vertex[v[2] - 1]);
						t.SetNormal(normal[n[0] - 1], normal[n[1] - 1], normal[n[2] - 1]);
						//�e�N�X�`���}�b�s���O�̐ݒ�
						t.mUv[0] = uv[u[0] - 1];
						t.mUv[1] = uv[u[1] - 1];
						t.mUv[2] = uv[u[2] - 1];
						//�}�e���A���ԍ��̐ݒ�
						t.mMaterialIdx = idx;
						//�ϒ��z��mTriangles�ɎO�p�`��ǉ�
						mTriangles.push_back(t);
					}
				}
			}
		}
	//�t�@�C���̃N���[�Y
	fclose(fp);

	//���_�z��̍쐬
	//�S�Ă̍��W,�@��,�e�N�X�`���}�b�s���O�̒l���}�e���A���̏��Ԃɕۑ�
	mpVertex = new float[mTriangles.size() * 9];
	mpNormal = new float[mTriangles.size() * 9];
	mpTextureCoord = new float[mTriangles.size() * 6];

	int v = 0, t = 0;
	//�}�e���A�����ɒ��_�z��ɐݒ肷��
	for (int i = 0; i < mpMaterials.size(); i++){
		//�S�Ă̎O�p�`���r
		for (int j = 0; j < mTriangles.size(); j++){
			//�}�e���A���ԍ�����v����Ƃ�
			if (i == mTriangles[j].mMaterialIdx){
				//���_�z��ɐݒ�
				//���_���W
				mpVertex[v++] = mTriangles[j].mV[0].mX;
				mpVertex[v++] = mTriangles[j].mV[0].mY;
				mpVertex[v++] = mTriangles[j].mV[0].mZ;
				mpVertex[v++] = mTriangles[j].mV[1].mX;
				mpVertex[v++] = mTriangles[j].mV[1].mY;
				mpVertex[v++] = mTriangles[j].mV[1].mZ;
				mpVertex[v++] = mTriangles[j].mV[2].mX;
				mpVertex[v++] = mTriangles[j].mV[2].mY;
				mpVertex[v++] = mTriangles[j].mV[2].mZ;
				v -= 9;
				//�@��
				mpNormal[v++] = mTriangles[j].mN[0].mX;
				mpNormal[v++] = mTriangles[j].mN[0].mY;
				mpNormal[v++] = mTriangles[j].mN[0].mZ;
				mpNormal[v++] = mTriangles[j].mN[1].mX;
				mpNormal[v++] = mTriangles[j].mN[1].mY;
				mpNormal[v++] = mTriangles[j].mN[1].mZ;
				mpNormal[v++] = mTriangles[j].mN[2].mX;
				mpNormal[v++] = mTriangles[j].mN[2].mY;
				mpNormal[v++] = mTriangles[j].mN[2].mZ;
				//�e�N�X�`���}�b�s���O
				mpTextureCoord[t++] = mTriangles[j].mUv[0].mX;
				mpTextureCoord[t++] = mTriangles[j].mUv[0].mY;
				mpTextureCoord[t++] = mTriangles[j].mUv[1].mX;
				mpTextureCoord[t++] = mTriangles[j].mUv[1].mY;
				mpTextureCoord[t++] = mTriangles[j].mUv[2].mX;
				mpTextureCoord[t++] = mTriangles[j].mUv[2].mY;
			}
		}
		//���_����ݒ�
		mpMaterials[i]->mVertexNum = v / 3;
	}
}
//�`��
void CModel::Render(){
	//�ϒ��z��̗v�f�������J��Ԃ�
	for (int i = 0; i < mTriangles.size(); i++){
		//�}�e���A���̓K�p
		mpMaterials[mTriangles[i].mMaterialIdx]->Enabled();
		//�ϒ��z��ɓY�����ŃA�N�Z�X����
		mTriangles[i].Render();
		//�}�e���A���𖳌���
		mpMaterials[mTriangles[i].mMaterialIdx]->Disabled();
	}
}

//�`��
//Render(�s��)
void CModel::Render(const CMatrix &m){
	//�s��̑ޔ�
	glPushMatrix();
	//�����s���������
	glMultMatrixf(&m.mM[0][0]);

	//���_���W,�@��,�e�N�X�`���}�b�s���O�̔z���L����
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//���_���W,�@��,�e�N�X�`���R�[�h�̔z����w��
	glVertexPointer(3, GL_FLOAT, 0, mpVertex);
	glNormalPointer(GL_FLOAT, 0, mpNormal);
	glTexCoordPointer(2, GL_FLOAT, 0, mpTextureCoord);

	int first = 0; //�`��ʒu
	//�}�e���A�����ɕ`�悷��
	for (int i = 0; i < mpMaterials.size(); i++){
		//�}�e���A����K�p
		mpMaterials[i]->Enabled();
		//�`��ʒu����̃f�[�^�ŎO�p�`��`��
		glDrawArrays(GL_TRIANGLES, first, mpMaterials[i]->mVertexNum - first);
		//�}�e���A���𖳌���
		mpMaterials[i]->Disabled();
		first = mpMaterials[i]->mVertexNum;
	}
	//�s���߂�
	glPopMatrix();
	
	//���_���W,�@��,�e�N�X�`���}�b�s���O�̔z��𖳌��ɂ���
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	return;

}