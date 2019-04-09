// EffectDescManager.h: interface for the CEffectDescManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTDESCMANAGER_H__267F7AC9_72BC_4867_88D6_EBFEB98350F3__INCLUDED_)
#define AFX_EFFECTDESCMANAGER_H__267F7AC9_72BC_4867_88D6_EBFEB98350F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Effect.h"

class CEffectDesc;
class CMHFile;

class CEffectDescManager  
{	
	DWORD m_MaxEffectDesc;
	CEffectDesc* m_pEffectDescArray;
public:
	CEffectDescManager();
	virtual ~CEffectDescManager();

	BOOL LoadEffectList(int EffectKind,CMHFile* pFile);
	void Release();

	BOOL GetEffect(int EffectDescNum,EFFECTPARAM* pParam,CEffect* pOutEffect,BOOL bWarning=TRUE);

	CEffectDesc* GetEffectDesc(int EffectDescNum);
};

#endif // !defined(AFX_EFFECTDESCMANAGER_H__267F7AC9_72BC_4867_88D6_EBFEB98350F3__INCLUDED_)
