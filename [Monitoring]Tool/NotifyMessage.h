// NotifyMessage.h: interface for the CNotifyMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOTIFYMESSAGE_H__1EB931D9_505E_49F0_B75F_C50A905681DE__INCLUDED_)
#define AFX_NOTIFYMESSAGE_H__1EB931D9_505E_49F0_B75F_C50A905681DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNotifyMessage  
{
protected:
	char		m_sMessage[1000][256];
	int			m_nMaxLine;
	int			m_nCurLine;

	BOOL		m_bRepeat;
	int			m_nRepeatNum;

public:
	CNotifyMessage();
	virtual ~CNotifyMessage();

	void	SetMessage( int index, char* message )	{ strcpy( m_sMessage[index], message ); }
	char*	GetMessage( int index )					{ return m_sMessage[index]; }
	char*	GetMessage();							
	int		GetMaxLine()							{ return m_nMaxLine; }
	
	BOOL	Open( char* filename );
	void	SetRepeatNum( int repeatnum );
	int		GetRepeatNum()							{ return m_nRepeatNum; }
	BOOL	IsRepeat()								{ return m_bRepeat; }
	void	Release();

};

#endif // !defined(AFX_NOTIFYMESSAGE_H__1EB931D9_505E_49F0_B75F_C50A905681DE__INCLUDED_)
