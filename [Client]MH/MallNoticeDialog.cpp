// MallNoticeDialog.cpp: implementation of the CMallNoticeDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MallNoticeDialog.h"
#include "WindowIDEnum.h"
#include <shellapi.h.>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMallNoticeDialog::CMallNoticeDialog()
{

}

CMallNoticeDialog::~CMallNoticeDialog()
{

}

void CMallNoticeDialog::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_DIALOG)
		AddTabSheet(curIdx2++, window);
	else 
		cTabDialog::Add(window);
}

void CMallNoticeDialog::OnActionEvent( LONG lId, void * p, DWORD we )
{
	if( we & WE_BTNCLICK )
	{
		if( lId == ITEM_MALLBTN )
		{	
//			ShellExecute( NULL, NULL, "Iexplore.exe", "http://mall.darkstoryonline.com/", NULL, SW_SHOWNORMAL );
#ifdef TAIWAN_LOCAL
			ShellExecute( NULL, NULL, "Iexplore.exe", "https://secure.tengwu.com.cn/ItemMall/web_product_main.asp", NULL, SW_SHOWNORMAL );
#elif defined _JAPAN_LOCAL_
#elif defined _HK_LOCAL_
#else
			ShellExecute( NULL, NULL, "Iexplore.exe", "http://www.wldhmx.com/webshop.aspx", NULL, SW_SHOWNORMAL );
#endif
		}
	}
}
