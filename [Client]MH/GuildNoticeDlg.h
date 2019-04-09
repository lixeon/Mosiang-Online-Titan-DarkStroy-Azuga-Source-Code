#pragma once
#include "./Interface/cDialog.h"

class cTextArea;

class CGuildNoticeDlg :
	public cDialog
{
	cTextArea * m_pNoticeText;

public:
	CGuildNoticeDlg(void);
	~CGuildNoticeDlg(void);

	void Linking();
	void OnActionEvnet(LONG lId, void * p, DWORD we);
	void SetActive(BOOL val);
};
