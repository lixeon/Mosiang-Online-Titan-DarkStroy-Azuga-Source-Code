// AniInfoManager.h: interface for the CAniInfoManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIINFOMANAGER_H__35540CD2_EF5B_4018_B13D_EF82A48DF691__INCLUDED_)
#define AFX_ANIINFOMANAGER_H__35540CD2_EF5B_4018_B13D_EF82A48DF691__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AniInfo.h"
#include "PtrList.h"

#define MAX_BYTE_VALUE	256

class CAniInfoManager  
{
	//////////////////////////////////////////////////////////////////////////
	// pooling
	struct ANIINFOOBJ
	{
		ANIINFOOBJ(char* fname,CAniInfo* painfo)
		{
			strcpy(filename,fname);
			pAniInfo = painfo;
		}
		char filename[MAX_BYTE_VALUE];
		CAniInfo* pAniInfo;
	};
	cPtrList m_AniInfoList[256];
	CAniInfo* FindAniInfoFromPool(char* filename);
	void AddAniInfoToPool(char* filename,CAniInfo* pAniInfo);
	//////////////////////////////////////////////////////////////////////////
	
	CAniInfo* LoadAniInfoFile(char* filename,GXOBJECT_HANDLE handle);

public:
	CAniInfoManager();
	~CAniInfoManager();
	
	CAniInfo* GetAniInfoObj(char* filename,GXOBJECT_HANDLE handle);
	CAniInfo* GetAniInfoObj(char* filename,GXOBJECT_HANDLE handle,WORD StartFrame,WORD EndFrame);
};

#endif // !defined(AFX_ANIINFOMANAGER_H__35540CD2_EF5B_4018_B13D_EF82A48DF691__INCLUDED_)
