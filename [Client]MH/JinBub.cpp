// JinBub.cpp: implementation of the CJinBub class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JinBub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJinBub::CJinBub()
{

}

CJinBub::~CJinBub()
{

}

void CJinBub::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cIcon::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_JINBUB;
}