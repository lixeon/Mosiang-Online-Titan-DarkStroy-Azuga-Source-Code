// GuildInviteDialog.h: interface for the CGuildInviteDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDINVITEDIALOG_H__170A81FF_38F2_4063_8AB5_7498C5E96FB7__INCLUDED_)
#define AFX_GUILDINVITEDIALOG_H__170A81FF_38F2_4063_8AB5_7498C5E96FB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"

class cTextArea;
class CGuildInviteDialog : public cDialog  
{
	cTextArea * m_pInviteMsg;
public:
	CGuildInviteDialog();
	virtual ~CGuildInviteDialog();
	
	void Linking();
	//void SetInfo(char* GuildName, char* MasterName);
	//SW060713 ¹®ÇÏ»ý
	void SetInfo(char* GuildName, char* MasterName, int FlgKind);

};

#endif // !defined(AFX_GUILDINVITEDIALOG_H__170A81FF_38F2_4063_8AB5_7498C5E96FB7__INCLUDED_)
