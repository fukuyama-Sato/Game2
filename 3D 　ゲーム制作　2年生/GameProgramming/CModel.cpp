#include"CModel.h"
//標準入出力のインクルード
#include<stdio.h>
//文字列関数のインクルード
#include<string.h>
//CVectorのインクルード
#include"CVector.h"

//デフォルトコンストラクタ
CModel::CModel()
: mpVertex(0), mpNormal(0), mpTextureCoord(0){

}

//デストラクタ
CModel::~CModel()
{
	for (int i = 0; i < mpMaterials.size(); i++)
	{
		delete mpMaterials[i];
	}
	//頂点座標,法線,テクスチャマッピング配列削除
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

//モデルファイルの入力
//Load(モデルファイル名,マテリアル名)
void CModel::Load(char *obj, char *mtl){

	//頂点データの保存(CVector型)
	std::vector<CVector>normal;
	std::vector<CVector>vertex;

	//テクスチャマッピングの保存
	std::vector<CVector>uv;

	//ファイルポインタ変数の作成
	FILE *fp;

	/*ファイルのオープン
	  fopen(ファイル名,モード)
	  オープンできない時はNULLを返す*/
	fp = fopen(mtl, "r");
	//ファイルオープンエラーの判定
	//fpがNULLの時はエラー
	if (fp == NULL){
		//コンソールにエラー出力して戻る
		printf("%s file open error\n", obj);
		return;
	}
	//ファイルからデータを入力
	//入力エリアを作成する
	char buf[256];
	//マテリアルインデックス
	int idx = 0;
	//ファイルから一行入力
	//fgets(入力エリア,エリアサイズ,ファイルポインタ)
	//ファイルの最後になるとNULLを返す
	while (fgets(buf, sizeof(buf), fp) != NULL){
		//データを分割する
		char str[4][64] = { "", "", "", "" };
		//文字列からデータを4つ変数へ代入する
		//sscanf(文字列,変換指定子,変数)
		sscanf(buf, "%s%s%s%s", str[0], str[1], str[2], str[3]);
		//先頭がnewmtlの時、マテリアルを追加する
		if (strcmp(str[0], "newmtl") == 0){
			CMaterial *pm = new CMaterial();
			//マテリアル名のコピー
			strncpy(pm->mName, str[1], sizeof(pm->mName) - 1);
			//マテリアルの可変長配列に追加
			mpMaterials.push_back(pm);
			//配列の長さを取得
			idx = mpMaterials.size() - 1;
		}
		//先頭がKdの時、Diffuseを設定する
		else if (strcmp(str[0], "Kd") == 0){
			mpMaterials[idx]->mDiffuse[0] = atof(str[1]);
			mpMaterials[idx]->mDiffuse[1] = atof(str[2]);
			mpMaterials[idx]->mDiffuse[2] = atof(str[3]);
		}
		//先頭がdの時、a値を設定する
		else if (strcmp(str[0], "d") == 0){
			mpMaterials[idx]->mDiffuse[3] = atof(str[1]);
		}
		//先頭がma_Kdの時、テクスチャを入力する
		else if (strcmp(str[0], "map_Kd") == 0){
			mpMaterials[idx]->mTexture.Load(str[1]);
		}
	}
//ファイルのクローズ
fclose(fp);

		fp = fopen(obj, "r");
		if (fp == NULL){
			//コンソールにエラー出力して戻る
			printf("%s file open error\n", obj);
			return;
		}
		while (fgets(buf, sizeof(buf), fp) != NULL){
			//データを分割する
			char str[4][64] = { "", "", "", "" };
			//文字列からデータを4つ変数へ代入する
			//sscanf(文字列,変換指定子,変数)
			sscanf(buf, "%s%s%s%s", str[0], str[1], str[2], str[3]);
			//文字列の比較
			//strcmp(文字列1,文字列2)
			//文字列1と文字列2が同じとき0、異なる時0以外を返す
			//先頭がvの時、頂点をvertexに追加する
			if (strcmp(str[0], "v") == 0){
				//可変長配列vertexに追加
				//atof(文字列) 文字列からfloat型の値を返す
				vertex.push_back(CVector(atof(str[1]), atof(str[2]), atof(str[3])));
			}
			else if (strcmp(str[0], "usemtl") == 0){
				//可変長配列を後から比較
				for (idx = mpMaterials.size() - 1; idx > 0; idx--){
					//同じ名前のマテリアルがあればループ終了
					if (strcmp(mpMaterials[idx]->mName, str[1]) == 0){
						break;
					}
				}
			}
			else if (strcmp(str[0], "vt") == 0){
				//可変長配列uvに追加
				//atof(文字列) 文字列からfloat型の値を返す
				uv.push_back(CVector(atof(str[1]), atof(str[2]), 0.0));
			}
			//先頭がfの時、三角形を作成して追加する
			else {
				if (strcmp(str[0], "vn") == 0){
					//可変長配列normalに追加
					//atof(文字列) 文字列からfloat型の値を返す
					normal.push_back(CVector(atof(str[1]), atof(str[2]), atof(str[3])));
				}
				if (strcmp(str[0], "f") == 0){
					//頂点と法線の番号作成
					int v[3], n[3];
					//テクスチャマッピングの有無を判定
					if (strstr(str[1], "//")){
						//頂点と法線の番号取得
						sscanf(str[1], "%d//%d", &v[0], &n[0]);
						sscanf(str[2], "%d//%d", &v[1], &n[1]);
						sscanf(str[3], "%d//%d", &v[2], &n[2]);
						//三角形作成
						CTriangle t;
						t.SetNormal(normal[n[0] - 1], normal[n[1] - 1], normal[n[2] - 1]);
						t.SetVertex(vertex[v[0] - 1], vertex[v[1] - 1], vertex[v[2] - 1]);
						//マテリアル番号の設定
						t.mMaterialIdx = idx;
						//可変長配列mTrianglesに三角形を追加
						mTriangles.push_back(t);
					}
					else{
						//テクスチャマッピング有
						int u[3]; //テクスチャマッピングの番号
						//頂点と法線の番号取得とマッピングの番号取得
						sscanf(str[1], "%d/%d/%d", &v[0], &u[0], &n[0]);
						sscanf(str[2], "%d/%d/%d", &v[1], &u[1], &n[1]);
						sscanf(str[3], "%d/%d/%d", &v[2], &u[2], &n[2]);
						//三角形作成
						CTriangle t;
						t.SetVertex(vertex[v[0] - 1], vertex[v[1] - 1], vertex[v[2] - 1]);
						t.SetNormal(normal[n[0] - 1], normal[n[1] - 1], normal[n[2] - 1]);
						//テクスチャマッピングの設定
						t.mUv[0] = uv[u[0] - 1];
						t.mUv[1] = uv[u[1] - 1];
						t.mUv[2] = uv[u[2] - 1];
						//マテリアル番号の設定
						t.mMaterialIdx = idx;
						//可変長配列mTrianglesに三角形を追加
						mTriangles.push_back(t);
					}
				}
			}
		}
	//ファイルのクローズ
	fclose(fp);

	//頂点配列の作成
	//全ての座標,法線,テクスチャマッピングの値をマテリアルの順番に保存
	mpVertex = new float[mTriangles.size() * 9];
	mpNormal = new float[mTriangles.size() * 9];
	mpTextureCoord = new float[mTriangles.size() * 6];

	int v = 0, t = 0;
	//マテリアル毎に頂点配列に設定する
	for (int i = 0; i < mpMaterials.size(); i++){
		//全ての三角形を比較
		for (int j = 0; j < mTriangles.size(); j++){
			//マテリアル番号が一致するとき
			if (i == mTriangles[j].mMaterialIdx){
				//頂点配列に設定
				//頂点座標
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
				//法線
				mpNormal[v++] = mTriangles[j].mN[0].mX;
				mpNormal[v++] = mTriangles[j].mN[0].mY;
				mpNormal[v++] = mTriangles[j].mN[0].mZ;
				mpNormal[v++] = mTriangles[j].mN[1].mX;
				mpNormal[v++] = mTriangles[j].mN[1].mY;
				mpNormal[v++] = mTriangles[j].mN[1].mZ;
				mpNormal[v++] = mTriangles[j].mN[2].mX;
				mpNormal[v++] = mTriangles[j].mN[2].mY;
				mpNormal[v++] = mTriangles[j].mN[2].mZ;
				//テクスチャマッピング
				mpTextureCoord[t++] = mTriangles[j].mUv[0].mX;
				mpTextureCoord[t++] = mTriangles[j].mUv[0].mY;
				mpTextureCoord[t++] = mTriangles[j].mUv[1].mX;
				mpTextureCoord[t++] = mTriangles[j].mUv[1].mY;
				mpTextureCoord[t++] = mTriangles[j].mUv[2].mX;
				mpTextureCoord[t++] = mTriangles[j].mUv[2].mY;
			}
		}
		//頂点数を設定
		mpMaterials[i]->mVertexNum = v / 3;
	}
}
//描画
void CModel::Render(){
	//可変長配列の要素数だけ繰り返し
	for (int i = 0; i < mTriangles.size(); i++){
		//マテリアルの適用
		mpMaterials[mTriangles[i].mMaterialIdx]->Enabled();
		//可変長配列に添え字でアクセスする
		mTriangles[i].Render();
		//マテリアルを無効化
		mpMaterials[mTriangles[i].mMaterialIdx]->Disabled();
	}
}

//描画
//Render(行列)
void CModel::Render(const CMatrix &m){
	//行列の退避
	glPushMatrix();
	//合成行列をかける
	glMultMatrixf(&m.mM[0][0]);

	//頂点座標,法線,テクスチャマッピングの配列を有効に
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//頂点座標,法線,テクスチャコードの配列を指定
	glVertexPointer(3, GL_FLOAT, 0, mpVertex);
	glNormalPointer(GL_FLOAT, 0, mpNormal);
	glTexCoordPointer(2, GL_FLOAT, 0, mpTextureCoord);

	int first = 0; //描画位置
	//マテリアル毎に描画する
	for (int i = 0; i < mpMaterials.size(); i++){
		//マテリアルを適用
		mpMaterials[i]->Enabled();
		//描画位置からのデータで三角形を描画
		glDrawArrays(GL_TRIANGLES, first, mpMaterials[i]->mVertexNum - first);
		//マテリアルを無効に
		mpMaterials[i]->Disabled();
		first = mpMaterials[i]->mVertexNum;
	}
	//行列を戻す
	glPopMatrix();
	
	//頂点座標,法線,テクスチャマッピングの配列を無効にする
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	return;

}