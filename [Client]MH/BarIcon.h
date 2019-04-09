// BarIcon.h: interface for the CBarIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BARICON_H__C02CA210_4AA4_41DE_B6A6_651B1019B9EF__INCLUDED_)
#define AFX_BARICON_H__C02CA210_4AA4_41DE_B6A6_651B1019B9EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cPushupButton.h"

class BARICON_INFO
{
public:
	BYTE	type;
	WORD	kind;
	POSTYPE barPos;
	int		nToolTipMsg;
};

class CBarIcon : public cPushupButton  
{
public:
	CBarIcon();
	virtual ~CBarIcon();
	virtual void Render();
	void InitInfo(BARICON_INFO * info);
	BARICON_INFO * GetInfo() { return &m_Info; }

	virtual DWORD ActionEvent(CMouse * mouseInfo);
	POSTYPE GetData() { return m_pos; }
	void SetData(POSTYPE post) { m_pos = post; }

	//debug....//erase
//	cImage* GetCurImage() { return m_pCurImage; }
	
//	void SetAlramImage( cImage* pImg );
	void SetAlram( BOOL bAlram ) { m_bAlram = bAlram; }
	
private:
	BARICON_INFO	m_Info;
	POSTYPE			m_pos;
	
	BOOL			m_bCaptionHit;	//KES
	
//alram
	BOOL			m_bAlram;
	cImage			m_ImgAlram[3];
	
	cImage			m_ImgSpecialAlram[3];


};

#endif // !defined(AFX_BARICON_H__C02CA210_4AA4_41DE_B6A6_651B1019B9EF__INCLUDED_)
