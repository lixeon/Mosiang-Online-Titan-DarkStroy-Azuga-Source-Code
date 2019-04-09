#ifndef __PAGE__
#define __PAGE__

#include "cLinkedList.h"

class cPageBase 
{
protected:
	DWORD					m_dwPageId; // PageÀÇ ±¸ºÐÀÚ

	cLinkedList<DWORD>		m_pDialogue;
	int						m_nDialogueCount;			// µî·ÏµÈ DialogueÀÇ °¹¼ö
	
	int						m_nNextPageId;
	int						m_nPrevPageId;
public:
	cPageBase();
	virtual ~cPageBase();

	virtual void Init( DWORD dwId );

	void AddDialogue(DWORD dwId);
	virtual void RemoveAll();

	DWORD GetPageId() { return m_dwPageId; }
	DWORD GetRandomDialogue();			// ´ë»ç Á¤º¸¸¦ ·£´ýÇÏ°Ô È¹µæÇÑ´Ù.
	int GetDialogueCount() { return m_nDialogueCount; }

	void SetNextPageId(int id) { m_nNextPageId = id; }
	void SetPrevPageId(int id) { m_nPrevPageId = id; }
	int GetNextPageId() { return m_nNextPageId; }
	int GetPrevPageId() { return m_nPrevPageId; }
};


// HyperLink¸¦ Ã·°¡ÇÑ´Ù.
class cPage : public cPageBase
{
protected:
	cLinkedList<HYPERLINK>	m_pHyperLink;
	int						m_nHyperLinkCount;

public:
	cPage();
	virtual ~cPage();

	void AddHyperLink(HYPERLINK* pLink);
	virtual void RemoveAll();

	int GetHyperLinkCount() { return m_nHyperLinkCount; }
	HYPERLINK* GetHyperText( DWORD dwIdx );
};

#endif