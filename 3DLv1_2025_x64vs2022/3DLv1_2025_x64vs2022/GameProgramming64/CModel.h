#pragma once
#ifndef CMODEL_H
#define CMODEL_H
//vectorのインクルード
#include <vector>
#include "CTriangle.h"
#include "CMaterial.h"
#include "CVertex.h"

/*
モデルクラス
モデルデータの入力や表示
*/
class CModel
{
public:
	const std::vector<CTriangle>& Triangles() const;
	//描画
	//Render(行列)
	void Render(const CMatrix& m);
	~CModel();
	//描画
	void Render();
	//モデルファイルの入力
	//Load(モデルファイル名, マテリアルファイル名)
	void Load(const char* obj, const char* mtl);
private:
	//頂点の配列
	CVertex* mpVertexes;
	void CreateVertexBuffer();
	//マテリアルポインタの可変長配列
	std::vector<CMaterial*> mpMaterials;
	//三角形の可変長配列
	std::vector<CTriangle> mTriangles;
};

#endif