#include "CModel.h"
//標準入出力のインクルード
#include <stdio.h>
//CVectorのインクルード
#include "CVector.h"

//文字列s1と文字列s2の比較
//s1とs2が等しければ0を
//等しくなければ0以外を返す
int strcmp(const char* s1, const char* s2)
{
	int i = 0;
	//文字が同じ間は繰り返し
	//どちらかの文字が終わりになるとループの終わり
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
	{
		i++;
	}
	//同じなら引いて0
	return s1[i] - s2[i];
}

//モデルファイルの入力
//Load(モデルファイル名, マテリアルファイル名)
void CModel::Load(const char* obj, const char* mtl)
{
	//頂点データの保存(CVector型)
	std::vector<CVector> vertex;
	std::vector<CVector> normal;

	//ファイルポインタ変数の作成
	FILE* fp;
	//ファイルからデータを入力
	//入力エリアを作成する
	char buf[256];

	//ファイルのオープン
	//fopen(ファイル名,モード)
	//オープンできない時はNULLを返す
	fp = fopen(mtl, "r");
	//ファイルオープンエラーの判定
	//fpがNULLの時はエラー
	if (fp == NULL)
	{
		//コンソールにエラー出力して戻る
		printf("%s file open error￥n", mtl);
		return;
	}

	fp = fopen(obj, "r");
	if (fp == NULL)
	{
		printf("%s file open error￥n", obj);
		return;
	}

	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		printf("%s", buf);
		//データを分割する
		char str[4][64] = { "", "", "", "" };
		//文字列からデータを4つ変数へ代入する
		//sscanf(文字列, 変換指定子, 変数)
		sscanf(buf, "%s %s %s %s", str[0], str[1], str[2], str[3]);
		//文字列の比較
		//strcmp(文字列1, 文字列2)
		//文字列1と文字列2が同じ時0、異なる時0以外を返す
		//先頭がvの時、頂点をvertexに追加する
		if (strcmp(str[0], "v") == 0)
		{
			//可変長配列vertexに追加
			//atof(文字列)　文字列からfloat型の値を返す
			vertex.push_back(CVector(atof(str[1]), atof(str[2]), atof(str[3])));
		}
		else if (strcmp(str[0], "vn") == 0)
		{
			normal.push_back(CVector(atof(str[1]), atof(str[2]), atof(str[3])));
		}
		//先頭がfの時、三角形を作成して追加する
		else if (strcmp(str[0], "f") == 0)
		{
			//頂点と法線の番号作成
			int v[3], n[3];
			//頂点と法線の番号取得
			sscanf(str[1], "%d//%d", &v[0], &n[0]);
			sscanf(str[2], "%d//%d", &v[1], &n[1]);
			sscanf(str[3], "%d//%d", &v[2], &n[2]);
			//三角形作成
			CTriangle t;
			t.Vertex(vertex[v[0] - 1], vertex[v[1] - 1], vertex[v[2] - 1]);
			t.Normal(normal[n[0] - 1], normal[n[1] - 1], normal[n[2] - 1]);
			//可変長配列mTrianglesに三角形を追加
			mTriangles.push_back(t);
		}
	}

	fclose(fp);
}

//描画
void CModel::Render()
{
	//可変長配列の要素数だけ繰り返し
	for (int i = 0; i < mTriangles.size(); i++)
	{
		//可変長配列に添え字でアクセスする
		mTriangles[i].Render();
	}
}