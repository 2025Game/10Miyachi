#ifndef CMODEL_H
#define CMODEL_H

#include "CTriangle.h"
#include "CMaterial.h"
#include "CVertex.h"

//vectorのインクルード
#include <vector>
#include <memory>		//smart pointer

#include "CModelX.h"
#include "CShadowShader.h"

/*
モデルクラス
モデルデータの入力や表示
*/
class CModel : public CMesh {
public:
	const std::vector<CTriangle>& Triangles() const;

	//描画
	//Render(行列)
	void Render(const CMatrix& m);

	//モデルファイルの入力
	//Load(モデルファイル名, マテリアルファイル名)
	void Load(const char* obj, const char* mtl);
	//描画
	void Render();

	~CModel();

private:
	//std::unique_ptr<CShadowShader> mpShader; //シェーダー
	CShadowShader mShader; //シェーダーのインスタンス

	//頂点の配列
	CVertex* mpVertexes;
	void CreateVertexBuffer();

	//三角形の可変長配列
	std::vector<CTriangle> mTriangles;
	//マテリアルポインタの可変長配列
	//std::vector<CMaterial*> mMaterial;

};

#endif
