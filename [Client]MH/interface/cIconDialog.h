//////////////////////////////////////////////////////////////////////
// class name : cIconDialog 
// Lastest update : 2002.10.29. by taiyo
//////////////////////////////////////////////////////////////////////

#ifndef _cICONDIALOG_H_
#define _cICONDIALOG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cDialog.h"

class cIcon;

class cIconCell
{
public:
	cIconCell():icon(NULL), use(0){}
	~cIconCell(){};
	cIcon * icon;
	RECT rel_rect;
	BOOL use;
	BOOL bOnlyLink;
};

class cIconDialog : public cDialog  
{
public:
	enum {NOTUSE=0,USE=1};
	cIconDialog();
	virtual ~cIconDialog();


	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	void Release();
	virtual void Render();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void SetAbsXY(LONG x, LONG y);
	BOOL PtInCell(LONG x, LONG y);
	void AddIconCell(RECT * pRect);
	virtual void SetActive(BOOL val);
	virtual void SetAlpha(BYTE al);
	void SetIconCellBGImage(cImage * image) { m_SelectedBGImage = *image; }
	void SetDragOverBGImage(cImage * image) { m_DragOverBGImage = *image; }

	BOOL IsAddable(WORD idx);
	BOOL IsAcceptable(DWORD type){ return (type & m_acceptableIconType); }
	DWORD GetAcceptableIconType(){ return m_acceptableIconType; }
	void SetAcceptableIconType(DWORD type){ m_acceptableIconType=type; }
	BOOL GetPositionForXYRef(LONG mouseX, LONG mouseY, WORD& pos);
	virtual BOOL AddIcon(LONG mouseX, LONG mouseY, cIcon * icon);
	BOOL AddIcon(WORD cellIdx, cIcon * icon,BOOL bOnlyLink = FALSE);
	BOOL DeleteIcon(WORD cellIdx, cIcon ** icon);
	void DeleteIconAll();
	void SetCellNum(WORD num);
	WORD GetCellNum() { return m_wCellNum; }
	LONG GetCurSelCellPos(){ return m_lCurSelCellPos; }
	void SetCurSelCellPos(LONG pos){ m_lCurSelCellPos = pos; }
	cIcon * GetIconForIdx(WORD idx){ ASSERT(idx < m_wCellNum); return m_pIconCell[idx].icon; }
	
	virtual void SetDisable( BOOL val );
	virtual BOOL IsDragOverDraw();
	void SetDragOverIconType( int nIconType ) { m_nIconType = nIconType; }

	void NotifyLinkIconDeleted(cIcon* pIcon);
protected:
	DWORD		m_acceptableIconType;					// bit operation
	cIconCell*	m_pIconCell;
	WORD		m_wCellNum;
	int			idx;
	LONG		m_lCurSelCellPos;
	LONG		m_lCurDragOverPos;
	int			m_nIconType;
	BOOL		m_bItemDraged;
	cImage		m_SelectedBGImage;
	cImage		m_DragOverBGImage;
};

#endif // _cICONDIALOG_H_
