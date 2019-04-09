#ifndef _MACRODIALOG_H_
#define _MACRODIALOG_H_


#include "MacroManager.h"
#include "./interface/cDialog.h"

class cEditBox;

class CMacroDialog  : public cDialog
{

protected:

	sMACRO			m_MacroKey[ME_COUNT];
	cEditBox*		m_pMacroKeyEdit[ME_COUNT];		//¼¼ÆÃ³»¿ë È­¸é Ãâ·Â¿ë
//	cPushupButton*	m_pModeBtn[2];					//¸ðµå¼±ÅÃ ¹öÆ°
	int				m_nMode;						//Ã¤ÆÃ¿ì¼±(0) or not(1)

	cEditBox*		m_pFocusEdit;					//Æ÷Ä¿½º¸¦ °¡Áø ¿¡µðÆ®¹Ú½º
	int				m_nCurMacro;					//ÇöÀç ¼¼ÆÃÁßÀÎ ¸ÅÅ©·Î
	sMACRO			m_tempMacroKey;					//ÇöÀç ¼¼ÆÃÁßÀÎ ¸ÅÅ©·ÎÀÇ ¿øº»À» Àá½Ã ÀúÀå

	BOOL			m_bCombining;
	BOOL			m_bChanged;						//¼³Á¤ÀÌ º¯°æµÇ¾î¼­ ÀúÀå¿©ºÎ È®ÀÎ ÇÊ¿ä

protected:

	void SetNextEditFocus();
	BOOL IsAlreadySet( sMACRO* pMacro );
	void UpdateData( BOOL bSave, BOOL bDefault = FALSE );
	
public:

	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual void SetActive(BOOL val);
	virtual DWORD ActionEvent(CMouse * mouseInfo);

	void OnActionEvent(LONG lId, void * p, DWORD we);
	void Linking();

	void SetMode( int nMode ) { m_nMode = nMode; }	//SetActiveÇÏ±âÀü¿¡ ¸ÕÀú ¼³Á¤ÇØÁÖ¾î¾ßÇÑ´Ù.

	void ConvertMacroToText( char* str, sMACRO* pMacro );
};



#endif	//#ifndef _MACRODIALOG_H_