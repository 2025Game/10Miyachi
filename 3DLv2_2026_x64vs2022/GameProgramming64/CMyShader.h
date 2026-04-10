#ifndef CMYSHADER_H
#define CMYSHADER_H

#include "CShader.h"

class CModelX;
class CMaterial;
class CMesh;
class CMatrix;
class CModel;

class CMyShader : public CShader {
protected:
	//マテリアルの設定
	void SetShader(CMaterial* material);
	//描画処理
	void Render(CModelX* model, CMesh* mesh, CMatrix* pCombinedMatrix);
	void Render330(CModelX* model, CMesh* mesh, CMatrix* pCombinedMatrix);
public:
	//描画処理
	void Render(CModelX* model, CMatrix* combinedMatrix);
	//描画処理
	void Render(const CModel* mesh, const CMatrix* matrix);
	//シャドウマップ Version330用
	void Render330(const CMesh* mesh, const CMatrix* skin_matrix, int skin_matrix_size);
};

#endif
