#ifndef _VIMUMANAGER_H
#define _VIMUMANAGER_H


#define VIMUMGR CVimuManager::GetInstance()


class CVimuManager
{
protected:

public:

	MAKESINGLETON( CVimuManager );	

	CVimuManager();
	virtual ~CVimuManager();

	void UserLogOut( CPlayer* pPlayer );
	BOOL CanApplyVimu( CPlayer* pApplyer, CPlayer* pAccepter );
	BOOL CanAcceptVimu( CPlayer* pApplyer, CPlayer* pAccepter );

	void NetworkMsgParse(BYTE Protocol,void* pMsg);
	
};


#endif