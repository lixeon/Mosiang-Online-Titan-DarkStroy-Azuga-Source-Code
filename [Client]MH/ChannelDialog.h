// ChannelDialog.h: interface for the CChannelDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANNELDIALOG_H__2799AAF3_2A8B_402D_86D4_EB2547D1888B__INCLUDED_)
#define AFX_CHANNELDIALOG_H__2799AAF3_2A8B_402D_86D4_EB2547D1888B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include ".\interface\cWindowHeader.h"


class cListCtrl;
class CChannelDialog  : public cDialog
{
	int m_BaseChannelIndex;
	cListCtrl * m_pChannelLCtrl;
	BOOL m_bInit;
	int m_SelectRowIdx;
#ifdef _KOR_LOCAL_
	WORD m_wMoveMapNum;	//2008. 5. 9. CBH - 이동할 맵의 번호를 저장
	DWORD m_dwChangeMapState;	//2008. 5. 9. CBH - 이동할 맵의 매개체 종류
#endif

public:
	CChannelDialog();
	virtual ~CChannelDialog();

	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void Linking();
	void SetChannelList(MSG_CHANNEL_INFO* pInfo);
	virtual void SetActive(BOOL val);

	void SelectChannel(int rowidx);
	void OnConnect();

#ifdef _KOR_LOCAL_
	//2008. 5. 8. CBH - 맵이동 채널 선택 관련 함수 추가
	void SendMapChannelInfoSYN(WORD wMapNum, DWORD dwState = eMapChange_General);	//이동할 맵의 채널 정보를 요청하는 함수
	void MapChange();
#endif
};

#endif // !defined(AFX_CHANNELDIALOG_H__2799AAF3_2A8B_402D_86D4_EB2547D1888B__INCLUDED_)
