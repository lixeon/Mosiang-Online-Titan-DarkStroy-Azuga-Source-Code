// RegenConditionInfo.h: interface for the CRegenConditionInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGENCONDITIONINFO_H__F56693A5_F330_484B_870E_843F3B71A2A4__INCLUDED_)
#define AFX_REGENCONDITIONINFO_H__F56693A5_F330_484B_870E_843F3B71A2A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRegenConditionInfo  
{
	DWORD	dwTargetGroupID;
	float	fRemainderRatio;
	DWORD	dwStartRegenTick;
	DWORD	dwRegenDelay;
	BOOL	bRegen;
	friend class CGroupRegenInfo;
	friend class CAISystem;
public:
	CRegenConditionInfo();
	virtual ~CRegenConditionInfo();
};

#endif // !defined(AFX_REGENCONDITIONINFO_H__F56693A5_F330_484B_870E_843F3B71A2A4__INCLUDED_)
