#ifndef __STRING_CORE_INCLUDE__
#define __STRING_CORE_INCLUDE__

class CStrClass
{
public:
// Constructors
	CStrClass();
	CStrClass(const CStrClass& stringSrc);
	CStrClass(TCHAR ch, int nRepeat = 1);
	CStrClass(LPCSTR lpsz);
	CStrClass(LPCWSTR lpsz);
	CStrClass(LPCTSTR lpch, int nLength);
	CStrClass(const unsigned char* psz);

// Attributes & Operations
	// as an array of characters
	int GetLength() const;
	BOOL IsEmpty() const;
	void Empty();						// free up the data

	TCHAR GetAt(int nIndex) const;		// 0 based
	TCHAR operator[](int nIndex) const; // same as GetAt
	void SetAt(int nIndex, TCHAR ch);
	operator LPCTSTR() const;			// as a C string

	// overloaded assignment
	const CStrClass& operator=(const CStrClass& stringSrc);
	const CStrClass& operator=(TCHAR ch);
#ifdef _UNICODE
	const CStrClass& operator=(char ch);
#endif
	const CStrClass& operator=(LPCSTR lpsz);
	const CStrClass& operator=(LPCWSTR lpsz);
	const CStrClass& operator=(const unsigned char* psz);

	// string concatenation
	const CStrClass& operator+=(const CStrClass& string);
	const CStrClass& operator+=(TCHAR ch);
#ifdef _UNICODE
	const CStrClass& operator+=(char ch);
#endif
	const CStrClass& operator+=(LPCTSTR lpsz);

	friend CStrClass __cdecl operator+(const CStrClass& string1,const CStrClass& string2);
	friend CStrClass __cdecl operator+(const CStrClass& string, TCHAR ch);
	friend CStrClass __cdecl operator+(TCHAR ch, const CStrClass& string);
#ifdef _UNICODE
	friend CStrClass __cdecl operator+(const CStrClass& string, char ch);
	friend CStrClass __cdecl operator+(char ch, const CStrClass& string);
#endif
	friend CStrClass __cdecl operator+(const CStrClass& string, LPCTSTR lpsz);
	friend CStrClass __cdecl operator+(LPCTSTR lpsz, const CStrClass& string);

	// string comparison
	int Compare(LPCTSTR lpsz) const;		 // straight character
	int CompareNoCase(LPCTSTR lpsz) const;	 // ignore case
	int Collate(LPCTSTR lpsz) const;		 // NLS aware

	// simple sub-string extraction
	CStrClass Mid(int nFirst, int nCount) const;
	CStrClass Mid(int nFirst) const;
	CStrClass Left(int nCount) const;
	CStrClass Right(int nCount) const;

	CStrClass SpanIncluding(LPCTSTR lpszCharSet) const;
	CStrClass SpanExcluding(LPCTSTR lpszCharSet) const;

	// upper/lower/reverse conversion
	void MakeUpper();
	void MakeLower();
	void MakeReverse();

	// searching (return starting index, or -1 if not found)
	// look for a single character match
	int Find(TCHAR ch) const;				// like "C" strchr
	int ReverseFind(TCHAR ch) const;
	int FindOneOf(LPCTSTR lpszCharSet) const;

	// look for a specific sub-string
	int Find(LPCTSTR lpszSub) const;		// like "C" strstr

	// simple formatting
	void __cdecl Format(LPCTSTR lpszFormat, ...);

	// Windows support
	BOOL LoadString(UINT nID);			// load from string resource
										// 255 chars max
#ifndef _UNICODE
	// ANSI <-> OEM support (convert string in place)
	void AnsiToOem();
	void OemToAnsi();
#endif

#ifndef _AFX_NO_BSTR_SUPPORT
	// OLE 2.0 BSTR support (use for OLE automation)
	BSTR AllocSysString();
	BSTR SetSysString(BSTR* pbstr);
#endif

	// Access to string implementation buffer as "C" character array
	LPTSTR GetBuffer(int nMinBufLength);
	void ReleaseBuffer(int nNewLength = -1);
	LPTSTR GetBufferSetLength(int nNewLength);
	void FreeExtra();

// Implementation
public:
	~CStrClass();
	int GetAllocLength() const;

protected:
	// lengths/sizes in characters
	//	(note: an extra character is always allocated)
	LPTSTR m_pchData;			// actual string (zero terminated)
	int m_nDataLength;			// does not include terminating 0
	int m_nAllocLength; 		// does not include terminating 0

	// implementation helpers
	void Init();
	void AllocCopy(CStrClass& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
	void AllocBuffer(int nLen);
	void AssignCopy(int nSrcLen, LPCTSTR lpszSrcData);
	void ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, int nSrc2Len, LPCTSTR lpszSrc2Data);
	void ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData);
	static void SafeDelete(LPTSTR lpch);
	static int SafeStrlen(LPCTSTR lpsz);

};

// Compare helpers
BOOL __cdecl operator==(const CStrClass& s1, const CStrClass& s2);
BOOL __cdecl operator==(const CStrClass& s1, LPCTSTR s2);
BOOL __cdecl operator==(LPCTSTR s1, const CStrClass& s2);
BOOL __cdecl operator!=(const CStrClass& s1, const CStrClass& s2);
BOOL __cdecl operator!=(const CStrClass& s1, LPCTSTR s2);
BOOL __cdecl operator!=(LPCTSTR s1, const CStrClass& s2);
BOOL __cdecl operator<(const CStrClass& s1, const CStrClass& s2);
BOOL __cdecl operator<(const CStrClass& s1, LPCTSTR s2);
BOOL __cdecl operator<(LPCTSTR s1, const CStrClass& s2);
BOOL __cdecl operator>(const CStrClass& s1, const CStrClass& s2);
BOOL __cdecl operator>(const CStrClass& s1, LPCTSTR s2);
BOOL __cdecl operator>(LPCTSTR s1, const CStrClass& s2);
BOOL __cdecl operator<=(const CStrClass& s1, const CStrClass& s2);
BOOL __cdecl operator<=(const CStrClass& s1, LPCTSTR s2);
BOOL __cdecl operator<=(LPCTSTR s1, const CStrClass& s2);
BOOL __cdecl operator>=(const CStrClass& s1, const CStrClass& s2);
BOOL __cdecl operator>=(const CStrClass& s1, LPCTSTR s2);
BOOL __cdecl operator>=(LPCTSTR s1, const CStrClass& s2);

// conversion helpers
int _wcstombsz_(char* mbstr, const wchar_t* wcstr, size_t count);
int _mbstowcsz_(wchar_t* wcstr, const char* mbstr, size_t count);

// Globals
extern TCHAR jazzidChNil;
extern const CStrClass jazzidEmptyString;


#endif
