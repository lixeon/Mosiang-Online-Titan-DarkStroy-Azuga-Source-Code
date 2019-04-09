// MugongDialog_BACKUP.h: interface for the CMugongDialog_BACKUP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUGONGDIALOG_BACKUP_H__FE563B1A_26E4_4C68_B3BD_235D19ECB2AD__INCLUDED_)
#define AFX_MUGONGDIALOG_BACKUP_H__FE563B1A_26E4_4C68_B3BD_235D19ECB2AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
enum MUGONG_TYPE
{	
	MUGONG=0, 
	NAEGONG=1, 
	KYUNGGONG=2,
	ALL=3,
	ATTACK_GUM,
	ATTACK_GWUN,
	ATTACK_DO,
	ATTACK_CHANG,
	ATTACK_GUNG,
	ATTACK_AMGI,

	DEFENSE_GUM,
	DEFENSE_GWUN,
	DEFENSE_DO,
	DEFENSE_CHANG,
	DEFENSE_GUNG,
	DEFENSE_AMGI,

	ASSIST_GUM,
	ASSIST_GWUN,
	ASSIST_DO,
	ASSIST_CHANG,
	ASSIST_GUNG,
	ASSIST_AMGI,


	MUGONGERR,
};

enum MLOC{ WINDOW_STD=0, WINDOW_EXTEND=1/*ÆîÄ£¹«°ø*/, WINDOW_DOWN=2/*³»°ø°æ°ø*/,WINDOW_ERR=4};
enum { MGI_NAME, MGI_ATTR, MGI_TYPE, MGI_KIND, MGI_SUNG, MGI_EXPPOINT };
*/
class CItem;
class CMugongBase;

class CMugongDialog  : public cIconDialog
{
	cStatic * pWearedMugongInfo[3][6];			// ÀÏ¹Ý,³»°ø,°æ°ø
	
	friend class CMugongManager;
public:
	CMugongDialog();
	virtual ~CMugongDialog();
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Render();
	virtual void SetAbsXY(LONG x, LONG y);
	virtual void SetActive(BOOL val);
	
	void Linking();
	virtual void Add(cWindow * window);
	void AddMugong(MUGONGBASE * mugongBase);
	void AddMugong(POSTYPE pos, CMugongBase * mugong);
	CMugongBase * GetMugongAbs(POSTYPE abs_Pos);
	void GetMugong(POSTYPE RelPos, CMugongBase * mugongOut);
	void FakeDeleteMugong(POSTYPE absPos);
	BOOL DeleteMugong(POSTYPE pos, CMugongBase ** mugong);
	
	virtual BOOL FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon);
	void MoveMugong(POSTYPE ToPos, POSTYPE FromPos);
	void UpdateData(MUGONG_TYPE type=ALL);
	BOOL GetBlankPositionRef(BYTE type, WORD & absPos);
	BOOL GetPositionForXYRef(LONG x, LONG y, POSTYPE& pos);
	MLOC ConvAbs2Rel(POSTYPE abs_pos);
	BOOL IsAddMugong(POSTYPE pos);

	// µ¥ÀÌÅÍ ¼ÂÆÃ°ú ÀÎÅ×ÆäÀÌ½º Ãâ·Â
	void SetMugongSung(BYTE mugongType, WORD sung);
	void SetMugongExpPoint(BYTE mugongType, DWORD expPoint);
	void FakeAddItem(LONG mouseX, LONG mouseY, CItem * icon);
	
private:
	BOOL CanBeMoved(cIcon * pIcon, POSTYPE abs_pos);
	void FakeMoveMugong(LONG mouseX, LONG mouseY, CMugongBase * icon);
	cIconDialog * m_pMugongDlg1_1;			// ³»°ø, °æ°ø
	cIconGridDialog * m_pMugongDlg1_2;		// ÆîÄ£ ¹«°ø
	cPushupButton * m_pMugongSpreadBtn;
};
#endif // !defined(AFX_MUGONGDIALOG_BACKUP_H__FE563B1A_26E4_4C68_B3BD_235D19ECB2AD__INCLUDED_)
