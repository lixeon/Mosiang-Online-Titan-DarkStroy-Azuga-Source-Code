// OBalloonName.h: interface for the COBalloonName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBALLOONNAME_H__24D73409_A142_4E80_8783_48688340245F__INCLUDED_)
#define AFX_OBALLOONNAME_H__24D73409_A142_4E80_8783_48688340245F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class cMultiLineText;

class COBalloonName  
{
public:

	COBalloonName();
	virtual ~COBalloonName();

	void InitName(char * name);
	void SetName(char* nickname);
	void SetNickName(char* nickname);
	void SetPetMasterName(char* petname);
	BOOL IsActive()						{ return m_bActive;	}
	BOOL IsShow()						{ return m_bShow;	}
	void SetActive(BOOL val)			{ m_bActive = val;	}
	void Show( BOOL val )				{ m_bShow	= val; 	}
	BOOL Render(LONG absX, LONG absY);
	void SetTall(LONG	Tall)			{ m_lTall = Tall;	}
	void SetPosX(LONG	pos);
	void SetNickPosX(LONG pos); 
	void SetPetMasterNameTall(LONG Tall)			{ m_lMasterTall = Tall;	}
	void SetPetMasterNamePosX(LONG pos)				{ m_lMasterPosX = pos;	}
	void SetNickNameTall(LONG	nickTall)			{ m_lNickTall = nickTall;	}
	void SetNickNamePosX(LONG	nickpos)			{ m_lNickPosX = nickpos;	}
	void SetFGColor(DWORD col)			{ m_fgColor = col;	}
	
	LONG GetPosX()						{ return m_lPosX; }
	LONG GetTall()						{ return m_lTall; }
	LONG GetNickPosX()					{ return m_lNickPosX; }

	// SiegeWar
	void SetSiegeName( DWORD NameType, DWORD dwLength );

	//2007. 10. 11. CBH - 오브젝트 폰트 바꺼주기위해 추가
	void SetFontIndex(BOOL bFontIndex);

protected:

	BOOL m_bActive;			//on/off option
	BOOL m_bShow;			//show/hide
	LONG m_lTall;
	LONG m_lPosX;
	LONG m_lNickTall;
	LONG m_lNickPosX;
	LONG m_lMasterTall;
	LONG m_lMasterPosX;
	char m_szObjectName[MAX_MONSTER_NAME_LENGTH +1];
	BOOL m_wFontIdx;
	DWORD m_fgColor;
	char m_szObjectNickName[MAX_GUILD_NICKNAME+1];
	char m_szPetMasterName[MAX_MASTERNAME_LENGTH+1];
	
	BOOL m_bSiegeMap;
	cMultiLineText* m_pSiegeName;
};

#endif // !defined(AFX_OBALLOONNAME_H__24D73409_A142_4E80_8783_48688340245F__INCLUDED_)
