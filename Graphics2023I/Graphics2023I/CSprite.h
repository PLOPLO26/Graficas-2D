#pragma once
#include "CCanvas.h"
class CSprite
{
protected:
	int m_nSpritesX;
	int m_nSpritesY;
	CCanvas* m_pCanvas;
public:
	static CCanvas::PIXEL ColorKeyWhite(CCanvas::PIXEL Color);
	static CCanvas::PIXEL ColorKeyMagenta(CCanvas::PIXEL Color);
	bool LoadSprites(const char* pszFileName, int nSpritesX, int nSpritesY,
					CCanvas::PIXEL (*pAlpha)(CCanvas::PIXEL));
	void DrawSprite(CCanvas* pTarget, MATRIX4D M, int id);
	CCanvas* GetCanvas() { return m_pCanvas; }
	void Uninitialize();
};

