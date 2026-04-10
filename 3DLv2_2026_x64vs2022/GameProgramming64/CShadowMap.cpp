#include "glew.h"
#include "CShadowMap.h"
#include "CMatrix.h"

bool CShadowMap::msShadow = false; // trueならシャドウマップを生成している
CMatrix	CShadowMap::msDepthTextureMatrix; //シャドウマップ用テクスチャ変換行列の保存用

void CShadowMap::Init(int width, int height, void (*funcRender)(), float shadowCol[], float lightPos[])
{
	mDepthTextureID = 0;
	mFb = 0;
	mTextureHeight = height;
	mTextureWidth = width;
	mpRender = funcRender;
	mShadowCol[0] = shadowCol[0];
	mShadowCol[1] = shadowCol[1];
	mShadowCol[2] = shadowCol[2];
	mShadowCol[3] = shadowCol[3];
	mLightPos[0] = lightPos[0];
	mLightPos[1] = lightPos[1];
	mLightPos[2] = lightPos[2];
	Init();
}

void CShadowMap::Init()
{
	/* テクスチャユニット１をDepthテクスチャで使用 */
	glActiveTexture(GL_TEXTURE1);
	//テクスチャの生成
	glGenTextures(1, &mDepthTextureID);
	//使用するテクスチャのバインド
	glBindTexture(GL_TEXTURE_2D, mDepthTextureID);

	/* Depthテクスチャの割り当て */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mTextureWidth, mTextureHeight, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	/* テクスチャを拡大・縮小する方法の指定 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/* テクスチャの繰り返し方法の指定 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	/* 書き込むポリゴンのテクスチャ座標値のＲとテクスチャとの比較を行うようにする */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	/* もしＲの値がテクスチャの値以下なら真（つまり日向） */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	//テクスチャの解除
	glBindTexture(GL_TEXTURE_2D, 0);

	//フレームバッファ追加
	//* フレームバッファオブジェクトを生成して結合する 
	glGenFramebuffers(1, &mFb);
	glBindFramebuffer(GL_FRAMEBUFFER, mFb);

	//* フレームバッファオブジェクトにデプスバッファ用のテクスチャを結合する 
	glFramebufferTexture2D(GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, mDepthTextureID, 0);
	//* カラーバッファが無いので読み書きしない 
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	//* フレームバッファオブジェクトの結合を解除する 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//テクスチャユニットを0に戻す
	glActiveTexture(GL_TEXTURE0);
}

void CShadowMap::Render()
{
	//日向のライト
	const GLfloat lightcol[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLint	viewport[4]; //ビューポートの保存用
	CMatrix modelviewLight;
	CMatrix	projection; //透視変換行列の保存用
	/* モデルビュー変換行列を保存しておく */
	CMatrix modelviewCamera;
	glGetFloatv(GL_MODELVIEW_MATRIX, modelviewCamera.M());

	/*
	** 第１ステップ：デプステクスチャの作成
	*/
	//* フレームバッファオブジェクトへのレンダリング開始
	glBindFramebuffer(GL_FRAMEBUFFER, mFb);

	/* デプスバッファをクリアする */
	glClear(GL_DEPTH_BUFFER_BIT);

	/* 現在のビューポートを保存しておく */
	glGetIntegerv(GL_VIEWPORT, viewport);

	/* ビューポートをテクスチャのサイズに設定する */
	glViewport(0, 0, mTextureWidth, mTextureHeight);

	/* 現在の透視変換行列を保存しておく */
	glGetFloatv(GL_PROJECTION_MATRIX, projection.M());

	/* 透視変換行列を設定する */
	glMatrixMode(GL_PROJECTION); //透視変換行列に切り替え
	glLoadIdentity(); //行列の初期化

	/* 光源位置を視点としシーンが視野に収まるようモデルビュー変換行列を設定する */
	glMatrixMode(GL_MODELVIEW); //モデルビュー行列に切り替え
	glLoadIdentity(); //行列の初期化

	//光源位置から見るように行列を設定する
	gluPerspective(75.0, (GLdouble)mTextureWidth / (GLdouble)mTextureHeight, 1.0, 100000.0);
	gluLookAt(mLightPos[0], mLightPos[1], mLightPos[2], mLightPos[0] - 10.0f, 0, mLightPos[2] - 10.0f, 0.0, 1.0, 0.0);
	/* 設定した透視変換行列×モデルビュー変換行列を保存しておく */
	glGetFloatv(GL_MODELVIEW_MATRIX, modelviewLight.M());
	/* テクスチャのモデルビュー変換行列と透視変換行列の積をかける */
	//テクスチャ変換行列を作成
	msDepthTextureMatrix = modelviewLight * CMatrix().Scale(0.5, 0.5, 0.5) * CMatrix().Translate(0.5, 0.5, 0.5);

	/* デプスバッファの内容だけを取得するのでフレームバッファには書き込まない */
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	/* したがって陰影付けも不要なのでライティングをオフにする */
	glDisable(GL_LIGHTING);

	/* デプスバッファには背面のポリゴンの奥行きを記録するようにする */
	//glCullFace(GL_FRONT);
	glCullFace(GL_BACK); //表面に変更

	// シャドウマップの描画時にバイアスを適用する
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.5f, 3.5f);

	msShadow = true; // シャドウマップを生成している

	// デプステクスチャへの描画
	if (mpRender)
	{
		(*mpRender)();
	}

	msShadow = false; // シャドウマップの生成は終了

	glDisable(GL_POLYGON_OFFSET_FILL);

	/* フレームバッファオブジェクトへのレンダリング終了 */
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/* 通常の描画の設定に戻す */
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION); //透視変換行列に切り替え
	glLoadMatrixf(projection.M());
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_LIGHTING);
	glCullFace(GL_BACK);

	/*
	** 第２ステップ：全体の描画
	*/
	/* フレームバッファとデプスバッファをクリアする */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* モデルビュー変換行列に戻す */
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(modelviewCamera.M());

	/* テクスチャユニット１に切り替える */
	glActiveTexture(GL_TEXTURE1);
	/* テクスチャオブジェクトを結合する */
	glBindTexture(GL_TEXTURE_2D, mDepthTextureID);
	/* テクスチャマッピングを有効にする */
	glEnable(GL_TEXTURE_2D);

	/* テクスチャユニット0に切り替える */
	glActiveTexture(GL_TEXTURE0);

	if (mpRender)
	{
		(*mpRender)();
	}

	/* テクスチャユニット１に切り替える */
	glActiveTexture(GL_TEXTURE1);
	/* テクスチャオブジェクトの結合を解除する */
	glBindTexture(GL_TEXTURE_2D, 0);
	/* テクスチャマッピングを無効にする */
	glDisable(GL_TEXTURE_2D);
	/* テクスチャユニット0に切り替える */
	glActiveTexture(GL_TEXTURE0);
}

bool CShadowMap::Shadow()
{
	return msShadow;
}

CShadowMap::CShadowMap()
	: mDepthTextureID(0)
	, mFb(0)
	, mTextureHeight(0)
	, mTextureWidth(0)
	, mLightPos()
	, mShadowCol()
	, mpRender(nullptr)
{
}

CShadowMap::~CShadowMap()
{
	if (mDepthTextureID)
	{
		glDeleteTextures(1, &mDepthTextureID);
		mDepthTextureID = 0;
	}
	if (mFb)
	{
		glDeleteFramebuffers(1, &mFb);
		mFb = 0;
	}
}
