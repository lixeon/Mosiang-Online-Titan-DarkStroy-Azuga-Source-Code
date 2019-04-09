#ifndef __STREETSTALL__
#define __STREETSTALL__


#define MAX_STREETSTALL_CELLNUM 25
#define MAX_STREETBUYSTALL_CELLNUM 5

enum STALL_KIND
{
	eSK_NULL,
	eSK_SELL,
	eSK_BUY,
};

//#define DEFAULT_USABLE_INVENTORY	3

struct sCELLINFO 
{
	void Init()
	{
		wVolume = 0;
		dwMoney = 0;
		bLock = FALSE;
		bFill = FALSE;
		memset(&sItemBase, 0, sizeof(ITEMBASE));
	}

	ITEMBASE	sItemBase;
	DWORD		dwMoney;
	WORD		wVolume;
	BOOL		bLock;
	BOOL		bFill;
};

class CPlayer;

class cStreetStall 
{
protected:
	sCELLINFO	m_sArticles[MAX_STREETSTALL_CELLNUM];		// º¸À¯¸ñ·Ï
	CPlayer*	m_pOwner;									// »óÁ¡ ¿î¿µÀÚ
	cPtrList	m_GuestList;								// »óÁ¡ÀÇ ¼Õ´Ô
	int			m_nCurRegistItemNum;
	WORD		m_wStallKind;
//	WORD		m_nUsable;
	DWORD		m_nTotalMoney;

public:
	cStreetStall();
	virtual ~cStreetStall();
	void Init();

	DWORD	GetTotalMoney() { return m_nTotalMoney; };
//	WORD	GetUsable() { return m_nUsable; };
//	void	SetMaxUsable() { m_nUsable = MAX_STREETSTALL_CELLNUM; };
//	void	SetExtraUsable(WORD num) { m_nUsable += num; };
//	void	SetDefaultUsable() { m_nUsable = DEFAULT_USABLE_INVENTORY; };

	// »óÁ¡¿î¿µ Á¦¾î ÇÔ¼ö
	BOOL FillCell(ITEMBASE* pBase, DWORD money, BOOL bLock = FALSE, DWORD Volume = 0, WORD wAbsPosition = 0);
	void EmptyCell( ITEMBASE* pBase, eITEMTABLE tableIdx );
	void EmptyCell( POSTYPE pos );
	void EmptyCellAll();
	void ChangeCellState( WORD pos, BOOL bLock );

	// °ãÄ¡±â ¾ÆÀÌÅÛ¿¡ ´ëÇÑ Ã³¸®
	void UpdateCell( WORD pos, DURTYPE dur );

	void SetMoney( WORD pos, DWORD money );
	void SetVolume( WORD pos, WORD Volume );

	// LYJ ���Գ��� �߰�
	WORD GetStallKind() { return m_wStallKind; }
	void SetStallKind(WORD wStallKind) { m_wStallKind = wStallKind; }

	// »óÁ¡ ¿î¿µÀÚ¿¡ ´ëÇÑ ÇÔ¼ö 
	CPlayer* GetOwner() { return m_pOwner; }
	void SetOwner( CPlayer* pOwner ) { m_pOwner = pOwner; }
	
	// »óÁ¡¿¡ ´ëÇÑ ÀüÃ¼ Á¤º¸ ÇÔ¼ö
	void GetStreetStallInfo( STREETSTALL_INFO& stall );
	sCELLINFO* GetCellInfo( POSTYPE pos ) { return &m_sArticles[pos];}

	// »óÁ¡ ¼Õ´Ô¿¡ ´ëÇÑ Á¦¾î ÇÔ¼ö
	void AddGuest( CPlayer* pGuest );
	void DeleteGuest( CPlayer* pGuest );
	void DeleteGuestAll();
	void SendMsgGuestAll( MSGBASE* pMsg, int nMsgLen, BOOL bChangeState = FALSE );

	int GetCurRegistItemNum() { return m_nCurRegistItemNum; }
	
	BOOL IsFull();

	BOOL CheckItemDBIdx(DWORD idx);
	BOOL CheckItemIdx(DWORD idx);// Áßº¹ °Ë»ç (ÇÊÈ÷ DB¿¡ ÀÖ´Â ÀÎµ¦½º¿¡ ´ëÇÑ ºñ±³¸¦ ÇÑ´Ù.)
};

#endif //__STREETSTALL__