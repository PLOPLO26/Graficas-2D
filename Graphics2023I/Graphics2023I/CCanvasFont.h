#pragma once
#include "CCanvas.h"
#include "Matrix4D.h"
class CCanvasFont :
    public CCanvas
{
public:
    static PIXEL Alpha(PIXEL);
    static CCanvasFont* LoadFont(const char* pszFileName);
    void PutText(CCanvas* pTarget, MATRIX4D M, const char* pszText);
};

