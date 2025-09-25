#version 330 core

uniform mat4 Transforms[128];	//スキニング行列
//uniform mat4 WorldMatrix;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec4 weights;
layout(location = 4) in vec4 indices;

uniform mat4 MVP; //モデルビュー×プロジェクション行列
uniform mat4 TextureMatrix1; // テクスチャ座標変換行列

// 出力
//フラグメントシェーダーに渡す変数
out vec3 N;//法線ベクトル
//フラグメントシェーダに渡すテクスチャ座標
out vec2 TexCoord0;	//テクスチャ
out vec4 TexCoord1;	//デプステクスチャ

void main(void)
{
    mat4 comb = mat4(0);
	comb += Transforms[int(indices.x)] * weights.x;
	comb += Transforms[int(indices.y)] * weights.y;
	comb += Transforms[int(indices.z)] * weights.z;
	comb += Transforms[int(indices.w)] * weights.w;
		  
	//座標、法線、テクスチャ座標を求める
	gl_Position = MVP * comb * vec4(aPosition,1);
    N = normalize(mat3(comb) * aNormal);
	TexCoord0 = aTexCoord;

	//デプステクスチャのテクスチャ座標を求める
	TexCoord1 = TextureMatrix1 * comb * vec4(aPosition,1);
}