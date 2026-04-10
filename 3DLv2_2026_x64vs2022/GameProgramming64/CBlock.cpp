#include "CBlock.h"

#define TEXCOORD 0, 48, 128, 80 //テクスチャ座標

CBlock::CBlock(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBLOCK;
}

