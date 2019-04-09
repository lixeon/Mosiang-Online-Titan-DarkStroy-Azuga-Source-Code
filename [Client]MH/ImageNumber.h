#ifndef _IMAGENUMBER_H
#define _IMAGENUMBER_H


#include "./interface/cImage.h"


class CImageNumber
{
protected:

	VECTOR2 m_vPos;
	VECTOR2 m_vScale;
//	float	m_x;
//	float	m_y;
	cImage	m_ImageNum[10];
	DWORD	m_dwNumber;
	DWORD	m_dwLimitCipher;	//ÀÚ¸®¼ö
	BOOL	m_bFillZero;

	int		m_nNumWidth;
	int		m_nGap;

///È¿°ú
	BOOL	m_bNumberChange;
	DWORD	m_dwFadeOutStartTime;
	DWORD	m_dwFadeOutTime;
	BOOL	m_bFadeOut;

public:

	CImageNumber();
	virtual ~CImageNumber();

	void Init( int nNumWidth, int nGap );
	void SetNumber( DWORD dwNum );
	void SetLimitCipher( DWORD dwCipher );
	void SetFillZero( BOOL bFillZero ) { m_bFillZero = bFillZero; }
	void SetPosition( int x, int y );
	void Render();

	void SetScale( float x, float y ) { m_vScale.x = x; m_vScale.y = y;	}
//ÀÓ½Ã
	void SetFadeOut( DWORD dwTime );
	BOOL IsNumberChanged() { return m_bNumberChange; }
};


#endif