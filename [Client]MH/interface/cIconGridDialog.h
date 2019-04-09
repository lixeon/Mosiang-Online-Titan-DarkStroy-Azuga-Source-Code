// cIconGridDialog.h: interface for the cIconGridDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CICONGRIDDIALOG_H_
#define _CICONGRIDDIALOG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cDialog.h"

class cIcon;

class cIconGridCell
{
public:
	cIcon * icon;
	BOOL use;
};

class cIconGridDialog : public cDialog  
{
	enum {NOTUSE=0,USE=1};
public:
	cIconGridDialog();
	virtual ~cIconGridDialog();
	
	virtual void InitDialog(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, WORD col, WORD row, LONG ID=0);
	virtual void Render();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void SetAbsXY(LONG x, LONG y);
	BOOL PtInCell(LONG x, LONG y);
	virtual void SetActive(BOOL val);
	virtual void SetAlpha(BYTE al);
	void SetIconCellBGImage(cImage * image) { m_SelectedBGImage = *image; }
	void SetDragOverBGImage(cImage * image) { m_DragOverBGImage = *image; }	

	cIcon * GetIconForIdx(WORD idx){ ASSERT( idx < m_nRow*m_nCol ); return m_pIconGridCell[idx].icon; }
	inline void SetAcceptableIconType(DWORD type){ m_acceptableIconType=type; }
	BOOL GetCellPosition(LONG mouseX, LONG mouseY, WORD& cellX,WORD& cellY);
	BOOL GetPositionForXYRef(LONG mouseX, LONG mouseY, WORD& pos);
	WORD GetPositionForCell(WORD cellX, WORD cellY);
	BOOL GetCellAbsPos(WORD pos, int& absX, int& absY);
	BOOL IsAddable(WORD idx);
	virtual BOOL IsAddable(WORD cellX,WORD cellY,cIcon* pIcon);

	BOOL AddIcon(WORD pos, cIcon * icon);
	BOOL AddIcon(WORD cellX, WORD cellY, cIcon * icon);
	virtual BOOL MoveIcon(WORD cellX, WORD cellY, cIcon * icon);
	BOOL DeleteIcon(WORD pos, cIcon ** icon);
	BOOL DeleteIcon(cIcon * icon);
	BOOL DeleteIcon(WORD cellX, WORD cellY, cIcon ** iconOut);
	WORD GetCellNum() { return m_nRow*m_nCol; }

	WORD m_IconDlgID;							//???? 안쓰이는 건감???ㅡㅡ;
	DWORD m_acceptableIconType;					// bit operation

	cIconGridCell * m_pIconGridCell;
	WORD m_nRow;
	WORD m_nCol;

	void SetCellRect(RECT * rect);
	LONG GetCurSelCellPos(){ return m_lCurSelCellPos; }
	void SetCurSelCellPos(LONG pos){ m_lCurSelCellPos = pos; }
	void InitGrid(LONG grid_x, LONG grid_y, WORD cellWid, WORD cellHei, WORD borderX, WORD borderY );

	// LBS 03.09.22
	void SetShowGrid( BOOL val ) { m_bShowGrid = val; }
	
	virtual void SetDisable( BOOL val );

	virtual BOOL IsDragOverDraw();
	void SetDragOverIconType( int nIconType ) { m_nIconType = nIconType; }
//	virtual void SetTopWindow();
	
#ifdef _JAPAN_LOCAL_
	void SetDisableIcon( DWORD dwFrom, DWORD dwTo )
		{	m_DisableFromPos = dwFrom;		m_DisableToPos = dwTo;		}
#endif	// _JAPAN_LOCAL_

#ifdef _HK_LOCAL_
	void SetDisableIcon( DWORD dwFrom, DWORD dwTo )
		{	m_DisableFromPos = dwFrom;		m_DisableToPos = dwTo;		}
#endif

#ifdef _TL_LOCAL_
	void SetDisableIcon( DWORD dwFrom, DWORD dwTo )
		{	m_DisableFromPos = dwFrom;		m_DisableToPos = dwTo;		}
#endif

protected:
	VECTOR2 m_gridPos;
	RECT m_cellRect;						// it;s can;t setting on outside in the future
	WORD m_wCellBorderX;
	WORD m_wCellBorderY;
	WORD m_wCellWidth;
	WORD m_wCellHeight;
	LONG m_lCurSelCellPos;
	LONG m_lCurDragOverPos;
	cImage m_SelectedBGImage;
	cImage m_DragOverBGImage;
	cImage m_GridLockImage;
	int	m_nIconType;
	BOOL m_bItemDraged;
	
	// LBS 03.09.22 추가 
	BOOL m_bShowGrid;

#ifdef _JAPAN_LOCAL_	
	DWORD		m_DisableFromPos;
	DWORD		m_DisableToPos;
#endif	// _JAPAN_LOCAL_

#ifdef _HK_LOCAL_	
	DWORD		m_DisableFromPos;
	DWORD		m_DisableToPos;
#endif	// _HK_LOCAL_

#ifdef _TL_LOCAL_	
	DWORD		m_DisableFromPos;
	DWORD		m_DisableToPos;
#endif
};

#endif // _CICONGRIDDIALOG_H_
