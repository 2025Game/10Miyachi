#include "CCollider.h"
#include "CCollisionManager.h"

//0~1の間にクランプ(値を強制的にある範囲内にすること)
void clamp0to1(float& v)
{
	if (v < 0.0f) v = 0.0f;
	else if (v > 1.0f) v = 1.0f;
}

//CalcCalcPointLineDist(点, 始点, 終点, 線上の最短点, 割合)
//点と線(始点、終点を通る直線)の最短距離を求める
float CalcPointLineDist(const CVector& p, const CVector& s, const CVector& e, CVector* mp, float* t)
{
	*t = 0.0f; //割合の初期化
	CVector v = e - s; //始点から終点へのベクトルを求める
	float dvv = v.Dot(v); //ベクトルの長さの2乗を求める
	if (dvv > 0.0f)
	{
		*t = v.Dot(p - s) / dvv; //線上の垂線となる点の割合を求める
	}
	*mp = s + v * *t; //線上の垂線となる点を求める
	return (p - *mp).Length(); //垂線の長さを返す
}

//CalcLineLineDist(始点1, 終点1, 始点2, 終点2, 交点1, 交点2, 比率1, 比率2)
//2線間の最短距離を返す
float CalcLineLineDist(
	const CVector& s1, //始点1
	const CVector& e1, //終点1
	const CVector& s2, //始点2
	const CVector& e2, //終点2
	CVector* mp1, //交点1
	CVector* mp2, //交点2
	float* t1, //比率1
	float* t2 //比率2
)
{
	CVector v1 = e1 - s1;
	CVector v2 = e2 - s2;
	//2直線が平行
	if (v1.Cross(v2).Length() < 0.000001f)
	{
		//線分1の始点から直線2までの最短距離問題に帰着する
		*t1 = 0.0f;
		*mp1 = s1;
		float dist = CalcPointLineDist(*mp1, s2, e2, mp2, t2);
		return dist;
	}
	//2直線が平行でない
	float dv1v2 = v1.Dot(v2);
	float dv1v1 = v1.Dot(v1);
	float dv2v2 = v2.Dot(v2);
	CVector vs2s1 = s1 - s2;
	//比率1を求める
	*t1 = (dv1v2 * v2.Dot(vs2s1) - dv2v2 * v1.Dot(vs2s1)) / (dv1v1 * dv2v2 - dv1v2 * dv1v2);
	//交点1を求める
	*mp1 = s1 + v1 * *t1;
	//比率2を求める
	*t2 = v2.Dot(*mp1 - s2) / dv2v2;
	//交点2を求める
	*mp2 = s2 + v2 * *t2;
	//最短距離を返す
	return (*mp2 - *mp1).Length();
}

//2線分間の最短距離
float CalcSegmentSegmentDist
(
	const CVector& s1, const CVector& e1, //線分1
	const CVector& s2, const CVector& e2, //線分2
	CVector* mp1, //最短線の端点1(始点や終点になることもある)
	CVector* mp2 //最短線の端点2(始点や終点になることもある)
)

{
	float dist = 0, t1, t2;
	//----------------------------------------------------------------
	//とりあえず2直線間の最短距離,mp1,mp2,t1,t2を求めてみる
	dist = CalcLineLineDist(s1, e1, s2, e2, mp1, mp2, &t1, &t2);
	if (0.0f <= t1 && t1 <= 1.0f && 0.0f <= t2 && t2 <= 1.0f)
	{
		//mp1,mp2が両方とも線分内にあった
		return dist;
	}
	//mp1,mp2の両方、またはどちらかが線分内になかったので次へ
	//mp1,t1を求め直す ⇒
	//t2を0~1にクランプしてmp2からs1.vに垂線を降ろしてみる
	clamp0to1(t2);
	*mp2 = s2 + (e2 - s2) * t2;
	dist = CalcPointLineDist(*mp2, s1, e1, mp1, &t1);
	if (0.0f <= t1 && t1 <= 1.0f)
	{
		//mp1が線分内にあった
		return dist;
	}
	//mp1が線分内になかったので次へ
	//mp2,t2を求め直す ⇒
	//t1を0~1にクランプしてmp1からs2.vに垂線を降ろしてみる
	clamp0to1(t1);
	*mp1 = s1 + (e1 - s1) * t1;
	dist = CalcPointLineDist(*mp1, s2, e2, mp2, &t2);
	if (0.0f <= t2 && t2 <= 1.0f)
	{
		//mp2が線分内にあった
		return dist;
	}
	//mp2が線分内になかったので次へ
	//t2をクランプしてmp2を再計算すると、mp1からmp2までが最短
	clamp0to1(t2);
	*mp2 = s2 + (e2 - s2) * t2;
	return (*mp2 - *mp1).Length();
}

//点と三角形の面への射影(内部判定つき)
//p:点
//a,b,c:三角形の頂点
//outProj:射影点(pから三角形を含む面への垂線との交点座標)
//返り値:true:点が三角形の面内にある場合、false:点が三角形の面外にある場合
inline bool ProjectPointOnTriangle(const CVector& p, const CVector& a, const CVector& b, const CVector& c, CVector& outProj)
{
	CVector ab = b - a;
	CVector ac = c - a;
	CVector ap = p - a;
	float d00 = ab.Dot(ab);
	float d01 = ab.Dot(ac);
	float d11 = ac.Dot(ac);
	float d20 = ap.Dot(ab);
	float d21 = ap.Dot(ac);
	float denom = d00 * d11 - d01 * d01;
	if (denom == 0.0f) return false; // 退化三角形
	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0f - v - w;
	//全て0以上なら点は三角形の面内
	if (u >= 0 && v >= 0 && w >= 0)
	{
		outProj = a * u + b * v + c * w;
		return true;
	}
	outProj = a * u + b * v + c * w;
	return false;
}

//線分 vs 三角形(面距離)
//outSegPoint:線分側の最近接点
//outTriPoint:三角形側の最近接点
//返り値:距離の2乗(衝突している場合は0)
inline float SegmentTriangleDistanceSq(const CVector& p0, const CVector& p1, float cr, const CVector& a, const CVector& b, const CVector& c, CVector& outSegPoint, CVector& outTriPoint)
{
	CVector segDir = p1 - p0; //線分の向き
	// 1. 線分を無限直線として三角形面と交差するか
	CVector n = (b - a).Cross(c - a);
	float denom = n.Dot(segDir);
	//線分が三角形面に平行ではない?
	if (fabsf(denom) > 1e-6f)
	{
		float dot = n.Dot(a - p0);
		float t = dot / denom;
		//線分は三角形面を貫く?
		if (t >= 0.0f && t <= 1.0f)
		{
			//面との交点p
			CVector p = p0 + segDir * t;
			CVector proj;
			//交点が三角形内か?
			if (ProjectPointOnTriangle(p, a, b, c, proj))
			{
				//p0が面の裏か?
				if (dot >= 0.0f)
					outSegPoint = p0;
				else
					outSegPoint = p1;
				//面の交点を設定
				outTriPoint = proj;
				return 0.0f; // 交差の場合は距離0
			}
		}
		else
		{ //貫いてない時
			CVector proj0, proj1;
			n.Normalize();
			//p0と三角形面までの距離s
			float s = n.Dot(p0 - a);
			//点p0が三角形内か?
			if (ProjectPointOnTriangle(p0, a, b, c, proj0))
			{
				//p1と三角形面までの距離t
				float t = n.Dot(p1 - a);
				//点p1が三角形内か?
				if (ProjectPointOnTriangle(p1, a, b, c, proj1))
				{
					//面までの距離が近い方を採用
					if (s < t)
					{
						outSegPoint = p0;
						outTriPoint = proj0;
					}
					else
					{
						outSegPoint = p1;
						outTriPoint = proj1;
					}
					//点と三角形との距離の2乗を求め、半径より距離が小さければ距離の2乗を戻す
					float sq = (outSegPoint - outTriPoint).Dot(outSegPoint - outTriPoint);
					if (sq <= cr * cr)
						return sq;

				}
				else
				{ //p1が三角形外の場合、p0を採用
					outSegPoint = p0;
					outTriPoint = proj0;
					float sq = (outSegPoint - outTriPoint).Dot(outSegPoint - outTriPoint);
					if (sq <= cr * cr)
						return sq;

				}
			}
			else
			{
				float t = n.Dot(p1 - a);
				if (ProjectPointOnTriangle(p1, a, b, c, proj1))
				{
					outSegPoint = p1;
					outTriPoint = proj1;
					float sq = (outSegPoint - outTriPoint).Dot(outSegPoint - outTriPoint);
					if (sq <= cr * cr)
						return sq;
				}
			}
		}
	}
	// 2. 面内に落ちない → エッジ距離で決まる
	float best = FLT_MAX;
	//ラムダ式
	//線分p0p1と線分e0e1の最短距離をbestに保存する
	auto testEdge = [&](const CVector& e0, const CVector& e1)
		{
		CVector s, t;
		float d = CalcSegmentSegmentDist(p0, p1, e0, e1, &s, &t);
		float dsq = (s - t).Dot(s - t);
		if (dsq < best)
		{
			best = dsq;
			outSegPoint = s;
			outTriPoint = t;
		}
		};
	testEdge(a, b);
	testEdge(b, c);
	testEdge(c, a);
	return best;
}

bool CCollider::CollisionCapsuleCapsule(CCollider* m, CCollider* o, CVector* adjust)
{
	CVector mp1, mp2;
	float radius = m->mRadius + o->mRadius;
	*adjust = CVector();
	if (CalcSegmentSegmentDist(m->mV[0], m->mV[1], o->mV[0], o->mV[1], &mp1, &mp2) < radius)
	{
		*adjust = mp1 - mp2;
		float len = radius - adjust->Length();
		*adjust = adjust->Normalize() * len;
		return true;
	}
	return false;
}

bool CCollider::CollisionTriangleCapsule(const CVector& t0, const CVector& t1, const CVector& t2, const CVector& cs, const CVector& ce, float cr, CVector* adjust)
{
	CVector pointCaps, pointTri;
	// 線分 vs 三角形の最近接点を取得
	float distSq = SegmentTriangleDistanceSq(cs, ce, cr, t0, t1, t2, pointCaps,	pointTri);
	//半径より遠いので当たってない
	if (distSq >= cr * cr)
	{
		*adjust = CVector();
		return false;
	}
	// 最近接点間の距離
	CVector diff;
	// penetration depth
	float penetration;
	float dist;
	if (distSq == 0.0f)
	{
		diff = pointTri - pointCaps;
		dist = diff.Length();
		penetration = dist + cr;
	}
	else
	{
		diff = pointCaps - pointTri;
		dist = diff.Length();
		penetration = cr - dist;
	}
	// 押し戻し方向
	CVector dir;
	if (dist > 1e-6f)
	{
		// 最近接点の差分方向(最も安定)
		dir = diff * (1.0f / dist);
	}
	else
	{
		// 退避:三角形の法線方向
		CVector triN = (t1 - t0).Cross(t2 - t0);
		if (triN.Dot(triN) > 1e-6f)
			dir = triN.Normalize();
		else
			dir = CVector(0.0f, 1.0f, 0.0f); // 完全退化三角形

	}
	// 最終押し戻しベクトル
	*adjust = dir * penetration;
	return true;
}

bool CCollider::CollisionTriangleCapsule(CCollider* triangle, CCollider* c, CVector* adjust)
{
	CVector v0, v1, v2;
	//各コライダの頂点をワールド座標へ変換
	v0 = triangle->mV[0] * *triangle->mpMatrix;
	v1 = triangle->mV[1] * *triangle->mpMatrix;
	v2 = triangle->mV[2] * *triangle->mpMatrix;
	return CollisionTriangleCapsule(v0, v1, v2, c->mV[0], c->mV[1], c->mRadius, adjust);
}

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
