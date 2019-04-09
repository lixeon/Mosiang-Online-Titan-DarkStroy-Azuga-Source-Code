#include "stdafx.h"
#include "fileio.h"

CFileIO::CFileIO()
{
    bError = FALSE;
    m_hFile = hFileNull;
    m_bCloseOnDelete = FALSE;
}

CFileIO::CFileIO(int hFile)
{
    bError = FALSE;
    m_hFile = hFile;
    m_bCloseOnDelete = FALSE;
}

CFileIO::CFileIO(LPCTSTR lpszFileName, UINT nOpenFlags)
{
    bError = FALSE;
    if (!Open(lpszFileName, nOpenFlags))
        bError = TRUE;
}

CFileIO::~CFileIO()
{
    if (m_hFile != (UINT)hFileNull && m_bCloseOnDelete)
        Close();
}

CFileIO* CFileIO::Duplicate() const
{
    CFileIO* pFile = new CFileIO(hFileNull);
    HANDLE hFile;
    if (!::DuplicateHandle(::GetCurrentProcess(), (HANDLE)m_hFile,
        ::GetCurrentProcess(), &hFile, 0, FALSE, DUPLICATE_SAME_ACCESS))
    {
        delete pFile;
        return NULL;
    }
    pFile->m_hFile = (UINT)hFile;
    pFile->m_bCloseOnDelete = m_bCloseOnDelete;
    return pFile;
}

BOOL CFileIO::Open(LPCTSTR lpszFileName, UINT nOpenFlags)
{
    // CFileIO objects are always binary and CreateFile does not need flag
    nOpenFlags &= ~(UINT)typeBinary;

    m_bCloseOnDelete = FALSE;
    m_hFile = (UINT)hFileNull;

    // map read/write mode
    DWORD dwAccess;
    switch (nOpenFlags & 3)
    {
    case modeRead:
        dwAccess = GENERIC_READ;
        break;
    case modeWrite:
        dwAccess = GENERIC_WRITE;
        break;
    case modeReadWrite:
        dwAccess = GENERIC_READ|GENERIC_WRITE;
        break;
    }

    // map share mode
    DWORD dwShareMode;
    switch (nOpenFlags & 0x70)
    {
        case shareCompat:       // map compatibility mode to exclusive
        case shareExclusive:
            dwShareMode = 0;
            break;
        case shareDenyWrite:
            dwShareMode = FILE_SHARE_READ;
            break;
        case shareDenyRead:
            dwShareMode = FILE_SHARE_WRITE;
            break;
        case shareDenyNone:
            dwShareMode = FILE_SHARE_WRITE|FILE_SHARE_READ;
            break;
    }

    // Note: typeText and typeBinary are used in derived classes only.

    // map modeNoInherit flag
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = (nOpenFlags & modeNoInherit) == 0;

    // map creation flags
    DWORD dwCreateFlag;
    if (nOpenFlags & modeCreate)
        dwCreateFlag = CREATE_ALWAYS;
    else
        dwCreateFlag = OPEN_EXISTING;

    // attempt file creation
    HANDLE hFile = ::CreateFile(lpszFileName, dwAccess, dwShareMode, &sa,
        dwCreateFlag, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        bError = TRUE;
        return FALSE;
    }
    m_hFile = (HFILE)hFile;
    m_bCloseOnDelete = TRUE;
    return TRUE;
}

UINT CFileIO::Read(void* lpBuf, UINT nCount)
{
    if (nCount == 0)
        return 0;   // avoid Win32 "null-read"

    DWORD dwRead;
    if (!::ReadFile((HANDLE)m_hFile, lpBuf, nCount, &dwRead, NULL))
    {
        bError = TRUE;
        return FILEIO_ERROR;
    }

    return (UINT)dwRead;
}

void CFileIO::Write(const void* lpBuf, UINT nCount)
{
    if (nCount == 0)
        return;     // avoid Win32 "null-write" option

    DWORD nWritten;
    if (!::WriteFile((HANDLE)m_hFile, lpBuf, nCount, &nWritten, NULL))
    {
        bError = TRUE;
        return;
    }

    // Win32s will not return an error all the time (usually DISK_FULL)
    if (nWritten != nCount)
    {
        bError = TRUE;
    }
}

LONG CFileIO::Seek(LONG lOff, UINT nFrom)
{
    DWORD dwNew = ::SetFilePointer((HANDLE)m_hFile, lOff, NULL, (DWORD)nFrom);
    if (dwNew  == (DWORD)-1)
    {
        bError = TRUE;
        return FILEIO_ERROR;
    }

    return dwNew;
}

DWORD CFileIO::GetPosition() const
{
    DWORD dwPos = ::SetFilePointer((HANDLE)m_hFile, 0, NULL, FILE_CURRENT);
    if (dwPos  == (DWORD)-1)
    {
        return FILEIO_ERROR;
    }

    return dwPos;
}

void CFileIO::Flush()
{
    if (m_hFile == (UINT)hFileNull)
        return;

    if (!::FlushFileBuffers((HANDLE)m_hFile))
    {
        bError = TRUE;
        return;
    }
}

void CFileIO::Close()
{
    bError = FALSE;
    if (m_hFile != (UINT)hFileNull)
        bError = ::CloseHandle((HANDLE)m_hFile);

    m_hFile = hFileNull;
    m_bCloseOnDelete = FALSE;
}

void CFileIO::Abort()
{
    if (m_hFile != (UINT)hFileNull)
    {
        // close but ignore errors
        ::CloseHandle((HANDLE)m_hFile);
        m_hFile = (UINT)hFileNull;
    }
}

void CFileIO::LockRange(DWORD dwPos, DWORD dwCount)
{
    if (!::LockFile((HANDLE)m_hFile, dwPos, 0, dwCount, 0))
    {
        bError = TRUE;
    }
}

void CFileIO::UnlockRange(DWORD dwPos, DWORD dwCount)
{
    if (!::UnlockFile((HANDLE)m_hFile, dwPos, 0, dwCount, 0))
    {
        bError = TRUE;
    }
}

void CFileIO::SetLength(DWORD dwNewLen)
{
    Seek((LONG)dwNewLen, (UINT)begin);

    if (!::SetEndOfFile((HANDLE)m_hFile))
    {
        bError = TRUE;
    }
}

DWORD CFileIO::GetLength()
{
    DWORD dwLen, dwCur;

    // Seek is a non const operation
    dwCur = ((CFileIO*)this)->Seek(0L, current);
    dwLen = SeekToEnd();

    return dwLen;
}

// CFileIO does not support direct buffering (CMemFile does)
UINT CFileIO::GetBufferPtr(UINT nCommand, UINT /*nCount*/,
    void** /*ppBufStart*/, void** /*ppBufMax*/)
{
    nCommand;   // not used in retail build

    return 0;   // no support
}

BOOL PASCAL CFileIO::Rename(LPCTSTR lpszOldName, LPCTSTR lpszNewName)
{
    if (!::MoveFile((LPTSTR)lpszOldName, (LPTSTR)lpszNewName))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL PASCAL CFileIO::Remove(LPCTSTR lpszFileName)
{
    if (!::DeleteFile((LPTSTR)lpszFileName))
    {
        return FALSE;
    }

    return TRUE;
}

/////////////////////////////////////////////////////////////////////

DWORD CFileIO::ReadHuge(void* lpBuffer, DWORD dwCount)
    { return (DWORD)Read(lpBuffer, (UINT)dwCount); }
void CFileIO::WriteHuge(const void* lpBuffer, DWORD dwCount)
    { Write(lpBuffer, (UINT)dwCount); }

DWORD CFileIO::SeekToEnd()
    { return Seek(0, CFileIO::end); }
void CFileIO::SeekToBegin()
    { Seek(0, CFileIO::begin); }
