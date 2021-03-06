#ifndef CMATRIX_H
#define CMATRIX_H

/*}gNXNX
@4s4ńĚsńf[^đľ˘Üˇ*/
class CMatrix{
public:
	 //4~4Ěsńf[^đÝč
	float mM[4][4];
	 //\ŚmF
	 //4~4ĚsńđćĘoÍ
	void Print();
	 //ftHgRXgN^
	CMatrix();
	 //PĘsńĚěŹ
	CMatrix Identity();
	 //gĺkŹsńĚěŹ
	 //Scale({ŚX,{ŚY,{ŚZ)
	CMatrix Scale(float sx, float sy, float sz);

	 //ń]sń(Y˛,Z˛,X˛)ĚěŹ
	 //RotateY(px)
	CMatrix RotateY(float degree);
	 //RotateZ(px)
	CMatrix RotateZ(float degree);
	 //RotateX(px)
	CMatrix RotateX(float degree);

	 //ÚŽsńĚěŹ
	 //Translate(ÚŽĘX,ÚŽĘY,ÚŽĘZ)
	CMatrix Translate(float mx, float my, float mz);

	//*ZqĚI[o[[h
	//CMatrix * CMatrix ĚZqđÔˇ
	CMatrix operator*(const CMatrix &m);

};
#endif