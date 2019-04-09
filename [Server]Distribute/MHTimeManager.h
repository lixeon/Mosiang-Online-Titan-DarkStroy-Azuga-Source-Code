// MHTimeManager.h: interface for the CMHTimeManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHTIMEMANAGER_H__890F483A_4FB0_4A20_91BF_BD8BD48941A0__INCLUDED_)
#define AFX_MHTIMEMANAGER_H__890F483A_4FB0_4A20_91BF_BD8BD48941A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MHTIMEMGR_OBJ CMHTimeManager::GetInstance()

class CMHTimeManager  
{
	DWORD m_MHDate;
	DWORD m_MHTime;
	
	DWORD m_lasttime;

public:
	GETINSTANCE(CMHTimeManager);
	
	CMHTimeManager();
	virtual ~CMHTimeManager();

	void Init(DWORD mhDate,DWORD mhTime);
	void Process();

	DWORD GetMHDate();
	DWORD GetMHTime();

	void GetMHDate(BYTE& year,BYTE& month,BYTE& day);
	void GetMHTime(BYTE& hour,BYTE& minute);

	DWORD GetNewCalcCurTime();
};

#endif // !defined(AFX_MHTIMEMANAGER_H__890F483A_4FB0_4A20_91BF_BD8BD48941A0__INCLUDED_)
