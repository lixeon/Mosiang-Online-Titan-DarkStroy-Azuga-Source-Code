//
////Add 060803 by wonju
//


#ifndef _GUILDPLUSTIMEDLG__
#define _GUILDPLUSTIMEDLG__

#include "./Interface/cDialog.h"

class cStatic;
class cListDialog;

class CGuildPlusTimeDlg :
	public cDialog
{
private:
	cStatic*		m_pCurrentHavePoint;
	cListDialog*    m_pPlusItemList;
	int             m_CurrentSelectedItem; //현재 선택된 아이템의 번호

public:
	void Linking();
	void OnActionEvnet(LONG lId, void * p, DWORD we);
	void SetActive(BOOL val);
	virtual DWORD ActionEvent(CMouse* mouseInfo);

	int GetCurrentSelectedItemNumber(){ return m_CurrentSelectedItem; }

	void SetGuildPointText(DWORD GuildPoint);
	void LoadPlustimeList();
	CGuildPlusTimeDlg();
	~CGuildPlusTimeDlg();
};

#endif // end of _GUILDPLUSTIMEDLG__