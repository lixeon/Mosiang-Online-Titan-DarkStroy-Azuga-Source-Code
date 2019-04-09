// OBalloonChat.h: interface for the COBalloonChat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBALLOONCHAT_H__6754B17E_55C3_4695_93CC_89FAC1CB3EF1__INCLUDED_)
#define AFX_OBALLOONCHAT_H__6754B17E_55C3_4695_93CC_89FAC1CB3EF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BalloonOutline.h"

#define MAX_CHATBALLON_TEXT_LINE	3
#define MAX_CHATBALLON_TEXT_LENGTH	24
#ifdef _TL_LOCAL_
#define MAX_CHATBALLON_TEXT_EXTENT	144
#endif

class COBalloonChat  : public cBalloonOutline
{
public:

	COBalloonChat();
	virtual ~COBalloonChat();
	
	BOOL IsActive()							{ return m_bActive;	}
	BOOL IsShow()							{ return m_bShow;	}
	void SetActive(BOOL val);
	void Show(BOOL val);
	BOOL Render(LONG absX, LONG absY);
	void SetTall(LONG	Tall)				{ m_lTall = Tall;	}
	void SetPosX(LONG	pos)				{ m_lPosX = pos;	}
	void SetChat(char * chatMsg);

	void SetFGColor(DWORD col)				{ m_fgColor = col;	}
	void SetDisappearTime(DWORD disappearTime);

	LONG GetBalloonHeight();
	
protected:
	void InitChatMsg();

protected:

	BOOL m_bActive;
	BOOL m_bShow;

	LONG m_lTall;
	LONG m_lPosX;

//	char m_szChatMsg[MAX_CHAT_LENGTH+1];
	
#ifdef _TL_LOCAL_
	char m_szChatMsg[MAX_CHATBALLON_TEXT_LINE][256];	//256?? ?????
#else
	char m_szChatMsg[MAX_CHATBALLON_TEXT_LINE][MAX_CHATBALLON_TEXT_LENGTH+1];
#endif
	int	 m_ChatMsgLen[MAX_CHATBALLON_TEXT_LINE];
	int	 m_nLineNum;
	// font
	BOOL m_wFontIdx;
	DWORD m_fgColor;

	DWORD m_dwStartTime;
	DWORD m_dwDisappearTime;

};

#endif // !defined(AFX_OBALLOONCHAT_H__6754B17E_55C3_4695_93CC_89FAC1CB3EF1__INCLUDED_)
