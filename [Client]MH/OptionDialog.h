#ifndef _OPTIONDIALOG_H
#define _OPTIONDIALOG_H


#include "./Interface/cTabDialog.h"
#include "OptionManager.h"

//class cButton;

class COptionDialog : public cTabDialog
{
protected:

	sGAMEOPTION		m_GameOption;
//	cButton*		m_pBtnPreview;

protected:

	void UpdateData( BOOL bSave );
public:

	COptionDialog();
	virtual ~COptionDialog();

	virtual void Add(cWindow * window);
	virtual void SetActive(BOOL val);

	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);

	void DisableGraphicTab(BOOL bDisable);

#ifndef _JAPAN_LOCAL_	
	// 2005.12.28ÀÏ Ãß°¡ ´« On/Off
	int GetEffectSnow() { return m_GameOption.nEffectSnow;}
#endif
};





#endif
