#include "CRectangle.h"
#include <stdio.h>
#include "glut.h"

bool CRectangle::Collision(CRectangle* r, float* ax, float* ay)
{
	if (mX < r->mX)
		*ax = r->mX - mX - mW - r->mW;
	else
		*ax = mX - r->mX - mW - r->mW;
	//0以上は衝突しない
	if (*ax >= 0.0f)
		return false;
	if (mY < r->mY)
		*ay = r->mY - mY - mH - r->mH;
	else
		*ay = mY - r->mY - mH - r->mH;
	//0以上は衝突しない
	if (*ay >= 0.0f)
		return false;

	//Yが短いか判定
	if (*ax < *ay)
	{//Y修正、Xは0
		*ax = 0.0f;
		//上の時
		if (mY > r->mY)
			*ay = -*ay;
	}
	else
	{//X修正、Yは0
		*ay = 0.0f;
		//右の時
		if (mX > r->mX)
			*ax = -*ax;
	}
	return true;

}

bool CRectangle::Collision(CRectangle* r)
{
	float distX = mX - r->mX; //X方向の差を求める
	if (distX < 0.0f)  //差がマイナスならプラスにする
		distX = -distX;
	if (distX >= mW + r->mW) //差が幅の合計以上なら
		return false;  //当たっていない
	//課題１３
	float distY = mY - r->mY; //Y方向の差を求める
	if (distY < 0.0f)  //差がマイナスならプラスにする
		distY = -distY;
	if (distY >= mH + r->mH) //差が高さの合計以上なら
		return false;  //当たっていない

	return true;
}

CRectangle::CRectangle()
{
	printf("インスタンスが生まれました\n");
}

CRectangle::~CRectangle()
{
	printf("インスタンスが破棄されました\n");
}

void CRectangle::Render()
{
	glBegin(GL_QUADS);
	glVertex2f(mX - mW, mY - mH);
	glVertex2f(mX + mW, mY - mH);
	glVertex2f(mX + mW, mY + mH);
	glVertex2f(mX - mW, mY + mH);
	glEnd();

}

void CRectangle::Render(float x, float y, float w, float h)
{
	glBegin(GL_QUADS);
	glVertex2f(x - w, y - h);
	glVertex2f(x + w, y - h);
	glVertex2f(x + w, y + h);
	glVertex2f(x - w, y + h);
	glEnd();
}

void CRectangle::Set(float x, float y, float w, float h)
{
	mX = x;
	mY = y;
	mW = w;
	mH = h;
}

float CRectangle::X()
{
	return mX;
}

float CRectangle::Y()
{
	return mY;
}

float CRectangle::W()
{
	return mW;
}

float CRectangle::H()
{
	return mH;
}

void CRectangle::Y(float y)
{
	mY = y;
}
void CRectangle::X(float x)
{
	mX = x;
}
