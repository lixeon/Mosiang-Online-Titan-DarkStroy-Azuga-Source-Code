// CharaChangeDlg.h: interface for the CCharaChangeDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARCHANGEDLG_H__6AF9E3E8_8C9F_4A17_ACD7_F555276CDBBB__INCLUDED_)
#define AFX_CHARCHANGEDLG_H__6AF9E3E8_8C9F_4A17_ACD7_F555276CDBBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "interface/cDialog.h"

class cStatic;
class cButton;
class cGuageBar;



class CCharChangeDlg : public cDialog
{
	cStatic*		m_pName;
	cStatic*		m_pSex;
	cStatic*		m_pHair;
	cStatic*		m_pFace;
	
	cButton*		m_pSexBtn[2];
	cGuageBar*		m_pHeight;
	cGuageBar*		m_pWidth;

	CHARACTERCHANGE_INFO		m_CharacterInfoBackup;
	CHARACTERCHANGE_INFO		m_CharacterInfo;

	DWORD			m_ItemPos;

	BOOL			m_bShapeChange;

protected:
	void RefreshCharacter();

public:
	CCharChangeDlg();
	virtual ~CCharChangeDlg();

	void Linking();
	void SetActive( BOOL val );
	void Process();
	void SetItemInfo( DWORD Pos )				{	m_ItemPos = Pos;	}
	void SetCharacterInfo( CHARACTER_TOTALINFO* pInfo );
	void SetShapeChange( BOOL val )				{	m_bShapeChange = val;	}

	void Reset( BOOL bSave );
	void RefreshCharacterShape();
	void ChangeSexType( BOOL bPrev );
	void ChangeHairType( BOOL bPrev );
	void ChangeFaceType( BOOL bPrev );

	void CharacterChangeSyn();
};

#endif // !defined(AFX_CHARACHANGEDLG_H__6AF9E3E8_8C9F_4A17_ACD7_F555276CDBBB__INCLUDED_)
