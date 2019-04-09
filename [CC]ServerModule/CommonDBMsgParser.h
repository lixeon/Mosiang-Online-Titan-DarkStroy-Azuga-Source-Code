
#ifndef __COMMONDBMSGPARSER_H__
#define __COMMONDBMSGPARSER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "db.h"
 

//-----------------------------------------------------------------------
// DBÄõ¸® ¸®ÅÏ ÇÔ¼ö°¡ ´Ã¾î³¯¶§¸¶´Ù Ãß°¡
// °è¼Ó Ãß°¡µÇ´Â ºÎºÐ


//---------------------------------------------------------------------------
// ÇÔ¼ö ÇÁ·ÎÅä Å¸ÀÔ
// °è¼Ó Ãß°¡µÇ´Â ºÎºÐ
//---------------------------------------------------------------------------
// Query function
// ÇÔ¼ö¸¸µéÁö ¾Ê°í ¹Ù·Î »ç¿ëÇÏ´Â °Ô ??

// Query Result function
void	ASSERTQuery(LPQUERY pData, LPDBMESSAGE pMessage);







//-------------------------------------------------------------------------
// ±¸Á¶Ã¼ Á¤ÀÇ ºÎºÐ 
// º¯°æµÇÁö ¾Ê´Â´Ù.
//-------------------------------------------------------------------------
typedef	void (*DBMsgFunc)(LPQUERY pData, LPDBMESSAGE pMessage);
typedef	void (*DBMiddleMsgFunc)(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
typedef	void (*DBLargeMsgFunc)(LPLARGEQUERY pData, LPDBMESSAGE pMessage);

extern DBMsgFunc g_DBMsgFunc[];
//-------------------------------------------------------------------------





#endif //__COMMONDBMSGPARSER_H__