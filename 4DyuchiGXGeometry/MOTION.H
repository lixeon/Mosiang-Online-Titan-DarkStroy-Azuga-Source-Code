#pragma once


#include "motion_obj.h"
#include "../4DyuchiGRX_common/typedef.h"
#include "../4DyuchiGXGFunc/global.h"

class CoGeometry;

struct FILE_MOTION_HEADER
{
	DWORD					dwVersion;
	DWORD					dwTicksPerFrame;
	DWORD					dwFirstFrame;
	DWORD					dwLastFrame;
	DWORD					dwFrameSpeed;
	DWORD					dwMotionObjectNum;
	DWORD					dwKeyFrameStep;
	CMotionObject*			pBaseMotionObject;
	char					szMotionName[MAX_NAME_LEN];
};

class CMotion : public IUnknown
{
	DWORD					m_dwTicksPerFrame;
	DWORD					m_dwFirstFrame;
	DWORD					m_dwLastFrame;
	DWORD					m_dwFrameSpeed;
	DWORD					m_dwMotionObjectNum;
	DWORD					m_dwKeyFrameStep;
	CMotionObject*			m_pBaseMotionObject;
	char					m_szMotionFileName[MAX_NAME_LEN];

	DWORD					m_dwRefCount;
	CoGeometry*				m_pGeometry;
	DWORD					m_dwIndexInGeometry;
	DWORD					m_dwMaxNameLen;

	// Various Bytes HashTable
	VBHASH_HANDLE			m_pHashObjName;
	

	CMotionObject*			AllocMotionObject();
public:

    STDMETHODIMP			QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);

	BOOL					GetMotionDesc(MOTION_DESC* pDesc);
	DWORD					GetLastFrame() {return m_dwLastFrame;}
	void					SetGeometry(CoGeometry* pGeometry) {m_pGeometry = pGeometry;}
	void					SetIndexInGeometry(DWORD dwIndex) {m_dwIndexInGeometry = dwIndex;}
	DWORD					GetFileIndex() {return m_dwIndexInGeometry;}
	char*					GetMotionFileName()	{return m_szMotionFileName;}
	CMotionObject*			GetMotionObject(char* szObjName,DWORD dwSize);
	BOOL					ReadFile(char* szFileName);
	void					ReleaseObjects();
	BOOL					Unload();



	
	CMotion();
	~CMotion();
	
	CMotion*				m_pNextMotion;
	
	
	
};

