#ifndef __FASTFILEIO_INCLUDE__
#define __FASTFILEIO_INCLUDE__

/////////////////////////////////////////////////////////////////////////////
// File - raw unbuffered disk file I/O

#define FILEIO_ERROR (0xffffffff)

class CFileIO
{
public:
// Error boolean
    BOOL bError;
// Flag values
    enum OpenFlags {
        modeRead =          0x0000,
        modeWrite =         0x0001,
        modeReadWrite =     0x0002,
        shareCompat =       0x0000,
        shareExclusive =    0x0010,
        shareDenyWrite =    0x0020,
        shareDenyRead =     0x0030,
        shareDenyNone =     0x0040,
        modeNoInherit =     0x0080,
        modeCreate =        0x1000,
        typeText =          0x4000, // typeText and typeBinary are used in
        typeBinary =   (int)0x8000 // derived classes only
        };

    enum Attribute {
        normal =    0x00,
        readOnly =  0x01,
        hidden =    0x02,
        system =    0x04,
        volume =    0x08,
        directory = 0x10,
        archive =   0x20
        };

    enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };

    enum { hFileNull = -1 };

// Constructors
    CFileIO();
    CFileIO(int hFile);
    CFileIO(LPCTSTR lpszFileName, UINT nOpenFlags);

// Attributes
    UINT m_hFile;

    virtual DWORD GetPosition() const;

// Operations
    virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags);

    static BOOL PASCAL Rename(LPCTSTR lpszOldName,
                LPCTSTR lpszNewName);
    static BOOL PASCAL Remove(LPCTSTR lpszFileName);

    DWORD SeekToEnd();
    void SeekToBegin();

    // backward compatible ReadHuge and WriteHuge
    DWORD ReadHuge(void* lpBuffer, DWORD dwCount);
    void WriteHuge(const void* lpBuffer, DWORD dwCount);

// Overridables
    virtual CFileIO* Duplicate() const;

    virtual LONG Seek(LONG lOff, UINT nFrom);
    virtual void SetLength(DWORD dwNewLen);
    virtual DWORD GetLength();

    virtual UINT Read(void* lpBuf, UINT nCount);
    virtual void Write(const void* lpBuf, UINT nCount);

    virtual void LockRange(DWORD dwPos, DWORD dwCount);
    virtual void UnlockRange(DWORD dwPos, DWORD dwCount);

    virtual void Abort();
    virtual void Flush();
    virtual void Close();

// Implementation
public:
    virtual ~CFileIO();
    enum BufferCommand { bufferRead, bufferWrite, bufferCommit, bufferCheck };
    virtual UINT GetBufferPtr(UINT nCommand, UINT nCount = 0,
        void** ppBufStart = NULL, void** ppBufMax = NULL);

protected:
    BOOL m_bCloseOnDelete;
};

#endif
