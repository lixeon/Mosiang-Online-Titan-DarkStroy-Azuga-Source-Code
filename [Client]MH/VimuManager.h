#ifndef _VIMUMANAGER_H
#define _VIMUMANAGER_H


#define VIMUMGR USINGTON(CVimuManager)

class CPlayer;

class CVimuManager 
{

protected:

	BOOL m_bIsVimuing;

public:

	//MAKESINGLETON(CVimuManager);

	CVimuManager();
	virtual ~CVimuManager();

	void Init();
	void ApplyVimu();		
	void CancelApply();
	void AcceptVimu( BOOL bAccept );
	BOOL CanAcceptVimu( CPlayer* pAccepter );

	void NetworkMsgParse(BYTE Protocol,void* pMsg);
	
	void SetVimuing( BOOL bVimuing );
	BOOL IsVimuing() { return m_bIsVimuing; }

};

EXTERNGLOBALTON(CVimuManager);
#endif