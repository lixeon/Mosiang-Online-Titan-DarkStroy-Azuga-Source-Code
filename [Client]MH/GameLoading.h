// GameLoading.h: interface for the CGameLoading class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMELOADING_H__1AFABF78_3E9E_4B20_B18B_E07438BFB3E9__INCLUDED_)
#define AFX_GAMELOADING_H__1AFABF78_3E9E_4B20_B18B_E07438BFB3E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"
#include "cImageSelf.h"


#define GAMELOADING USINGTON(CGameLoading)


class CLoadingDlg;

class CGameLoading : public CGameState  
{					
	HANDLE hThread;
	BOOL m_bFirst;
	BOOL m_bThreadTerminated;
	BOOL m_bMapLoadComplete;
	MAPTYPE m_MapNum;

	CLoadingDlg*	m_pDlg;
	cImageSelf		m_LoadingImage;

#ifndef _HK_LOCAL_
#ifndef _TL_LOCAL_
	///////////////////////////////////////////////////////////////////////
	// 06. 02 로딩화면 팁추가 - 이영준
	cImageSelf		m_TipImage;
	///////////////////////////////////////////////////////////////////////
#endif
#endif	//_HK_LOCAL_//

public:

	CGameLoading();
	virtual ~CGameLoading();
//	//MAKESINGLETON(CGameLoading)
		
	BOOL Init(void* pInitParam);
	void Release(CGameState* pNextGameState);

	void Process();
	void BeforeRender();
	void AfterRender();
	
	void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);



	// custom func
	void EnterGame();
};

EXTERNGLOBALTON(CGameLoading)

#endif // !defined(AFX_GAMELOADING_H__1AFABF78_3E9E_4B20_B18B_E07438BFB3E9__INCLUDED_)
