#ifndef _CLASS_MR_MYCONTROL_
#define _CLASS_MR_MYCONTROL_

class CMRButton : public CButton
{
protected:
	DWORD		m_dwData;
	COLORREF text_colour, background_colour, disabled_background_colour;
	COLORREF light, highlight, shadow, dark_shadow;

public:
	CMRButton();
	virtual ~CMRButton();

	void Release();

	void SetColorText(COLORREF color);
	void ReSetColorText();

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void DrawFrame(CDC *dc, CRect r, int state);
	void DrawFilledRect(CDC *dc, CRect r, COLORREF colour);
	void DrawLine(CDC *dc, long sx, long sy, long ex, long ey, COLORREF colour);
	void DrawButtonText(CDC *dc, CRect r, const char *buf, COLORREF text_colour);

	void SetData(DWORD data) { m_dwData = data; }
	DWORD GetData() { return m_dwData; }
};


class CMREdit : public CEdit
{
protected:
	char				m_strData[256];	// stringDataÀÎ °æ¿ì
	DWORD				m_dwData;		// ValueDataÀÎ °æ¿ì

	BYTE				m_bType;		// DataÀÇ ÇüÅÂ±¸ºÐ 0:Text, 1:Value
	BOOL				m_bChange;		// EditÃ¢ÀÇ µ¥ÀÌÅ¸¸¦ ¼öÁ¤ÇÏ¿´´ÂÁö¸¦ Ã¼Å©ÇÑ´Ù.
	void*				m_pPointer;

	BOOL				m_bInSave;
	COLORREF			m_dwTextColor;
	
	DWORD				m_dwMaxValue;

protected:
	void CheckMaxValue(LPCSTR strValue, CString& strOut);

public:
	CMREdit();
	virtual ~CMREdit();
	
	void Release();

	COLORREF GetEditTextColor() { return m_dwTextColor; }
	void SetEditTextColor(COLORREF color);
	void ResetEditTextColor();

	void SetMaxValue(DWORD dwValue)  { m_dwMaxValue = dwValue; }

	void SetWindowTextEx(LPCSTR lpszString, BYTE DataType = 0);
	void CheckMaxValue(LPCSTR lpszString);

	// String Data
	void GetStringData(char* str) { strcpy( str, m_strData); }
	// Value Data
	DWORD GetData() { return m_dwData; }

	void ResetData();

	void SetChanged(BOOL bChange=TRUE)  { m_bChange = bChange; }
	void SetInSave(BOOL bSave=TRUE) { m_bInSave = bSave; }

	BOOL IsTextTypeData() { return (!m_bType)? TRUE:FALSE; }
	BYTE GetTextType() { return m_bType; }
	BOOL IsChanged() { return m_bChange; }

	BOOL HasSaveData() { return m_bInSave; }
};



#endif //_CLASS_MR_MYCONTROL_
