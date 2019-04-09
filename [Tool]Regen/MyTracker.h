
class CMyTracker{
protected:
	CWnd *m_pWnd;//ºÎ¸ð À©µµ¿ì ÇÚµé·¯
	BOOL m_bDown;//¸¶¿ì½º ¹öÆ° ÇÃ·¯±× 
	CRect m_rectOld;//ÀÌÀü ¿µ¿ª ÁÂÇ¥ ÀúÀå º¯¼ö
	//¸¶¿ì½º°¡ ÃßÀû¹Ú½º¿¡¼­ 8°³¹æÇâ°ú ÀÌµ¿ À§Ä¡ÀÇ ¾î´ÀÁ¡¿¡ ÀÖ´Â°¡ÀÇ 
	//°ªÀ» ÀúÀåÇÏ°í ÀÖ´Â º¯¼ö
	int m_nChoNum;
	//ÇöÀç ÇÁ·Î¼¼½º¸¦ ¼³Á¤ÇÏ°í ÀÖ´Â º¯¼ö
	int m_nProcess;
	//8°³ ¹æÇâÀÇ ÁÂÇ¥¿Í 4°³¹æÇâÀÇ ÀÌµ¿ÁÂÇ¥ ÀúÀå º¯¼ö
	CRect m_rectDirect[12];
	//ÀÌÀü ¸¶¿ì½º Æ÷ÀÎÅÍ º¯¼ö
	CPoint m_OldPoint;
	//m_rect¿µ¿ªÀÌ ¼³Á¤µÇ¸é ÀÌ¿µ¿ªÀ» Åä´ë·Î m_rectDirectÀÇ ¿µ¿ªÀ» ¸¸µé¾îÁÖ´Â 
	//ÇÔ¼ö
	void MakeDirect();
	//8°³ ¿µ¿ª ¹Ú½º Á¡ÀÌ ¼±º¸´Ù ¾ó¸¶³ª µÎ²¨¿î°¡¸¦ ÀúÀåÇÏ´Â º¯¼ö
	int m_nLineDot;
	//ÃßÀû¹Ú½º ¶óÀÎÀÇ µÎ²² 
	int m_nLindTick;
	//m_rectDirect¿µ¿ªÀÌ ¼³Á¤µÇ¾î ÀÖÀ¸¸é TRUE±×·¸Áö ¾ÊÀ¸¸é FALSE
	int m_bSet;
public:
	//ÀÌµ¿µÇ´Â ¿µ¿ª ÁÂÇ¥ ÀúÀå º¯¼ö
	CRect m_rectTrack;
	//ÇöÀç ÃßÀû¹Ú½ºÀÇ ÁÂÇ¥
	CRect m_rect;
	CMyTracker();
	CMyTracker(CWnd *pWnd,int dot=2,int tick=3);
	void SetRect(CRect rect);
	void Draw(CDC *pDC);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void SetProcess(int num);
	int  GetProcess();

	int PtInRect(CPoint point);
	void SetMouseCursor();
	int GetChoNum(){return m_nChoNum;}

	
};