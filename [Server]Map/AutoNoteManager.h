#pragma once


#define AUTONOTEMGR	USINGTON(CAutoNoteManager)

enum eBLIT_IMAGE_FLAG
{
	BIF_NONE		= 0,
	BIF_RANDOMCOLOR	= 1,
};

class CAutoNoteRoom;

class CAutoNoteManager
{
protected:
	CYHHashTable< CAutoNoteRoom >			m_htAutoNoteRoom;
	CMemoryPoolTempl< CAutoNoteRoom >*		m_pmpAutoNoteRoom;

	BOOL	m_bInited;				// 기능 제어 변수로도 사용한다...

	BYTE*	m_pOriRaster[4];
	BYTE*	m_pBGRaster;
	BYTE*	m_pNoiseRaster;

	BYTE*	m_pSendRaster;

	DWORD	m_dwReplyTime;
	int		m_nReplyChance;

public:
	CAutoNoteManager(void);
	~CAutoNoteManager(void);

	void Init();
	void Release();
	void Process();
	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );

	BOOL CanUseAutoNote()				{ return m_bInited; }
	void SetUseAutoNoet( BOOL bUse )	{ m_bInited = bUse; }

	BYTE* GetSendRaster()	{ return m_pSendRaster; }

	void AutoPlayerLogOut( CPlayer* pAutoPlayer );
	void MakeSendRaster( DWORD* pData );
	BOOL BMP_RasterLoad( char* filename, BYTE* pRaster );
	void BlitImage( BYTE* pDest, BYTE* pSrc, int dw, int dh, int sw, int sh, int x, int y, float fszw = 1.0f, float fszh = 1.0f, float fRot = 0.0f, int cx = 0, int cy = 0, int Flag = BIF_NONE );

	DWORD GetReplyTime()				{ return m_dwReplyTime; }
	void SetReplyTime( DWORD dwTime )	{ m_dwReplyTime = dwTime; }
	DWORD GetReplyChance()				{ return m_nReplyChance; }
	void SetReplyChance( int nChance )	{ m_nReplyChance = nChance; }
};

EXTERNGLOBALTON(CAutoNoteManager)
