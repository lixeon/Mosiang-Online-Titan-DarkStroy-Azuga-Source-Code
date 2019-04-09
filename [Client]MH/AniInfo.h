// AniInfo.h: interface for the CAniInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIINFO_H__CA1F68D8_E7B3_41CF_A7A2_C195592AF1EA__INCLUDED_)
#define AFX_ANIINFO_H__CA1F68D8_E7B3_41CF_A7A2_C195592AF1EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct ANIINFO
{
	WORD EngineMotionIndex;
	WORD StartFrame;
	WORD EndFrame;
	ANIINFO()
	:	EngineMotionIndex(1)
	,	StartFrame(0)
	,	EndFrame(-1)
	{
	}
};

class CAniInfo  
{
	DWORD m_MaxAniNum;
	ANIINFO* m_AniInfo;

public:
	CAniInfo(char* filename,DWORD MaxAniNum);
	~CAniInfo();

	inline DWORD GetMaxAniNum()	{	return m_MaxAniNum-1;	}

	inline ANIINFO* GetAniInfo(DWORD MotionNum)
	{
		static ANIINFO m_DefaultAniInfo;
		if(GetMaxAniNum() == 0)
			return &m_DefaultAniInfo;
//		ASSERT(MotionNum < m_MaxAniNum);
		if(MotionNum >= m_MaxAniNum)
			return &m_DefaultAniInfo;
		return &m_AniInfo[MotionNum];
	}
};



#endif // !defined(AFX_ANIINFO_H__CA1F68D8_E7B3_41CF_A7A2_C195592AF1EA__INCLUDED_)
