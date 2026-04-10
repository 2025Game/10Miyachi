#ifndef CSHADOWSHADER_H
#define CSHADOWSHADER_H

#include "CMyShader.h"

class CShadowShader : public CMyShader {
	//•`‰æˆ—
	void Render(const CModelX* model, const CMesh* mesh, const CMatrix* pCombinedMatrix);
public:
	//•`‰æˆ—
	void Render(CModelX* model, CMatrix* combinedMatrix);
	void Render330(const CMesh* mesh, const CMatrix* skin_matrix, int skin_matrix_size);
};

#endif
