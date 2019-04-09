// GuildMarkManager.h: interface for the CGuildMarkManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDMARKMANAGER_H__5F7B3564_F660_4AB8_91B1_023A697C6DFE__INCLUDED_)
#define AFX_GUILDMARKMANAGER_H__5F7B3564_F660_4AB8_91B1_023A697C6DFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define GUILDMARKMGR CGuildMarkManager::GetInstance()

class CGuildMark;
class CGuildMarkImg;

class CGuildMarkManager  
{
	CYHHashTable<CGuildMark> m_MarkTable;

	CGuildMarkImg* m_pLoadingImg;

	CGuildMarkImg* LoadMarkImg(MARKNAMETYPE markname);	
	void SendGuildMarkImgRequest(MARKNAMETYPE MarkName);
	void SendGuildMarkImgUpdate(DWORD GuildIdx,char* pImgData);
	BOOL ReadBitInfoFromFile(char* filename,char* imgData);
	
	BOOL LoadMarkInfo();
public:
	MAKESINGLETON(CGuildMarkManager);
	CGuildMarkManager();
	virtual ~CGuildMarkManager();

	BOOL Init();
	void Release();

	CGuildMark* GetGuildMark(MARKNAMETYPE markname);

	void OnRecvGuildMarkImg(MSG_GUILDMARK_IMG* pmsg);

	BOOL ChangeGuildMarkSyn(DWORD GuildIdx,char* filename);
	void UpdateGuildMarkImg(DWORD GuildIdx,MARKNAMETYPE markname,char* pImgData);

	CGuildMark* LoadGuildMarkImg( MARKNAMETYPE markname );
};
#endif // !defined(AFX_GUILDMARKMANAGER_H__5F7B3564_F660_4AB8_91B1_023A697C6DFE__INCLUDED_)
