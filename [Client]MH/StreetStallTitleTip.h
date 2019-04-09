#ifndef __STREETSTALL_TITLETIP__
#define __STREETSTALL_TITLETIP__

#include "BalloonOutline.h"

#define SS_TITLE_LINE 2
#define SS_TITLE_LENGTH 32

class CStreetStallTitleTip : public cBalloonOutline
{
public:
	CStreetStallTitleTip();
	virtual ~CStreetStallTitleTip();

	BOOL IsActive()							{ return m_bActive;	}
	BOOL IsShow()							{ return m_bShow;	}
	void SetActive(BOOL val)				{ m_bActive = val;	}
	void Show(BOOL val)						{ m_bShow	= val;	}
	BOOL Render(LONG absX, LONG absY);
	void SetTall(LONG	Tall)				{ m_lTall = Tall;	}
	void SetPosX(LONG	pos)				{ m_lPosX = pos;	}
	void SetTitle(char * chatMsg);

	void SetFGColor(DWORD col)				{ m_fgColor = col;	}
	
	int GetLineCount()						{ return m_nLineNum; }

protected:
	BOOL m_bActive;
	BOOL m_bShow;
	LONG m_lTall;
	LONG m_lPosX;
	char m_sztitle[MAX_STREETSTALL_TITLELEN+1];
	int	 m_TitleLen;
	// font
	BOOL m_wFontIdx;
	DWORD m_fgColor;
	
	int m_nLineNum;
	char m_szChatMsg[SS_TITLE_LINE][SS_TITLE_LENGTH+1];
	int	 m_ChatMsgLen[SS_TITLE_LINE];
};

#endif //__STREETSTALL_TITLETIP__
