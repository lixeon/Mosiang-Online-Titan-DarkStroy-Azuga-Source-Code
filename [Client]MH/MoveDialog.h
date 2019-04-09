// MoveDialog.h: interface for the CMoveDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVEDIALOG_H__AEF92BC4_C38B_49D4_9157_C75C38355D0A__INCLUDED_)
#define AFX_MOVEDIALOG_H__AEF92BC4_C38B_49D4_9157_C75C38355D0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"


class cListCtrl;
class cPushupButton;



class CMoveDialog : public cDialog
{
	cListCtrl*		m_pMoveLCtrl;
	cPtrList		m_pDataList;
	cPtrList		m_pTownList;
	cButton*		m_pNameBtn;
	cButton*		m_pMoveBtn;
	cButton*		m_pDelBtn;
	cPushupButton*	m_pPageBtn[MAX_MOVEPOINT_PAGE];

	BOOL		m_bTownMove;
	int			m_SelectRowIdx;

	BOOL		m_bMovePos;
	DWORD		m_ItemIdx;
	DWORD		m_ItemPos;
	
	BOOL		m_bExtend;
	DWORD		m_SelectedTab;
	

public:
	CMoveDialog();
	virtual ~CMoveDialog();

	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void LoadTownMovePoint();

	void Linking();
	void SetActive( BOOL val );
	void SetTownMoveView( BOOL bTown );
	BOOL IsTownMove()					{	return m_bTownMove;	}

	void RefreshMoveInfo();
	void ResetMoveIdx();
	void SetMoveInfo(SEND_MOVEDATA_INFO* pData);
	void AddMoveInfo(MOVEDATA* pData, BOOL bTown=FALSE);
	void DelMoveInfoSync();
	void UpdateMoveInfo(MOVEDATA* pData);	
	void DelMoveInfo(MOVEDATA* pData);
	void SelectMoveIdx(int rowidx);

	void MapMoveOK();
	
	DWORD GetSavedPoint();
	DWORD GetSavedCount()			{	return m_pDataList.GetCount();	}
	DWORD GetSelectedDBIdx();

	BOOL IsMovePos()				{	return m_bMovePos;	}
	void SetItemToMapServer( DWORD ItemIdx, DWORD ItemPos, BOOL bMovePos )		{	m_ItemIdx=ItemIdx;	m_ItemPos=ItemPos;	m_bMovePos=bMovePos;	}
	BOOL CheckSameName( char* pName );
	void SetExtend( BOOL bExtend );
	void SetButton( DWORD Idx );

#ifdef _KOR_LOCAL_
	void MapChange(WORD wMoveMapNum);	//2008. 5. 9. CBH - 아이탬 사용하여 맵 채널 이동
#endif
};

#endif // !defined(AFX_MOVEDIALOG_H__AEF92BC4_C38B_49D4_9157_C75C38355D0A__INCLUDED_)
