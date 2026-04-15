#include "CEnemy3.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CPlayer.h"

#define OBJ "res\\f16.obj"	//モデルのファイル
#define MTL "res\\f16.mtl"	//モデルのマテリアルファイル
#define HP 3	//耐久値

CModel CEnemy3::sModel;	//モデルデータ作成

//デフォルトコンストラクタ
CEnemy3::CEnemy3()
	: CCharacter3(1)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.4f)
	, mHp(HP)
{
	//モデルが無いときは読み込む
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(OBJ, MTL);
	}
	//モデルのポインタ設定
	mpModel = &sModel;
}

//コンストラクタ
//CEnemy3(位置, 回転, 拡縮)
CEnemy3::CEnemy3(const CVector& position, const CVector& rotation, const CVector& scale)
	: CEnemy3()	//デフォルトコンストラクタを実行する
{
	//位置、回転、拡縮を設定する
	mPosition = position;	//位置の設定
	mRotation = rotation;	//回転の設定
	mScale = scale;	//拡縮の設定
	CTransform::Update();	//行列の更新
}

//更新処理
void CEnemy3::Update()
{
	//HPが0以下の時　撃破
	if (mHp <= 0)
	{
		mHp--;
		//15フレーム毎にエフェクト
		if (mHp % 15 == 0)
		{
			//エフェクト生成
			new CEffect(mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		}
		//下降させる
		mPosition = mPosition - CVector(0.0f, 0.03f, 0.0f);
		CTransform::Update();
		return;
	}

	//プレイヤーのポインタが0以外の時
	CPlayer* player = CPlayer::Instance();
	if (player != nullptr)
	{
		//プレイヤーまでのベクトルを求める
		CVector vp = player->Position() - mPosition;
		float dz = vp.Dot(mMatrixRotate.VectorZ());
		//左ベクトルとの内積を求める
		float dx = vp.Dot(mMatrixRotate.VectorX());
		//上ベクトルとの内積を求める
		float dy = vp.Dot(mMatrixRotate.VectorY());
		float dist = vp.Length();

		if (dz > 0.0f && dist < 30.0f)
		{
		    //X軸のズレが2.0未満
			if (-2.0f < dx && dx < 2.0f)
			{
				//Y軸のズレが2.0未満
				if (-2.0f < dy && dy < 2.0f)
				{
					//弾を発射します
					CBullet* bullet = new CBullet();
					bullet->Set(0.1f, 1.5f);
					bullet->Position(CVector(0.0f, 0.0f, 10.0f) * mMatrix);
					bullet->Rotation(mRotation);
					bullet->Update();
				}
			}
		}
	}
}

//衝突処理
//Collision(コライダ1, コライダ2)
void CEnemy3::Collision(CCollider* m, CCollider* o)
{
	switch (o->Type())
	{
	case CCollider::EType::ESPHERE:
		if (CCollider::Collision(m, o))
		{
			mHp--;	//ヒットポイントの減算
			new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		}
		break;
	case CCollider::EType::ETRIANGLE:
		CVector adjust;
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			mPosition = mPosition + adjust;

			//撃破で地面に衝突すると無効
			if (mHp <= 0)
			{
				mEnabled = false;
			}
		}
		break;
	}
}

void CEnemy3::Collision()
{
	mCollider.ChangePriority();
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}