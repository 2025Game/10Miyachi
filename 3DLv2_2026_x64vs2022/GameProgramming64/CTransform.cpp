#include "CTransform.h"

CTransform::CTransform()
{
	mpParent = nullptr;
}

const CMatrix& CTransform::CombinedMatrix() const
{
	return mCombinedMatrix;
}

const CVector& CTransform::Position() const
{
	return mPosition;
}

const CVector& CTransform::Rotation() const
{
	return mRotation;
}

void CTransform::Position(const CVector& v)
{
	mPosition = v;
}

void CTransform::Rotation(const CVector& v)
{
	mRotation = v;
}

void CTransform::Scale(const CVector& v)
{
	mScale = v;
}

void CTransform::Parent(CTransform* parent)
{
	mpParent = parent;
}

const CMatrix& CTransform::Matrix() const
{
	return mMatrix;
}

const CMatrix& CTransform::MatrixRotate() const
{
	return mMatrixRotate;
}

void CTransform::Update(const CVector& pos, const CVector& rot
	, const CVector& scale)
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	Update();
}

//行列更新処理
void CTransform::Update()
{
	//拡大縮小行列の設定
	mMatrixScale.Scale(mScale.X(), mScale.Y(), mScale.Z());
	//回転行列の設定
	mMatrixRotate = CMatrix().RotateZ(mRotation.Z()) * CMatrix().RotateX(mRotation.X()) * CMatrix().RotateY(mRotation.Y());
	//平行移動行列の設定
	mMatrixTranslate.Translate(mPosition.X(), mPosition.Y(), mPosition.Z());
	//合成行列の設定
	mMatrix = mMatrixScale * mMatrixRotate * mMatrixTranslate;
	//合成行列の設定
    //子に引き継ぐ合成行列は回転と移動のみ
	mCombinedMatrix = mMatrixRotate * mMatrixTranslate;
	//親がいる場合は、親の合成行列を掛ける
	if (mpParent) {
		mCombinedMatrix = mCombinedMatrix *
			mpParent->mCombinedMatrix;

	}
	//自分が使用する合成行列には、拡大縮小行列を掛ける
	mMatrix = mMatrixScale * mCombinedMatrix;
}