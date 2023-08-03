#include "CCanvasFont.h"

CCanvas::PIXEL CCanvasFont::Alpha(CCanvas::PIXEL Color)
{
	PIXEL P = Color;
	P.a = P.g; //Canal verde, decide alfa.
	return P;
}

CCanvasFont* CCanvasFont::LoadFont( const char* pszFileName)
{
	CCanvas* pSource = CreateCanvasFromFile(pszFileName, Alpha);
	if (pSource)
	{
		CCanvasFont* pNewFont = new CCanvasFont;
		pSource->Attach(pNewFont);
		delete pSource;
		return pNewFont;
	}
	return nullptr;
}
void CCanvasFont::PutText(CCanvas* pTarget, MATRIX4D M, const char* pszText)
{
	int pos = 0;
	while (*pszText)
	{
		unsigned char c = (unsigned char)*pszText;
		int row = (0xf0 & c)>>4;
		int col = (0x0f & c);
		float u, v;
		u = col / 16.0f;
		v = row / 16.0f;
		const float o = 1 / 16.0f;

		VERTEX Quad[] =
		{
			{{-0.5f, -0.5f, 0, 1.0f},{ u , v , 0, 1.0f}},
			{{ 0.5f, -0.5f, 0, 1.0f},{u + o, v , 0, 1.0f}},
			{{-0.5f,  0.5f, 0, 1.0f},{ u, v + o ,0, 1.0f}},
			{{-0.5f,  0.5f, 0, 1.0f},{ u, v + o, 0, 1.0f}},
			{{ 0.5f, -0.5f, 0, 1.0f},{u + o, v , 0, 1.0f}},
			{{ 0.5f,  0.5f, 0, 1.0f},{u + o, v + o,0, 1.0f}}
		};

		VERTEX Trns[6];
		for (int i = 0; i < 6; i++)
		{
			Trns[i].Position = Quad[i].Position *Translation(pos, 0, 0) * M;
			Trns[i].TexCoord = Quad[i].TexCoord;
		}
		pTarget->ResetLimits();
		pTarget->InverseTextureMapping(Trns, this, &CCanvas::PointSampler);
		pTarget->ResetLimits();
		pTarget->InverseTextureMapping(Trns + 3, this, &CCanvas::PointSampler);
		pszText++;
		pos++;
	}
}