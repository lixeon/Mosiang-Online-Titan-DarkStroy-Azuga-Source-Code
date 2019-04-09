//////////////////////////////////////////////////////////////////////
// class name : cIcon 
// Lastest update : 2002.10.29. by taiyo
//////////////////////////////////////////////////////////////////////

#ifndef _cICON_H_
#define _cICON_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cDialog.h"

enum EICONTYPE
{
	eIconType_NotInited		= 0,
	eIconType_AllItem		= 0x00000001,
	eIconType_Skill			= 0x00000002,
//KES EXCHANGE 030920 //±³È¯Ã¢ ¹× ³ëÁ¡»ó¿¡ ¸µÅ©µÈ ¾ÆÀÌÅÛ
	eIconType_LinkedItem	= 0x00000003,
	eIconType_Ability		= 0x00000004,
};

class cIconDialog;

class cIcon : public cDialog  
{

protected:

	WORD m_CellX;
	WORD m_CellY;

	DWORD m_IconType;
	DWORD m_dwData;
	
//KES EXCHANGE 031001
	BOOL	m_bLock;
	cImage	m_LockImage;
//

	// magi82 - Titan(070424) 타이탄 내구도가 낮을때 블리팅해주는 변수들
	cImage	m_TitanEnduranceImage;
	BOOL	m_TitanEnduranceFlag;

	cPtrList m_LinkDialogList;
	
public:
	cIcon();
	virtual ~cIcon();

	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual void Render();
	virtual void Render(VECTOR2* pPos);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	
	inline void SetIconType(DWORD type){ m_IconType=type; }
	inline DWORD GetIconType(){ return m_IconType; }

	inline void SetData(DWORD dwData)	{	m_dwData = dwData;	}
	inline DWORD GetData()	{	return m_dwData;	}


	void AddLinkIconDialog(cIconDialog* pDialog);
	void RemoveLinkIconDialog(cIconDialog* pDialog);

//KES EXCHANGE 031001
	virtual void SetLock( BOOL bLock );
	BOOL IsLocked() { return m_bLock; }
	// LBS Ãß°¡ 
	void SetLockImage( cImage* pImage ) { m_LockImage = *pImage; }
//

	// magi82 - Titan(070424) 타이탄 내구도가 낮을때 블리팅해주는 함수
	void SetTitanEndurance( BOOL bFlag )	{	m_TitanEnduranceFlag = bFlag;	}
	BOOL GetTitanEndurance()	{	return m_TitanEnduranceFlag;	}

	void SetCellPosition(WORD x,WORD y)
	{
		m_CellX = x;
		m_CellY = y;
	}
	WORD GetCellX()	{	return m_CellX;	}
	WORD GetCellY()	{	return m_CellY;	}
	

	virtual BOOL CanMoveToDialog( cDialog* targetdlg )	{	return TRUE;	}
	virtual BOOL CanDelete()							{	return TRUE;	}	
};

#endif // _cICON_H_
