#ifndef _SHARESTRUCT_H
#define _SHARESTRUCT_H


//struct CHANNEL_LIST_INFO

/*
struct CHANNEL_INFO
{
	int		nChannelIndex;
	char	strChannelName[MAX_MNCHANNEL_NAME];
	int		nTotalPlayer;
	BYTE	cbOffset;		//sizeof(CHANNEL_INFO)
};
*/

struct CHANNEL_CREATE_INFO
{
	DWORD	dwChannelIndex;
	char	strChannelTitle[MAX_CHANNELTITLE_LENGTH];
	char	cbChannelKind;	//Channel Kind --- is public, is private, is for munpa...
	WORD	wMaxPlayer;
};

struct PLAYROOM_CREATE_INFO
{
	DWORD		dwPlayRoomIndex;
	char		strPlayRoomTitle[MAX_PLAYROOMTITLE_LENGTH];
	char		cbPlayRoomKind;
	WORD		wMaxObserver;
	WORD		wMaxPlayerPerTeam;
	MONEYTYPE	MoneyForPlay;
	MAPTYPE		wMapNum;
};


enum eCHANNEL_KIND
{
	eCHNK_UNKNOWN,
	eCHNK_PUBLIC,
	eCHNK_PRIVATE,
	eCHNK_MUNPA,
	eCHNK_COUNT,
};	



//struct MNPLAYER_GENERAL_INFO

//struct MNPLAYER_DETAIL_INFO


struct PLAYROOM_INFO
{
	DWORD	dwPlayRoomIndex;
	char	strPlayRoomTitle[MAX_PLAYROOMTITLE_LENGTH];
	int		nPlayRoomKind;

	DWORD	dwCurPlayerNum;
	DWORD	dwMaxPlayerNum;
	DWORD	dwMaxPlayerPerTeam;
	
	char	bStart;
};


//struct MNPLAYER_INFO_INPLAYROOM		//---include team num

//character



//////////
enum
{
	eLOCATION_NULL,
	eLOCATION_CHANNEL,
	eLOCATION_PLAYROOM,
	eLOCATION_GAME,
	eLOCATION_RESULT,
};

struct PLAYER_LOCATION
{
	int		nKind;
	DWORD	dwIndex;
};
//////////



#endif