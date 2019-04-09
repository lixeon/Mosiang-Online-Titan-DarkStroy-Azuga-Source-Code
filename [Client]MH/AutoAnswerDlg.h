#pragma once

#include "./Interface/cDialog.h"
#include "cImageSelf.h"

class cTextArea;
class cStatic;
class cButton;
class cImage;

class CAutoAnswerDlg : public cDialog
{
protected:
	cTextArea*	m_pTextAreaDesc;
	cStatic*	m_pStcQuestion;
	cStatic*	m_pStcAnswer;
	cStatic*	m_pStcTime;
	cButton*	m_pBtnColor[4];

	DWORD		m_dwEndTime;

	VECTOR2		m_v2BtnPos[4];
	DWORD		m_dwOriginalPos[4];

	BOOL		m_bAnswerStart;
	DWORD		m_dwAnswer[4];
	int			m_nAnswerPos;

	cImageSelf	m_Question;

public:
	CAutoAnswerDlg(void);
	~CAutoAnswerDlg(void);

	void Linking();
	virtual void SetActive( BOOL val );
	void SetActiveWithTime( BOOL val, DWORD dwTime );
	DWORD ActionEvent( CMouse * mouseInfo );
	void OnActionEvent( LONG lId, void * p, DWORD we );

	void Shuffle( DWORD* pColorTable );
	void SetQuestion( char* strQuestion );
	void Retry();

	virtual void Render();

	void SaveImage( BYTE* pRaster );
	
	void SetActiveTestClient();
};