#include "CMaterial.h"
//memset,strncpyのインクルード
#include <string.h>
#include "glut.h"

/*
* strncpy(char* str1, const char* str2, int len)
* コピー先str1にコピー元str2の文字をlen文字数までコピーする
*/
char* strncpy(char* str1, const char* str2, int len)
{
	int i = 0;
	//iがlenより小さく、かつ、コピー元が終わりでない間繰り返し
	while (i < len && *str2 != '\0')
	{
		*(str1 + i) = *str2; //コピー先にコピー元を代入
		str2++; //コピー元を次へ
		i++;
	}
	str1[i] = '\0'; //コピー先の文字列に終わり
	return str1; //コピー先の先頭アドレスを返却
}

//デフォルトコンストラクタ
CMaterial::CMaterial()
{
	//名前を0で埋め
	memset(mName, 0, sizeof(mName));
	//0で埋める
	memset(mDiffuse, 0, sizeof(mDiffuse));
}

//マテリアルを有効にする
void CMaterial::Enabled()
{
	//拡散光の設定
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mDiffuse);
}

//マテリアルの名前の取得
char* CMaterial::Name()
{
	return mName;
}
// マテリアルの名前を設定する
// Name(マテリアルの名前)
void CMaterial::Name(char* name)
{
	strncpy(mName, name, MATERIAL_NAME_LEN);
}
//mDiffuse配列の取得
float* CMaterial::Diffuse()
{
	return mDiffuse;
}