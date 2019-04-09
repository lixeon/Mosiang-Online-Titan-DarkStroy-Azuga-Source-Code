#include "stdafx.h"
#include "ChangeValue.h"
#include "ChangeManager.h"
#include "BaseDoc.h"

CChangeManager::CChangeManager()
{	
	m_pChangeEditInfo	= NULL;
	m_pChangeBtnInfo	= NULL;

	m_pDocument			= NULL;
}

CChangeManager::~CChangeManager()
{
	DeleteAll();
	SAFE_DELETE(m_pChangeEditInfo);
	SAFE_DELETE(m_pChangeBtnInfo);
}

void CChangeManager::Init(CBaseDoc* pDoc)
{
	m_pChangeEditInfo = new CYHHashTable<CChangeValue>;
	m_pChangeEditInfo->Initialize(2000);

	m_pChangeBtnInfo = new CYHHashTable<CChangeValue>;
	m_pChangeBtnInfo->Initialize(2000);

	m_ChangePosition.Initialize( 1000 );

	m_pDocument = pDoc;
}

/* ID´Â ¼öÁ¤ ÄÁÆ®·Ñ ¾ÆÀÌµð¸¦ »ç¿ëÇÑ´Ù. */
void CChangeManager::Add(CChangeValue* info, DWORD Idx, BOOL bEdit)
{
	if( bEdit ) // Control ID
		m_pChangeEditInfo->Add( info, Idx );
	else // 
		m_pChangeBtnInfo->Add( info, Idx );
}

void CChangeManager::AddPositionTable( CChangeValue* pInfo, DWORD dwItemDBIdx )
{
	m_ChangePosition.Add( pInfo, dwItemDBIdx );
}

void CChangeManager::Delete(DWORD Idx, BOOL bEdit)
{
	CChangeValue* pValue;

	if(bEdit)
	{
		pValue = m_pChangeEditInfo->GetData(Idx);

		SAFE_DELETE(pValue);
		m_pChangeEditInfo->Remove(Idx);
	}
	else
	{
		pValue = m_pChangeBtnInfo->GetData(Idx);

		SAFE_DELETE(pValue);
		m_pChangeBtnInfo->Remove(Idx);
	}
}


void CChangeManager::DeleteAll()
{
	CChangeValue* pValue;

	m_pChangeEditInfo->SetPositionHead();
	while( pValue = m_pChangeEditInfo->GetData() )
		delete pValue;
	m_pChangeEditInfo->RemoveAll();

	m_pChangeBtnInfo->SetPositionHead();
	while( pValue = m_pChangeBtnInfo->GetData() )
		delete pValue;
	m_pChangeBtnInfo->RemoveAll();

	m_ChangePosition.SetPositionHead();
	while( pValue = m_ChangePosition.GetData() )
		delete pValue;
	m_ChangePosition.RemoveAll();
}

CChangeValue* CChangeManager::ExistChangeInfo(DWORD Idx, BOOL bEdit)
{
	if( bEdit )
		return m_pChangeEditInfo->GetData( Idx );
	else
		return m_pChangeBtnInfo->GetData( Idx );
}


void CChangeManager::ProcessList()
{
	CChangeValue* pValue;

	m_pChangeEditInfo->SetPositionHead();
	while( pValue = m_pChangeEditInfo->GetData() )
	{
		// º¯°æ »çÇ×À» Àû¿ëÇÑ´Ù.
		m_pDocument->UpdateData( MAKEWPARAM(pValue->GetTableKind(), pValue->GetDataPos()) );
		pValue->ApplyChangeValue(m_pDocument);
	}

	m_pChangeBtnInfo->SetPositionHead();
	while( pValue = m_pChangeBtnInfo->GetData() )
	{
		// º¯°æ »çÇ×À» Àû¿ëÇÑ´Ù.
		pValue->ApplyChangeValue(m_pDocument);
	}

	m_ChangePosition.SetPositionHead();
	while( pValue = m_ChangePosition.GetData() )
	{
		pValue->ApplyChangeValue( m_pDocument );
	}

	DeleteAll();
	
	m_pDocument->SaveData();
}

TABLE_KIND CChangeManager::GetTableKind_ByField(INFOFIELD field)
{
	switch(field)
	{
	case eIF_INVENTORY:
	case eIF_WEAR:
		return eTK_InventoryItem;
	case eIF_PHYOGUK:
		return eTK_PyogukItem;
	case eIF_MUGONG:
	case eIF_JINBUB:
		return eTK_MugongInfo;
	case eIF_MUNPA:
		return eTK_MunpaItem;
	}
	return eTK_None;
}
