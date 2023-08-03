#include "CSprite.h"

CCanvas::PIXEL CSprite::ColorKeyWhite(CCanvas::PIXEL Color)
{
	CCanvas::PIXEL P = { 255,255,255,255 };
	if (Color.r == P.r && Color.g == P.g && Color.b == P.b)
	{
		Color.a = 0;
		return Color;
	}
	return Color;
}

CCanvas::PIXEL CSprite::ColorKeyMagenta(CCanvas::PIXEL Color)
{
	CCanvas::PIXEL P = { 255,0,255,255 };
	if (Color.r == P.r && Color.g == P.g && Color.b == P.b)
	{
		Color.a = 0;
		return Color;
	}
	return Color;
}

bool CSprite::LoadSprites(const char* pszFileName, int nSpritesX, int nSpritesY,
	CCanvas::PIXEL (*pAlpha)(CCanvas::PIXEL))
{
	m_pCanvas = CCanvas::CreateCanvasFromFile(pszFileName, pAlpha);
	if (!m_pCanvas)
		return false;
	m_nSpritesX = nSpritesX;
	m_nSpritesY = nSpritesY;
	return true;
}

void CSprite::Uninitialize()
{
	if (m_pCanvas)
		CCanvas::Destroy(m_pCanvas);
	m_pCanvas = nullptr;
}

void CSprite::DrawSprite(CCanvas* pTarget, MATRIX4D M, int id)
{
	int row = id / m_nSpritesX;
	int col = id % m_nSpritesY;
	float u, v;
	u = (float)col / m_nSpritesX;
	v = (float)row / m_nSpritesY;
	const float ox = 1.0f / m_nSpritesX;
	const float oy = 1.0f / m_nSpritesY;

	CCanvas::VERTEX Quad[] =
	{
		{{-0.5f, -0.5f, 0, 1.0f},{ u    , v    ,  0, 1.0f}},
		{{ 0.5f, -0.5f, 0, 1.0f},{u + ox, v ,     0, 1.0f}},
		{{-0.5f,  0.5f, 0, 1.0f},{ u,     v + oy ,0, 1.0f}},
		{{-0.5f,  0.5f, 0, 1.0f},{ u,     v + oy, 0, 1.0f}},
		{{ 0.5f, -0.5f, 0, 1.0f},{u + ox, v ,     0, 1.0f}},
		{{ 0.5f,  0.5f, 0, 1.0f},{u + ox, v + oy, 0, 1.0f}}
	};

	CCanvas::VERTEX Trns[6];
	for (int i = 0; i < 6; i++)
	{
		Trns[i].Position = Quad[i].Position* M;
		Trns[i].TexCoord = Quad[i].TexCoord;
	}
	pTarget->ResetLimits();
	pTarget->InverseTextureMapping(Trns, m_pCanvas, &CCanvas::PointSampler);
	pTarget->ResetLimits();
	pTarget->InverseTextureMapping(Trns + 3, m_pCanvas, &CCanvas::PointSampler);
	
}