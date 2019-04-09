// GMToolManager.h: interface for the CGMToolManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMTOOLMANAGER_H__6A5E9778_EA08_42A5_82C0_F2FC37BD9EB5__INCLUDED_)
#define AFX_GMTOOLMANAGER_H__6A5E9778_EA08_42A5_82C0_F2FC37BD9EB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
#define GMTOOLMGR	CGMToolManager::GetInstance()


struct sFORBIDUSER
{
	DWORD				dwUserIdx;
	sFORBIDCONTENT*		pNextContent;

	sFORBIDUSER():pNextContent(0) {}
};

struct sFORBIDCONTENT
{
	BYTE				cbForbidKind;
	DWORD				dwDuringTime;
	BYTE				cbIsManual;
	sFORBIDCONTENT*		pNextContent;

	sFORBIDCONTENT():pNextContent(0) {}
};

class CGMToolManager  
{
protected:

	cPtrList					m_listForbidForTime;	//timeÀ¸·Î sorting
	CYHHashTable<sFORBIDUSER>	m_htForbidUser;


protected:

	void GetForbidListFromDB();	//

public:
	MAKESINGLETON( CGMToolManager );
	CGMToolManager();
	virtual ~CGMToolManager();
	void Init();
	void Release();

	void AddForbidList( DWORD dwUserIdx, sFORBIDCONTENT* pInfo );
	void DeleteForbidList( DWORD dwUserIdx, WORD wForbidKind );
	void ApplyForbidToUser( USERINFO* pUserInfo );

	void ProcessPermit();
};
*/ //À¸.. ÀÏ´Ü º¸·ù

#endif // !defined(AFX_GMTOOLMANAGER_H__6A5E9778_EA08_42A5_82C0_F2FC37BD9EB5__INCLUDED_)
