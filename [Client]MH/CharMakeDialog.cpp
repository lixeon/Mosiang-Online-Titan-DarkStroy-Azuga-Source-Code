#include "stdafx.h"
#include "CharMakeDialog.h"
#include "WindowIDEnum.h"
#include ".\interface\cWindowManager.h"
#include "interface/cButton.h"
//#include "cComboBoxEx.h"
#include "interface/cStatic.h"

#include "CharMakeManager.h"

cCharMakeDlg::cCharMakeDlg()
{
	m_type = WT_CHARMAKEDIALOG;
}

cCharMakeDlg::~cCharMakeDlg()
{
}

void cCharMakeDlg::Linking()
{
//	m_pMHair = 	(cComboBoxEx*) GetWindowForID(CMID_ManHairType);
//	m_pWMHair = (cComboBoxEx*) GetWindowForID(CMID_WomanHairType);
//	m_pMFace = (cComboBoxEx*) GetWindowForID(CMID_ManFaceType);
//	m_pWMFace = (cComboBoxEx*) GetWindowForID(CMID_WomanFaceType);

	m_pMHair = (cStatic*)GetWindowForID(CMID_ManHairType);
	m_pWMHair = (cStatic*)GetWindowForID(CMID_WomanHairType);
	m_pMFace = (cStatic*)GetWindowForID(CMID_ManFaceType);
	m_pWMFace = (cStatic*)GetWindowForID(CMID_WomanFaceType);

}

void cCharMakeDlg::ChangeComboStatus( WORD wSex )
{
	if( wSex == 0 ) // ³²ÀÚÀÎ °æ¿ì
	{
		m_pMHair->SetActive( TRUE );
		m_pWMHair->SetActive( FALSE );

		m_pMFace->SetActive( TRUE );
		m_pWMFace->SetActive( FALSE );
	}
	else // ¿©ÀÚÀÎ °æ¿ì
	{
		m_pMHair->SetActive( FALSE );
		m_pWMHair->SetActive( TRUE );

		m_pMFace->SetActive( FALSE );
		m_pWMFace->SetActive( TRUE );
	}
}

void cCharMakeDlg::OnActionEvent(LONG lId, void* p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		if( lId == CMID_SexLeft )
			CHARMAKEMGR->RotateSelection( CE_SEX, CM_PREV );
		else if( lId == CMID_SexRight )
			CHARMAKEMGR->RotateSelection( CE_SEX, CM_NEXT );

		else if( lId == CMID_HairLeft && m_pMHair->IsActive() )
			CHARMAKEMGR->RotateSelection( CE_MHAIR, CM_PREV );
		else if( lId == CMID_HairRight && m_pMHair->IsActive() )
			CHARMAKEMGR->RotateSelection( CE_MHAIR, CM_NEXT );
		else if( lId == CMID_HairLeft && m_pWMHair->IsActive() )
			CHARMAKEMGR->RotateSelection( CE_WMHAIR, CM_PREV );
		else if( lId == CMID_HairRight && m_pWMHair->IsActive() )
			CHARMAKEMGR->RotateSelection( CE_WMHAIR, CM_NEXT );

		else if( lId == CMID_FaceLeft && m_pMHair->IsActive() )
			CHARMAKEMGR->RotateSelection( CE_MFACE, CM_PREV );
		else if( lId == CMID_FaceRight && m_pMHair->IsActive() )
			CHARMAKEMGR->RotateSelection( CE_MFACE, CM_NEXT );
		else if( lId == CMID_FaceLeft && m_pWMHair->IsActive() )
			CHARMAKEMGR->RotateSelection( CE_WMFACE, CM_PREV );
		else if( lId == CMID_FaceRight && m_pWMHair->IsActive() )
			CHARMAKEMGR->RotateSelection( CE_WMFACE, CM_NEXT );
		
		else if( lId == CMID_ClothLeft )
			CHARMAKEMGR->RotateSelection( CE_WEAR, CM_PREV );
		else if( lId == CMID_ClothRight )
			CHARMAKEMGR->RotateSelection( CE_WEAR, CM_NEXT );
		else if( lId == CMID_BootLeft )
			CHARMAKEMGR->RotateSelection( CE_BOOT, CM_PREV );
		else if( lId == CMID_BootRight )
			CHARMAKEMGR->RotateSelection( CE_BOOT, CM_NEXT );
		else if( lId == CMID_WeaponLeft )
			CHARMAKEMGR->RotateSelection( CE_WEAPON, CM_PREV );
		else if( lId == CMID_WeaponRight )
			CHARMAKEMGR->RotateSelection( CE_WEAPON, CM_NEXT );
#ifdef _JAPAN_LOCAL_
		else if( lId == CMID_AttribLeft )
			CHARMAKEMGR->RotateSelection( CE_ATTRIBUTE, CM_PREV );
		else if( lId == CMID_AttribRight )
			CHARMAKEMGR->RotateSelection( CE_ATTRIBUTE, CM_NEXT );
#endif
	}
	
}
