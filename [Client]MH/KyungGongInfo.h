// KyungGongInfo.h: interface for the CKyungGongInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KYUNGGONGINFO_H__948CB341_A74B_46B0_B404_95D6598E9E46__INCLUDED_)
#define AFX_KYUNGGONGINFO_H__948CB341_A74B_46B0_B404_95D6598E9E46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MHFile.h"

class CKyungGongInfo  
{
	KYUNGGONG_INFO m_Info;
public:
	CKyungGongInfo();
	virtual ~CKyungGongInfo();

	void Init(CMHFile* pFile);

	WORD GetKyungGongIdx()	{	return m_Info.KyungGongIdx;	}
	char* GetKyungGongName()	{	return m_Info.KyungGongName;	}
	WORD GetNeedNaeRyuk()	{	return m_Info.NeedNaeRyuk;	}
	WORD GetMoveType()	{	return m_Info.MoveType;	}
	float GetSpeed()	{	return m_Info.Speed;	}
	WORD GetChangeTime()	{	return m_Info.ChangeTime;	}
	WORD GetStartEffect()	{	return m_Info.StartEffect;	}
	WORD GetIngEffect()	{	return m_Info.IngEffect;	}
	WORD GetEndEffect()	{	return m_Info.EndEffect;	}
};

#endif // !defined(AFX_KYUNGGONGINFO_H__948CB341_A74B_46B0_B404_95D6598E9E46__INCLUDED_)
