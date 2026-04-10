#include "CCollider.h"
#include "CCollisionManager.h"

//三角形v0v1v2と線分svevが衝突していればtrueを返す
bool FuncCollisionTriangleLine(
	const CVector& v0, //三角形の頂点1
	const CVector& v1, //三角形の頂点2
	const CVector& v2, //三角形の頂点3
	const CVector& normal, //三角形の法線
	const CVector& sv, //線分の始点
	const CVector& ev, //線分の終点
	CVector* a) //調整値
{
	//三角の頂点から線分始点へのベクトルを求める
	CVector v0sv = sv - v0;
	//三角の頂点から線分終点へのベクトルを求める
	CVector v0ev = ev - v0;
	//線分が面と交差しているか内積で確認する
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//プラスは交差してない
	if (dots * dote >= 0.0f) {
		//衝突してない（調整不要）
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//面と線分の交点を求める
	//交点の計算
	CVector cross = sv + (ev - sv) * (abs(dots) / (abs(dots) + abs(dote)));
	//交点が三角形内なら衝突している
	if ((v1 - v0).Cross(cross - v0).Dot(normal) < 0.0f) {
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	if ((v2 - v1).Cross(cross - v1).Dot(normal) < 0.0f) {
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	if ((v0 - v2).Cross(cross - v2).Dot(normal) < 0.0f) {
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//線分は面と交差している
	//調整値計算（衝突しない位置まで戻す）
	if (dots < 0.0f) {
		//始点が裏面
		*a = normal * -dots;
	}
	else {
		//終点が裏面
		*a = normal * -dote;
	}
	return true;
}

void CCollider::ChangePriority()
{
	//自分の座標×親の変換行列を掛けてワールド座標を求める
	CVector pos = mPosition * *mpMatrix;
	//ベクトルの長さが優先度
	CCollider::ChangePriority((int)pos.Length());
}

void CCollider::ChangePriority(int priority)
{
	mPriority = priority;
	CCollisionManager::Instance()->Remove(this); //一旦削除
	CCollisionManager::Instance()->Add(this); //追加
}

//CollisionTriangleSphere(三角コライダ, 球コライダ, 調整値)
//retrun:true（衝突している）false(衝突していない)
//調整値:衝突しない位置まで戻す値
bool CCollider::CollisionTriangleSphere(
	CCollider* triangle, //三角形コライダ
	CCollider* sphere, //球コライダ
	CVector* adjust) //調整値
{
	CVector v0, v1, v2, normal, sv, ev;
	//課題
	//各コライダの頂点をワールド座標へ変換
	v0 = triangle->mV[0] * *triangle->mpMatrix;
	v1 = triangle->mV[1] * *triangle->mpMatrix;
	v2 = triangle->mV[2] * *triangle->mpMatrix;
	ev = sphere->mPosition * *sphere->mpMatrix;
	//面の法線を、外積を正規化して求める
	normal = (v1 - v0).Cross(v2 - v0).Normalize();
	sv = ev + normal * sphere->mRadius;
	ev = ev - normal * sphere->mRadius;

	//三角形と線分の衝突判定を行う
	return FuncCollisionTriangleLine(v0, v1, v2, normal, sv, ev, adjust);
}


bool CCollider::CollisionTriangleLine(CCollider* t, CCollider* l, CVector* a)
{
	CVector v[3], sv, ev;
	//各コライダの頂点をワールド座標へ変換
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	sv = l->mV[0] * *l->mpMatrix;
	ev = l->mV[1] * *l->mpMatrix;
	//面の法線を、外積を正規化して求める
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Normalize();
	return FuncCollisionTriangleLine(v[0], v[1], v[2], normal, sv, ev, a);
}


CCollider::EType CCollider::Type()
{
	return mType;
}

CCollider::CCollider()
	: mpParent(nullptr)
	, mpMatrix(&mMatrix)
	, mType(EType::ESPHERE)
	, mRadius(0)
{
	//コリジョンマネージャに追加
	CCollisionManager::Instance()->Add(this);
}

bool CCollider::Collision(CCollider* m, CCollider* o)
{
	//各コライダの中心座標を求める
	//原点×コライダの変換行列×親の変換行列
	CVector mpos = m->mPosition * *m->mpMatrix;
	CVector opos = o->mPosition * *o->mpMatrix;
	//中心から中心へのベクトルを求める
	mpos = mpos - opos;
	//中心の距離が半径の合計より小さいと衝突
	if (m->mRadius + o->mRadius > mpos.Length()) {
		//衝突している
		return  true;
	}
	//衝突していない
	return false;
}

CCollider::~CCollider()
{
	//コリジョンリストから削除
	CCollisionManager::Instance()->Remove(this);
}

CCollider::CCollider(CCharacter3* parent, CMatrix* matrix,
	const CVector& position, float radius)
	: CCollider() 
{
	//親設定
	mpParent = parent;
	//親行列設定
	mpMatrix = matrix;
	//CTransform設定
	mPosition = position; //位置
	//半径設定
	mRadius = radius;
	//コリジョンマネージャに追加
	//CCollisionManager::Instance()->Add(this);

}

CCharacter3* CCollider::Parent()
{
	return mpParent;
}

void CCollider::Render() {
	glPushMatrix();
	//コライダの中心座標を計算
	//自分の座標×親の変換行列を掛ける
	CVector pos = mPosition * *mpMatrix;
	//中心座標へ移動
	glMultMatrixf(CMatrix().Translate(pos.X(), pos.Y(), pos.Z()).M());
	//DIFFUSE赤色設定
	float c[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//球描画
	glutWireSphere(mRadius, 16, 16);
	glPopMatrix();
}
