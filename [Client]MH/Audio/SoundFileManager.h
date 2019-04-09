// SoundFileManager.h: interface for the CSoundFileManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDFILEMANAGER_H__05C8D5EA_6C2B_4D3A_B70D_02A42F8857B8__INCLUDED_)
#define AFX_SOUNDFILEMANAGER_H__05C8D5EA_6C2B_4D3A_B70D_02A42F8857B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSoundItem;

class CSoundFileManager  
{
public:
	CSoundFileManager();
	virtual ~CSoundFileManager();

	void			Init( CSoundItem	** ppSoudItemList, __int32 & nMaxSound );
	void			Release();
	
protected:
};

#endif // !defined(AFX_SOUNDFILEMANAGER_H__05C8D5EA_6C2B_4D3A_B70D_02A42F8857B8__INCLUDED_)
