#version 330 core
uniform vec3 lightVec;
uniform vec3 lightAmbientColor;
uniform vec3 lightDiffuseColor;
uniform vec3 eyeVec;
uniform vec4 Ambient;
uniform vec4 Diffuse;
uniform vec3 Specular;
uniform vec3 Emissive;
uniform float Pow;
uniform int TextureFlg;
uniform sampler2D Sampler;	//テクスチャ
uniform sampler2DShadow DepthTexture;	//デプステクスチャ

//頂点シェーダーから受け取る変数
in vec3 N;//法線ベクトル
in vec2 TexCoord0;	// 頂点シェーダから受け取るテクスチャ座標
in vec4 TexCoord1;	// 頂点シェーダから受け取るテクスチャ座標(DepthTexture用)

//出力
out vec4 FragColor;             // 出力するフラグメントの色

void main() {
	float NL = dot(N, lightVec); 
	vec3 Reflect = normalize(2 * NL * N - lightVec);
	float specular = pow(clamp(dot(Reflect, eyeVec),0,1.0), Pow);

	vec4 texColor;
	if(TextureFlg == -1)
	{
		texColor = vec4(1.0,1.0,1.0,1.0);
	}
	else
	{
		texColor = texture(Sampler, TexCoord0);
	}
	//デプステクスチャの値を取得
	float shd = textureProj(DepthTexture, TexCoord1);
	//値が0は影にする
	if(shd == 0.0)
	{
		//texColor = texColor * 0.4; //影にする
		texColor.w = 1.0;
	}
    // テクスチャをサンプリング
	FragColor= texColor * (Diffuse * clamp(NL,0,1.0)* vec4(lightDiffuseColor,1.0)+ Ambient * vec4(lightAmbientColor,1.0) + vec4(specular*Specular + Emissive,1.0));
}
