// SoundItem.h: interface for the CSoundItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDITEM_H__C67B651B_CE62_4E7E_843A_73229C8B956E__INCLUDED_)
#define AFX_SOUNDITEM_H__C67B651B_CE62_4E7E_843A_73229C8B956E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sound.h"
#include "SoundLib.h"

class CSoundItem  
{
	friend class CSound;
	SOUND_FILE_HANDLE m_hSoundFile;
	
public:
	CSoundItem();
	virtual ~CSoundItem();
	void		Init(SNDIDX idx, char * fileName, BOOL bLoop, BOOL bStreaming,
						float MinDist, float MaxDist, float fVolume);
	
	CSound* CreateSound(CObject* pOwnerObject);

	char* GetFileName()	{	return m_szFileName;	}
	
protected:
	SNDIDX		idx;
	WORD		type;
	StaticString	m_szFileName;
	BOOL		m_bLoop;
	BOOL		m_bStream;
	
	float		m_MinDist;
	float		m_MaxDist;
	float		m_fVolume;
};



#endif // !defined(AFX_SOUNDITEM_H__C67B651B_CE62_4E7E_843A_73229C8B956E__INCLUDED_)
