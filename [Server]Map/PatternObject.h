// PatternObject.h: interface for the CPatternObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATTERNOBJECT_H__79F708F6_269C_4DB6_83B6_B4F5507B48E5__INCLUDED_)
#define AFX_PATTERNOBJECT_H__79F708F6_269C_4DB6_83B6_B4F5507B48E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum
{
	PO_NULL,
	PO_ATTACKINDEX,				// °ø°ÝÀÎµ¦½º
};

class CPatternObject  
{
public:
	CPatternObject();
	virtual ~CPatternObject();

	void		SetType(WORD type)		{	m_type = type;	}
	void		SetData(DWORD data)		{	m_Data1 = data;	}
	void		SetData(void *	data)	{	m_Data2 = data;	}
	void		SetNext(CPatternObject * next)	{	pNext = next;	}
protected:
	WORD		m_type;
	DWORD		m_Data1;
	void *		m_Data2;

	CPatternObject * pNext;
};

#endif // !defined(AFX_PATTERNOBJECT_H__79F708F6_269C_4DB6_83B6_B4F5507B48E5__INCLUDED_)

