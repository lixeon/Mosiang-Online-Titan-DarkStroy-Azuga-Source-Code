// CPyoGukManager.cpp: implementation of the PyoGukManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PyoGukManager.h"
#include "GameIn.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "./interface/cWindowManager.h"
#include "WindowIdEnum.h"
#include "cDivideBox.h"



#include "PyogukDialog.h"
#include "CharacterDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CPyoGukManager);
CPyoGukManager::CPyoGukManager()
{
}

CPyoGukManager::~CPyoGukManager()
{

}

void CPyoGukManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol) 
	{
	case MP_PYOGUK_INFO:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			HERO->SetPyogukNum(pmsg->bData);
		}
		break;
	case MP_PYOGUK_BUY_ACK:
		{
			MSG_BYTE * pmsg = (MSG_BYTE*)pMsg;
			HERO->SetPyogukNum(pmsg->bData);
			
			if(pmsg->bData == 1)
			{
				GAMEIN->GetPyogukDialog()->SetItemInit(TRUE);
				GAMEIN->GetPyogukDialog()->SetPyogukMoney(0);
			}
		//	GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukWare1+pmsg->bData-1);
		//	GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukWare1+pmsg->bData-1);
					
			GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukListInfo);
			GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukListInfo);
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(152) );
		}
		break;
	case MP_PYOGUK_PUTIN_MONEY_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			GAMEIN->GetPyogukDialog()->SetPyogukMoney(pmsg->dwData);
		}
		break;
	case MP_PYOGUK_PUTOUT_MONEY_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			GAMEIN->GetPyogukDialog()->SetPyogukMoney(pmsg->dwData);
		}
		break;
		
		//////////////////////////////////////////////////////////////////////////
		//nack 
	case MP_PYOGUK_BUY_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(50) );
		}
		break;
	case MP_PYOGUK_PUTIN_MONEY_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(130) );
		}
		break;
	}
}

void CPyoGukManager::BuyPyogukSyn()
{
	int rt = GAMEIN->GetPyogukDialog()->CanBuyPyoguk();

	if( rt != 0 )
	{
		if( rt == 1 )	//µ·¾ø´Ù
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(50) );
		}
		else if( rt == 2 ) //Ã¢°í ²ËÃ¡´Ù
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(308) );
		}
		return;
	}

	MSGBASE msg;
	msg.Category = MP_PYOGUK;
	msg.Protocol = MP_PYOGUK_BUY_SYN;
	msg.dwObjectID = HEROID;
	
	NETWORK->Send(&msg, sizeof(msg));
}

void CPyoGukManager::DelPyogukSyn(BYTE num)
{
	MSG_BYTE msg;
	msg.Category = MP_PYOGUK;
	msg.Protocol = MP_PYOGUK_DEL_SYN;
	msg.dwObjectID = HEROID;
	msg.bData = num;

	NETWORK->Send(&msg, sizeof(msg));
}

void CPyoGukManager::PutInOutMoney(int vals)
{	
	CPyogukDialog* pPyogukDlg = GAMEIN->GetPyogukDialog();
	CInventoryExDialog* pInvenDlg = GAMEIN->GetInventoryDialog();

	cStatic* pPyogukMoney = (cStatic*)WINDOWMGR->GetWindowForIDEx(PYO_MONEY);

	if(vals)
	{
		cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_INPUT_MONEY, (LONG)(pPyogukMoney->GetAbsX()-50), (LONG)(pPyogukMoney->GetAbsY()-100), OnPutInMoneyPyogukOk, OnPutInMoneyPyogukCancel, pInvenDlg, pPyogukDlg, CHATMGR->GetChatMsg(230) );
	}
	else
	{
		cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_INPUT_MONEY, (LONG)(pPyogukMoney->GetAbsX()-50) , (LONG)(pPyogukMoney->GetAbsY()-100), OnPutOutMoneyPyogukOk, OnPutInMoneyPyogukCancel, pInvenDlg, pPyogukDlg, CHATMGR->GetChatMsg(231) );
	}
}


void CPyoGukManager::OnPutInMoneyPyogukOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	if(param1 == 0)	return;

	MSG_DWORD msg;
	msg.Category = MP_PYOGUK;
	msg.Protocol = MP_PYOGUK_PUTIN_MONEY_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = param1;
	
	NETWORK->Send(&msg, sizeof(msg));
}

void CPyoGukManager::OnPutInMoneyPyogukCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
}

void CPyoGukManager::PutInOutMoneyResult(MSG_DWORD3 * msg)
{
	if(HEROID == msg->dwData1)
	{
		HERO->SetMoney(msg->dwData2);
		GAMEIN->GetCharacterDialog()->UpdateData();
		
	}
//	GAMEIN->GetMunpaDialog()->SetMoney(msg->dwData3);
}

void CPyoGukManager::OnPutOutMoneyPyogukOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	if(param1 == 0)	return;

	MSG_DWORD msg;
	msg.Category = MP_PYOGUK;
	msg.Protocol = MP_PYOGUK_PUTOUT_MONEY_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = param1;
	
	NETWORK->Send(&msg, sizeof(msg));
}
