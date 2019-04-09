// QuestNpcScriptData.h: interface for the CQuestNpcScriptData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTNPCSCRIPTDATA_H__63F69AC4_B743_475D_AC95_E69AA473E5A1__INCLUDED_)
#define AFX_QUESTNPCSCRIPTDATA_H__63F69AC4_B743_475D_AC95_E69AA473E5A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStrTokenizer;

class CQuestNpcScriptData  
{
protected:
	DWORD		m_dwNpcIdx;
	DWORD		m_dwScriptPage;
	DWORD		m_dwNpcMarkType;
	
public:
	CQuestNpcScriptData( DWORD dwKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~CQuestNpcScriptData();

	inline DWORD	IsSameNpc( DWORD dwNpcIdx )		{ return (m_dwNpcIdx == dwNpcIdx) ? m_dwScriptPage : 0; }
	inline DWORD	IsSameNpcMark( DWORD dwNpcIdx )	{ return (m_dwNpcIdx == dwNpcIdx) ? m_dwNpcMarkType : 0; }
	inline DWORD	GetNpcScriptPage()				{ return m_dwScriptPage; }
};

#endif // !defined(AFX_QUESTNPCSCRIPTDATA_H__63F69AC4_B743_475D_AC95_E69AA473E5A1__INCLUDED_)
