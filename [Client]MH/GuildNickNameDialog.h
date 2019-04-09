// GuildNickNameDialog.h: interface for the CGuildNickNameDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDNICKNAMEDIALOG_H__3748DA6F_2067_4B6F_81F2_5CEEDA74A941__INCLUDED_)
#define AFX_GUILDNICKNAMEDIALOG_H__3748DA6F_2067_4B6F_81F2_5CEEDA74A941__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"

class cTextArea;
class cEditBox;

class CGuildNickNameDialog : public cDialog  
{
	cTextArea* m_pNickMsg;
	cEditBox* m_pNickName;

public:
	CGuildNickNameDialog();
	virtual ~CGuildNickNameDialog();
	virtual void SetActive(BOOL val);
	void Linking();
	void SetNickMsg(char* Name);
};

#endif // !defined(AFX_GUILDNICKNAMEDIALOG_H__3748DA6F_2067_4B6F_81F2_5CEEDA74A941__INCLUDED_)
