#pragma once

#include "../4DyuchiGRX_common/typedef.h"

struct CLIP_BOX
{
	int			dwLeft;
	int			dwTop;
	int			dwRight;
	int			dwBottom;
};

class CImage
{
	DWORD				m_dwWidth;
	DWORD				m_dwHeight;
	char*				m_pRawImage;
	DWORD				m_dwColorDepth;
	void				Destroy();
public:
	DWORD				GetWidth()		{return m_dwWidth;}
	DWORD				GetHeight()		{return m_dwHeight;}
	char*				GetRawImage()	{return m_pRawImage;}
	BOOL				Resize(DWORD dwWidth,DWORD dwHeight,DWORD dwBPS);
	BOOL				LoadTga(char* szFileName,DWORD dwBPS);
	BOOL				LoadTga(void* pFP,DWORD dwBPS);
	BOOL				LoadTIF(char* filename,DWORD dwBPS);
	CImage();
	~CImage();
};
