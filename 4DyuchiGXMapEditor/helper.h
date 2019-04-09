#pragma once


#include "../4DyuchiGRX_common/typedef.h"

void CopyRect(char* pDest,DWORD dwDestWidth,POINT* pt,char* pSrc,DWORD dwSrcWidth,RECT* pRect,DWORD dwBytesPerPixel,DWORD dwNum);
BOOL OpenMODFileName(char* szFileName,DWORD dwMaxLen);
BOOL OpenMODFCHRFileName(char* szFileName,DWORD dwMaxLen);


