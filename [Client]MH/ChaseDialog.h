// ChaseDialog.h: interface for the CChaseDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHASEDIALOG_H__7D808836_FB10_4B68_8E74_0F797F496450__INCLUDED_)
#define AFX_CHASEDIALOG_H__7D808836_FB10_4B68_8E74_0F797F496450__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "MinimapDlg.h"

class cStatic;
class cImage;
class cTextArea;



class CChaseDialog : public cDialog
{
	cImage				m_pIconImage;
	MINIMAPIMAGE		m_MapImage;
	cImageSelf			m_WantedImage;
	cStatic*			m_pMap;
	cTextArea*			m_TextArea;

	BOOL				m_bActive;
	VECTOR2				m_TargetPos;
	MAPTYPE				m_MapNum;
	MAPTYPE				m_EventMapNum;
	char				m_WantedName[MAX_NAME_LENGTH+1];

public:
	CChaseDialog();
	virtual ~CChaseDialog();

	void Linking();
	void Render();
	void SetActive( BOOL val );
	
	BOOL InitMiniMap( MAPTYPE MapNum, WORD PosX, WORD PosY, char* strName, MAPTYPE EventMapNum  );
	BOOL LoadMinimapImageInfo( MAPTYPE MapNum );
};

#endif // !defined(AFX_CHASEDIALOG_H__7D808836_FB10_4B68_8E74_0F797F496450__INCLUDED_)
