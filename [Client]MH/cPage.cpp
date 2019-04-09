#include "stdafx.h"
#include "cPage.h"
#include "mhtimemanager.h"


/////////////////////////////////
// cPageBase Class
/////////////////////////////////
cPageBase::cPageBase()
{
	m_nDialogueCount = 0;
	
	m_nNextPageId = -1;
	m_nPrevPageId = -1;
}


cPageBase::~cPageBase()
{
	RemoveAll();
}

void cPageBase::RemoveAll()
{
	POS pos = m_pDialogue.GetFirstPos();
	while( DWORD* pData = m_pDialogue.GetNextPos( pos ) )
	{
		delete pData;
	}
	m_pDialogue.DeleteAll();
}

void cPageBase::Init( DWORD dwId )
{
	m_dwPageId = dwId;
}

void cPageBase::AddDialogue(DWORD dwId)
{
	DWORD* dwVal = new DWORD;
	*dwVal = dwId;
	m_pDialogue.AddTail(dwVal);
	++m_nDialogueCount;
}

DWORD cPageBase::GetRandomDialogue()
{
	if( m_nDialogueCount == 0 ) return 0;

	if( m_nDialogueCount > 1 )
	{
		return *(m_pDialogue.GetAt(rand()%m_nDialogueCount));
	}
	else
	{
		return *(m_pDialogue.GetAt(0));
	}
}

/////////////////////////////////
// cPage Class
/////////////////////////////////
cPage::cPage()
{
	m_nHyperLinkCount = 0;
}

cPage::~cPage()
{
	RemoveAll();
}

void cPage::RemoveAll()
{
	POS pos = m_pHyperLink.GetFirstPos();
	while( HYPERLINK* pHL = m_pHyperLink.GetNextPos( pos ) )
	{
		delete pHL;
	}
	m_pHyperLink.DeleteAll();
}

void cPage::AddHyperLink(HYPERLINK* pLink)
{
	HYPERLINK* pVal = new HYPERLINK;
	memcpy( pVal, pLink, sizeof(HYPERLINK) );
	m_pHyperLink.AddTail( pVal );
	++m_nHyperLinkCount;
}

HYPERLINK* cPage::GetHyperText( DWORD dwIdx )
{
	return m_pHyperLink.GetAt(dwIdx);
}