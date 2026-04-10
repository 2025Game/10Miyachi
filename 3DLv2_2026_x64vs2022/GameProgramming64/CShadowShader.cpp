#include "glew.h"
#include "CShadowShader.h"
#include "CModelX.h"
#include "CMatrix.h"
#include "CModel.h"
#include "CVertex.h"
#include "CShadowMap.h"

void CShadowShader::Render(CModelX* model, CMatrix* pCombinedMatrix) {
	for (size_t i = 0; i < model->mFrame.size(); i++) {
		if (model->mFrame[i]->mpMesh != nullptr) {
			//面のあるメッシュは描画する
			Render(model, model->mFrame[i]->mpMesh, pCombinedMatrix);
		}
	}
}
/*
メッシュの描画
*/
void CShadowShader::Render(const CModelX* model, const CMesh* mesh, const CMatrix* pCombinedMatrix) {
	//スキンマトリックス生成
	for (size_t i = 0; i < mesh->mSkinWeights.size(); i++) {
		//スキンメッシュの行列配列を設定する
		model->mpSkinningMatrix[mesh->mSkinWeights[i]->mFrameIndex]
			= mesh->mSkinWeights[i]->mOffset * pCombinedMatrix[mesh->mSkinWeights[i]->mFrameIndex];
	}

	Render330(mesh, model->mpSkinningMatrix, (int)model->mFrame.size());
}

void CShadowShader::Render330(
	  const CMesh* mesh
	, const CMatrix* skin_matrix
	, int skin_matrix_size
)
{
	//シェーダーを有効にする
	Enable();

	/*
	ライト設定
	*/
	CVector vec(100.0f, 700.0f, -300.0f), ambient(0.9f, 0.9f, 0.9f), diffuse(1.0f, 1.0f, 1.0f);
	//	vec = (CVector() - vec).Normalize();
	vec = vec.Normalize();
	int lightId = glGetUniformLocation(GetProgram(), "lightVec");  //ライトの向きを設定
	glUniform3fv(lightId, 1, (float*)&vec);
	glUniform3fv(glGetUniformLocation(GetProgram(), "lightAmbientColor"), 1, (float*)&ambient);
	glUniform3fv(glGetUniformLocation(GetProgram(), "lightDiffuseColor"), 1, (float*)&diffuse);
	//スキンメッシュ行列設定
	int MatrixLocation = glGetUniformLocation(GetProgram(), "Transforms");
	glUniformMatrix4fv(MatrixLocation, skin_matrix_size, GL_FALSE, skin_matrix->M());

	CMatrix modelview, projection;
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview.M());
	/* 現在の透視変換行列を保存しておく */
	glGetFloatv(GL_PROJECTION_MATRIX, projection.M());
	MatrixLocation = glGetUniformLocation(GetProgram(), "MVP");
	glUniformMatrix4fv(MatrixLocation, 1, GL_FALSE, (modelview * projection).M());

	MatrixLocation = glGetUniformLocation(GetProgram(), "TextureMatrix1");
	glUniformMatrix4fv(MatrixLocation, 1, GL_FALSE, CShadowMap::msDepthTextureMatrix.M());

	/* テクスチャユニット1を指定する */
	glUniform1i(glGetUniformLocation(GetProgram(), "DepthTexture"), 1);

	//頂点バッファをバインドする
	glBindBuffer(GL_ARRAY_BUFFER, mesh->mMyVertexBufferId);

	const int POSITION_LOC = 0;
	const int NORMAL_LOC = 1;
	const int TEXCOORD_LOC = 2;
	const int WEIGHT_LOC = 3;
	const int WINDEX_LOC = 4;

	//頂点座標の位置を設定
	intptr_t idx = 0;
	glEnableVertexAttribArray(POSITION_LOC);
	glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT, GL_TRUE, sizeof(CVertex), (void*)idx);

	//法線ベクトルの位置を設定
	idx += sizeof(CVector);
	glEnableVertexAttribArray(NORMAL_LOC);
	glVertexAttribPointer(NORMAL_LOC, 3, GL_FLOAT, GL_TRUE, sizeof(CVertex), (void*)idx);

	//テクスチャマッピングデータの位置を設定
	idx += sizeof(CVector);
	glEnableVertexAttribArray(TEXCOORD_LOC);
	glVertexAttribPointer(TEXCOORD_LOC, 2, GL_FLOAT, GL_TRUE, sizeof(CVertex), (void*)idx);

	//スキンウェイトデータの位置を設定
	idx += sizeof(CVector);
	glEnableVertexAttribArray(WEIGHT_LOC);
	glVertexAttribPointer(WEIGHT_LOC, 4, GL_FLOAT, GL_TRUE, sizeof(CVertex), (void*)idx);
	//スキンウェイトのインデックスデータの位置を設定
	idx += sizeof(float) * 4;
	glEnableVertexAttribArray(WINDEX_LOC);
	glVertexAttribPointer(WINDEX_LOC, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), (void*)idx);

	//マテリアル毎に頂点を描画します
	int k = 0;
	for (size_t i = 0; i < mesh->mMaterial.size(); i++) {
		//マテリアルの値をシェーダーに設定
		SetShader(mesh->mMaterial[i]);
		//三角形描画、開始頂点番号、描画に使用する頂点数
		glDrawArrays(GL_TRIANGLES, k, mesh->mMaterialVertexCount[i]);	//DrawArrays:VertexIndexなし
		//開始位置計算
		k += mesh->mMaterialVertexCount[i];
		//マテリアルの解除
		mesh->mMaterial[i]->Disabled();
	}

	//無効にする
	glDisableVertexAttribArray(WINDEX_LOC);
	glDisableVertexAttribArray(WEIGHT_LOC);
	glDisableVertexAttribArray(TEXCOORD_LOC);
	glDisableVertexAttribArray(NORMAL_LOC);
	glDisableVertexAttribArray(POSITION_LOC);

	//頂点バッファのバインド解除
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//シェーダーを無効にする
	Disable();
}

