// FixedTile.h: interface for the CFixedTile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIXEDTILE_H__6066A67E_DA78_4AAE_BA9F_38CDBD8B9FB1__INCLUDED_)
#define AFX_FIXEDTILE_H__6066A67E_DA78_4AAE_BA9F_38CDBD8B9FB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define TFA_COLLISON		0x00000001
typedef DWORD FIXEDTILEATTR;

struct FIXEDTILE_ATTR
{
	FIXEDTILEATTR uFixedAttr : 8;			//  0 ~ 255
};

class CFixedTile  
{
	FIXEDTILE_ATTR m_FixedAttr;
public:
	CFixedTile();
	virtual ~CFixedTile();
	
	void InitTileAttrib(FIXEDTILE_ATTR Attr);
	inline BOOL IsCollisonTile()
	{
		return m_FixedAttr.uFixedAttr & TFA_COLLISON ;
	}
};

#endif // !defined(AFX_FIXEDTILE_H__6066A67E_DA78_4AAE_BA9F_38CDBD8B9FB1__INCLUDED_)
