// GuildLevelUpDialog.h: interface for the CGuildLevelUpDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDLEVELUPDIALOG_H__E03C2DA3_728E_4724_B5B3_ED2EEA9D7A3F__INCLUDED_)
#define AFX_GUILDLEVELUPDIALOG_H__E03C2DA3_728E_4724_B5B3_ED2EEA9D7A3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"

class cStatic;
class CGuildLevelUpDialog : public cDialog  
{
	cStatic * m_pLevelupNotComplete[4];
	cStatic * m_pLevelupComplete[4];
	cStatic * m_pLevel[5];
public:
	CGuildLevelUpDialog();
	virtual ~CGuildLevelUpDialog();
	void Linking();
	void SetLevel(BYTE level);
	virtual void SetActive(BOOL val);
};

#endif // !defined(AFX_GUILDLEVELUPDIALOG_H__E03C2DA3_728E_4724_B5B3_ED2EEA9D7A3F__INCLUDED_)
