// MugongDialog.cpp: implementation of the CMugongDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _MUGONGDLGBACKUP_

#include "MugongDialog.h"
#include "GameResourceManager.h"
#include "MugongManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "WindowIDEnum.h"
#include ".\Interface\cWindowManager.h"
#include "Item.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "SkillManager_Client.h"
#include "SkillInfo.h"

CMugongDialog::CMugongDialog()
{
	m_type = WT_MUGONGDIALOG;
	
	m_pMugongDlg1_1 = NULL;
	m_pMugongDlg1_2 = NULL;
	m_pMugongSpreadBtn = NULL;
	/*
	for(int j=0;j<3;j++)
	for(int i=0;i<6;i++)
		pWearedMugongInfo[j].pStatic[i] = NULL;
	*/
}

CMugongDialog::~CMugongDialog()
{
	m_pMugongDlg1_1 = NULL;
	m_pMugongDlg1_2 = NULL;
	
	m_pMugongSpreadBtn = NULL;			// window manager에서 삭제함
	DeleteIconAll();
}

void CMugongDialog::Linking()
{
	pWearedMugongInfo[0][MGI_NAME] = (cStatic *)GetWindowForID(MGI_GENERICMUGONG_NAME);
	pWearedMugongInfo[0][MGI_ATTR] = (cStatic *)GetWindowForID(MGI_GENERICMUGONG_ATTR);
	pWearedMugongInfo[0][MGI_TYPE] = (cStatic *)GetWindowForID(MGI_GENERICMUGONG_TYPE);
	pWearedMugongInfo[0][MGI_KIND] = (cStatic *)GetWindowForID(MGI_GENERICMUGONG_KIND);
	pWearedMugongInfo[0][MGI_SUNG] = (cStatic *)GetWindowForID(MGI_GENERICMUGONG_SUNG);
	pWearedMugongInfo[0][MGI_EXPPOINT] = (cStatic *)GetWindowForID(MGI_GENERICMUGONG_EXPPOINT);
}

void CMugongDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cIconDialog::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_MUGONGDIALOG;
}

DWORD CMugongDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	
	we = cIconDialog::ActionEvent(mouseInfo);
	
	if(m_pMugongSpreadBtn->IsPushed())				//추가 : 수정
	{
		m_pMugongDlg1_1->SetActive(FALSE);
		m_pMugongDlg1_2->SetActive(TRUE);
		m_pGuageBar->SetActive(FALSE);
	}
	else
	{
		m_pMugongDlg1_1->SetActive(TRUE);
		m_pMugongDlg1_2->SetActive(FALSE);
		m_pGuageBar->SetActive(TRUE);
	}
	return we;
}

void CMugongDialog::Add(cWindow * window)
{
	if(window->GetID() == MGI_MUGONGDLG1_1)
	{
		m_pMugongDlg1_1 = (cIconDialog *)window;
	}
	else if(window->GetID() == MGI_MUGONGDLG1_2)
	{
		m_pMugongDlg1_2 = (cIconGridDialog *)window;
	}
	else if(window->GetID() == MGI_PUSHUPBTN)
	{
		m_pMugongSpreadBtn = (cPushupButton *)window;
	}
	cDialog::Add(window);
}

void CMugongDialog::Render()
{
	cIconDialog::Render();
}

void CMugongDialog::SetAbsXY(LONG x, LONG y)
{
	ASSERT(m_pMugongDlg1_1);
	ASSERT(m_pMugongDlg1_2);
	m_pMugongDlg1_1->SetAbsXY(m_pMugongDlg1_1->GetRelX()+m_absPos.x, m_pMugongDlg1_1->GetRelY()+m_absPos.y);
	m_pMugongDlg1_2->SetAbsXY(m_pMugongDlg1_2->GetRelX()+m_absPos.x, m_pMugongDlg1_2->GetRelY()+m_absPos.y);
	
	cIconDialog::SetAbsXY(x,y);
}
void CMugongDialog::SetActive(BOOL val)
{
	cIconDialog::SetActive(val);
	m_pMugongDlg1_1->SetActive(val);
	m_pMugongDlg1_2->SetActive(val);

	if(m_pMugongSpreadBtn->IsPushed())
	{
		m_pMugongDlg1_1->SetActive(FALSE);
		m_pMugongDlg1_2->SetActive(TRUE);
		m_pGuageBar->SetActive(FALSE);
	}
	else
	{
		m_pMugongDlg1_1->SetActive(TRUE);
		m_pMugongDlg1_2->SetActive(FALSE);
		m_pGuageBar->SetActive(TRUE);
	}
	
}
void CMugongDialog::UpdateData(MUGONG_TYPE type)
{
	switch(type)
	{
	case ALL:
		{
			UpdateData(MUGONG);
			UpdateData(NAEGONG);
			UpdateData(KYUNGGONG);
		}
		break;
	case MUGONG:
		{
			/*
			pWearedMugongInfo[0].pStatic[0]->SetStaticText(pWearedMugongInfo[0].name);
			for(int i = 1 ;i < 6 ; i++)
				pWearedMugongInfo[0].pStatic[i]->SetStaticValue(pWearedMugongInfo[0].numeric[i]);
				*/
		}
		break;
	case NAEGONG:
		{
			/*
			pWearedMugongInfo[1].pStatic[0]->SetStaticText(pWearedMugongInfo[1].name);
			for(int i = 1 ;i < 6 ; i++)
				pWearedMugongInfo[1].pStatic[i]->SetStaticValue(pWearedMugongInfo[1].numeric[i]);
				*/
		}
		break;
	case KYUNGGONG:
		{
			/*
			pWearedMugongInfo[2].pStatic[0]->SetStaticText(pWearedMugongInfo[2].name);
			for(int i = 1 ;i < 6 ; i++)
				pWearedMugongInfo[2].pStatic[i]->SetStaticValue(pWearedMugongInfo[2].numeric[i]);
				*/
		}
		break;
	}
}
/*
void CMugongDialog::FakeAddIcon(LONG mouseX, LONG mouseY, cIcon * icon)
{
	if(!IsAcceptable(icon->GetIconType()))
		return;
	
	WORD pos;
	if(!GetPositionForXYRef(mouseX, mouseY, pos))
		return ;
	MSG_MUGONG_ADDINVDEL_SYN msg;
	msg.Category = MP_MUGONG;
	msg.Protocol = MP_MUGONG_ADDNITEMDELETE_SYN;
	msg.dwObjectID = HEROID;
	msg.bMugongType = pos;
	msg.wItemPosition = ((CItem *)icon)->GetPosition();
	msg.dwItemDBIdx = ((CItem *)icon)->GetDBIdx();
	NETWORK->Send(&msg, sizeof(MSG_MUGONG_ADDINVDEL_SYN));
}
*/
BOOL CMugongDialog::GetBlankPositionRef(BYTE type, WORD & absPos)
{
	switch(type)
	{
	case eMUGONGBOOK:
		{
			for(int i =0 ; i < GetCellNum() ; ++i)
			{
				if(IsAddable(i))
				{
					absPos = MUGONG_STARTPOSITION + i;
					return TRUE;
				}
			}
			for(i = 0 ; i < m_pMugongDlg1_2->GetCellNum() ; ++i)
			{
				if(m_pMugongDlg1_2->IsAddable(i))
				{
					absPos = MUGONG1_2_STARTPOSITION + i;
					return TRUE;
				}
			}
		}
		break;
	case eNAEGONGBOOK:
		{
			if(m_pMugongDlg1_1->IsAddable(0))
			{
				absPos = MUGONG1_1_STARTPOSITION + 0;
				return TRUE;
			}
			
		}
		break;
	case eKYUNGGONGBOOK:
		{
			if(m_pMugongDlg1_1->IsAddable(1))
			{
				absPos = MUGONG1_1_STARTPOSITION + 1;
				return TRUE;
			}
		}
		break;
	}
	
	return FALSE;
}

void CMugongDialog::AddMugong(MUGONGBASE * mugongBase)
{
	if(mugongBase->dwDBIdx > 0)
	{
		if(IsAddMugong(mugongBase->Position))
		{
			CMugongBase * mugong = MUGONGMGR->NewMugongIcon(mugongBase);
			
			AddMugong(mugongBase->Position, mugong);
		}
	}
}

BOOL CMugongDialog::IsAddMugong(POSTYPE pos)
{
	switch(ConvAbs2Rel(pos))
	{
	case WINDOW_STD:
		{
			pos -= MUGONG_STARTPOSITION;
			return IsAddable(pos);		
		}
		break;
	case WINDOW_EXTEND:
		{
			pos -= MUGONG1_2_STARTPOSITION;
			return m_pMugongDlg1_2->IsAddable(pos);		
		}
		break;
	case WINDOW_DOWN:
		{
			pos -= MUGONG1_1_STARTPOSITION;
			return m_pMugongDlg1_1->IsAddable(pos);		
		}
		break;
	default:
		ASSERT(0);
	}

	return FALSE;
}

void SetSkillInfoStatic(cStatic** pMugongInfoStatic,SKILLINFO* pSkillInfo,MUGONGBASE * mugongBase)
{
	pMugongInfoStatic[MGI_NAME]->SetStaticText(pSkillInfo->SkillName);
	pMugongInfoStatic[MGI_ATTR]->SetStaticValue(0);
	pMugongInfoStatic[MGI_TYPE]->SetStaticValue(pSkillInfo->WeaponKind);
	pMugongInfoStatic[MGI_KIND]->SetStaticValue(pSkillInfo->SkillKind);
	pMugongInfoStatic[MGI_SUNG]->SetStaticValue(mugongBase->Sung);
	pMugongInfoStatic[MGI_EXPPOINT]->SetStaticValue(mugongBase->ExpPoint);
}

void CMugongDialog::AddMugong(POSTYPE pos/*abs pos*/, CMugongBase * mugong)
{
	ASSERT(mugong);
	
	switch(ConvAbs2Rel(pos))
	{
	case WINDOW_STD:
		{
			pos -= MUGONG_STARTPOSITION;
			if(pos == MUGONG_POSITION) mugong->SetWeared(1);
			if(mugong->IsWeared()) 
			{
				
				MUGONGBASE * mugongBase = mugong->GetMugongBase();
				CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(mugongBase->wIconIdx);
				SetSkillInfoStatic(pWearedMugongInfo[0],pSkillInfo->GetSkillInfo(),mugongBase);
			}
			//test
			//char buf[100];
			//wsprintf(buf, "  성:%s  ", mugong->GetMugongBase()->Sung);
			//mugong->SetToolTip(buf);

			if(AddIcon(pos, (cIcon *)mugong))
				return ;
			else
				return ;
		}
		break;
	case WINDOW_EXTEND:
		{
			pos -= MUGONG1_2_STARTPOSITION;
			//test
			//char buf[100];
			//wsprintf(buf, "  성:%s  ", mugong->GetMugongBase()->Sung);
			//mugong->SetToolTip(buf);
			if(m_pMugongDlg1_2->AddIcon(pos, (cIcon *)mugong))
				return ;
			else
				return ;
		}
		break;
	case WINDOW_DOWN:
		{
			pos -= MUGONG1_1_STARTPOSITION;
			switch(pos) 
			{
			case 0:
				{
					MUGONGBASE * mugongBase = mugong->GetMugongBase();
					CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(mugongBase->wIconIdx);
					SetSkillInfoStatic(pWearedMugongInfo[1],pSkillInfo->GetSkillInfo(),mugongBase);
				}
			case 1:
				{
					MUGONGBASE * mugongBase = mugong->GetMugongBase();
					CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(mugongBase->wIconIdx);
					SetSkillInfoStatic(pWearedMugongInfo[2],pSkillInfo->GetSkillInfo(),mugongBase);
				}
			}
			//test
			//char buf[100];
			//wsprintf(buf, "  성:%s  ", mugong->GetMugongBase()->Sung);
			//mugong->SetToolTip(buf);
			mugong->SetWeared(1);
			if(m_pMugongDlg1_1->AddIcon(pos, (cIcon *)mugong))
				return ;
			else
				return ;
		}
		break;
	default:
		return ;
	}
}


MLOC CMugongDialog::ConvAbs2Rel(POSTYPE abs_pos/*abs pos*/)
{
	if(MUGONG_STARTPOSITION <= abs_pos && abs_pos < MUGONG_ENDPOSITION)
	{
		return WINDOW_STD;
	}
	else if(MUGONG1_2_STARTPOSITION <= abs_pos && abs_pos < MUGONG1_2_ENDPOSITION)
	{
		return WINDOW_EXTEND;
	}
	else if(MUGONG1_1_STARTPOSITION <= abs_pos && abs_pos < MUGONG1_1_ENDPOSITION)
	{
		return WINDOW_DOWN;
	}
	
	return WINDOW_ERR;
}
BOOL CMugongDialog::DeleteMugong(POSTYPE pos/*abs pos*/, CMugongBase ** mugong)
{
	switch(ConvAbs2Rel(pos))
	{
	case WINDOW_STD:
		{
			pos -= MUGONG_STARTPOSITION;
			if(DeleteIcon(pos, (cIcon **)mugong))
			{
				if(pos == MUGONG_POSITION)
				{
					MUGONGBASE * mugongBase = (*mugong)->GetMugongBase();
					pWearedMugongInfo[0][MGI_NAME]->SetStaticText("");
					pWearedMugongInfo[0][MGI_ATTR]->SetStaticText("");
					pWearedMugongInfo[0][MGI_TYPE]->SetStaticText("");
					pWearedMugongInfo[0][MGI_KIND]->SetStaticText("");
					pWearedMugongInfo[0][MGI_SUNG]->SetStaticText("");
					pWearedMugongInfo[0][MGI_EXPPOINT]->SetStaticText("");
				}
				(*mugong)->SetWeared(0);
				return TRUE;
			}
			else
				return FALSE;
		}
		break;
	case WINDOW_EXTEND:
		{
			pos -= MUGONG1_2_STARTPOSITION;
			if(m_pMugongDlg1_2->DeleteIcon(pos, (cIcon **)mugong))
			{
				(*mugong)->SetWeared(0);
				return TRUE;
			}
			else
				return FALSE;
		}
		break;
	case WINDOW_DOWN:
		{
			pos -= MUGONG1_1_STARTPOSITION;
			if(m_pMugongDlg1_1->DeleteIcon(pos, (cIcon **)mugong))
			{
				(*mugong)->SetWeared(0);
				return TRUE;
			}
			else
				return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	
}
BOOL CMugongDialog::FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon)
{
	ASSERT(icon);

	if(icon->GetType() == WT_ITEM)
		FakeAddItem(mouseX, mouseY, (CItem *)icon);
	else if(icon->GetType() == WT_MUGONG)
		FakeMoveMugong(mouseX, mouseY, (CMugongBase *)icon);
	return FALSE;
}

void CMugongDialog::FakeAddItem(LONG mouseX, LONG mouseY, CItem * pItem)
{
	// 어떤 무공책이냐?무공, 내공, 경공
	if(pItem->GetItemKind() != eMUGONG_ITEM) return;
	BYTE type = GetItemKindDetail(pItem->GetItemIdx());
	switch(type)
	{
	case eMUGONGBOOK:
	case eNAEGONGBOOK:
	case eKYUNGGONGBOOK:
		break;
	default:
		return;
	}
	

	WORD ToPos=0;
	if(!GetPositionForXYRef(mouseX,mouseY, ToPos))		// 절대위치 넘어 옴
	{
		 if(!GetBlankPositionRef(type, ToPos))
			 return;
	}

	// 변환 가능한가?? 조건,제한 체크, 레벨, 직업, 성??
	if(!CanBeMoved(pItem, ToPos))  return ;


	MSG_MUGONG_ADD msg;
	msg.Category = MP_MUGONG;
	msg.Protocol = MP_MUGONG_ADD_SYN;
	msg.dwObjectID = HEROID;
	msg.FromTableIdx = eItemTable_Inventory;
	msg.ToTableIdx = eItemTable_Mugong;
	msg.FromAbsPosition = pItem->GetPosition();
	msg.ToAbsPosition = ToPos;

	NETWORK->Send(&msg,sizeof(msg));
	SetDisable(TRUE);
}
void CMugongDialog::FakeMoveMugong(LONG mouseX, LONG mouseY, CMugongBase * icon)
{
	WORD ToPos=0;
	if(!GetPositionForXYRef(mouseX,mouseY, ToPos))		// 절대위치 넘어 옴
			return;
	//if(!IsAddMugong(ToPos)) return;
	if(!CanBeMoved(icon, ToPos))  return ;

	// 무공,내공,경공
	CMugongBase * pMugong = (CMugongBase *)icon;
	//------------------------------------
	MSG_MUGONG_MOVEINFO msg;
	msg.Category = MP_MUGONG;
	msg.Protocol = MP_MUGONG_MOVE_SYN;
	msg.dwObjectID = HEROID;
	//------------------------------------
	msg.dwMugongDBIdx	= pMugong->GetDBIdx();
	msg.wToPosition		= ToPos;									//절대 위치로 back
	msg.wFromPosition	= pMugong->GetPosition();					//절대 위치로 back
	
	if(msg.wFromPosition == msg.wToPosition)
		return ;
	NETWORK->Send(&msg,sizeof(msg));
	SetDisable(TRUE);
}



void CMugongDialog::MoveMugong(POSTYPE ToPos/*abs pos*/, POSTYPE FromPos/*abs pos*/)
{
	CMugongBase * FromMugong = NULL;
	CMugongBase * ToMugong = NULL;
	DeleteMugong(FromPos, &FromMugong);
	DeleteMugong(ToPos, &ToMugong);

	if(FromMugong)
	{
		FromMugong->SetPosition(ToPos);
		AddMugong(ToPos, FromMugong);
	}
	
	if(ToMugong)
	{
		ToMugong->SetPosition(FromPos);
		AddMugong(FromPos, ToMugong);
	}
	
}

BOOL CMugongDialog::GetPositionForXYRef(LONG x, LONG y, POSTYPE& pos)
{
	WORD position=0;
	if(cIconDialog::GetPositionForXYRef(x,y,position))
	{
		// 기본(1) 무공위치
		pos = position+MUGONG_STARTPOSITION;								// 절대 위치로 바꿈
		return TRUE;
	}
	if(m_pMugongDlg1_2->IsActive() && m_pMugongDlg1_2->GetPositionForXYRef(x,y,position))
	{
		pos = position+MUGONG1_2_STARTPOSITION;								// 절대 위치로 바꿈
		return TRUE;
	}
	if(m_pMugongDlg1_2->IsActive() && m_pMugongDlg1_1->GetPositionForXYRef(x,y,position))
	{
		pos = position+MUGONG1_1_STARTPOSITION;								// 절대 위치로 바꿈
		return TRUE;
	}
	
	return FALSE;
}

void CMugongDialog::FakeDeleteMugong(POSTYPE absPos)
{
	MSG_MUGONG_REM_SYN msg;
	msg.Category = MP_MUGONG;
	msg.Protocol = MP_MUGONG_REM_SYN;
	msg.dwObjectID = HEROID;
	msg.wTableIdx = eItemTable_Mugong;
	msg.AbsPosition = absPos;

	NETWORK->Send(&msg,sizeof(msg));
	SetDisable(TRUE);
}



// 현재 수련중인 무공의 정보를 얻기 위해 : 임시
void CMugongDialog::GetMugong(POSTYPE relPos, CMugongBase * mugongOut)
{
	*mugongOut = *(CMugongBase * )GetIconForIdx(relPos);
}
CMugongBase * CMugongDialog::GetMugongAbs(POSTYPE abs_Pos)
{
	//*mugongOut = *(CMugongBase * )GetIconForIdx(relPos);
	switch(ConvAbs2Rel(abs_Pos))
	{
	case WINDOW_STD:
		{
			return (CMugongBase *)GetIconForIdx(abs_Pos-MUGONG_STARTPOSITION);
		}
		break;
	case WINDOW_EXTEND:
		{
			return (CMugongBase *)m_pMugongDlg1_2->GetIconForIdx(abs_Pos-MUGONG1_2_STARTPOSITION);
		}
		break;
	case WINDOW_DOWN:
		{
			return (CMugongBase *)m_pMugongDlg1_1->GetIconForIdx(abs_Pos-MUGONG1_1_STARTPOSITION);
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}
	return NULL;

}

void CMugongDialog::SetMugongSung(BYTE mugongType, WORD sung)
{
	ASSERT(mugongType < ALL);

	
	WORD relPos = 0;
	switch(mugongType)
	{
	case MUGONG:
		relPos = MUGONG_POSITION;
		break;
	case NAEGONG:
		relPos = NAEGONG_POSITION;
		break;
	case KYUNGGONG:
		relPos = KYUNGGONG_POSITION;
		break;
	default:
		{
			ASSERT(0);
		}
	}
	CMugongBase * mugongOut = NULL;
	mugongOut = (CMugongBase *)GetIconForIdx(relPos);
	if(mugongOut->GetDBIdx() > 0) 
		mugongOut->SetSung(sung);

	// 인터페이스
	// 무공 창의 성표시 
	// 무공 창의 경험치 max 게이지 setting
	pWearedMugongInfo[mugongType][MGI_SUNG]->SetStaticValue(sung);
}

void CMugongDialog::SetMugongExpPoint(BYTE mugongType, DWORD expPoint)
{
	ASSERT(mugongType < ALL);

	WORD relPos = 0;
	switch(mugongType)
	{
	case MUGONG:
		relPos = MUGONG_POSITION;
		break;
	case NAEGONG:
		relPos = NAEGONG_POSITION;
		break;
	case KYUNGGONG:
		relPos = KYUNGGONG_POSITION;
		break;
	default:
		{
			ASSERT(0);
		}
	}

	CMugongBase * mugongOut = NULL;
	mugongOut = (CMugongBase *)GetIconForIdx(relPos);
	if(mugongOut->GetDBIdx() > 0) 
		mugongOut->SetExpPoint(expPoint);

	//YH2DO
	//common_mugong * pInfo = GAMERESRCMNGR->GetMugongInfo(mugongOut->GetMugongIdx());
	char buf[20];
	wsprintf(buf, "%d/%d", expPoint, 0);//pInfo->experiance_numeric[mugongOut->GetSung()-1]);

	// 인터페이스
	// 무공 창의 경험치 게이지 증가
	pWearedMugongInfo[mugongType][MGI_EXPPOINT]->SetStaticText(buf);
}

BOOL CMugongDialog::CanBeMoved(cIcon * pIcon, POSTYPE abs_pos)
{
	ASSERT(pIcon);
	if(MUGONG_STARTPOSITION > abs_pos && abs_pos >= MUGONG_ENDPOSITION) return FALSE;

	switch(pIcon->GetType())
	{
	case WT_MUGONG:
		{
			// 무공 이동 : 유효한 위치인지 체크
			CMugongBase * pMugong = (CMugongBase *)pIcon;

		}
		break;
	case WT_ITEM:
		{
			// 무공 등록
			CItem * pItem = (CItem *)pIcon;
			return ITEMMGR->CanConvert(pItem->GetItemIdx());
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}

	return FALSE;
}

#endif