#include "stdafx.h"


#include <map>
#include <string>
#include "MHFile.h"
using namespace std;

BOOL g_bAssertMsgBox = FALSE;
char _g_AssertErrorMessage[255];

#ifndef _MURIMNET_

#include <math.h>
#include "CommonGameFunc.h"
#ifdef _MHCLIENT_
#include "../ItemManager.h"
#include "../mhMap.h"
#include "../ChatManager.h"
#endif

#ifdef __MAPSERVER__
#ifndef _MURIMNET_
#include "itemManager.h"
#endif
#include "GameResourceManager.h"
#include "Object.h"

#endif

// 무공인덱스를 넘기면 무공의 무기유형을 리턴
// enum WP_KIND{GUM=0,GWUN,DO,CHANG,GUNG,AMGI,WP_MAX, WP_ERR};
/*
#define ATTACK_MUGONG_INTERVAL		200
#define DEFENSE_MUGONG_INTERVAL		100
#define ASSIST_MUGONG_INTERVAL		100
*/
DWORD DayOfMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
DWORD DayOfMonth_Yundal[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static char Days[7][16] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
static char WeatherState[][16] = { "CLEAR", "SNOW" };

#ifdef _MHCLIENT_

void AddComma( char* pBuf )
{
	if( *pBuf == 0 ) return;

	char buf[64];
//	strcpy( buf, pBuf );	//buf = 원본
	SafeStrCpy( buf, pBuf, 64 );
	char* p = buf;

	*(pBuf++) = *(p++);

	int n = strlen( pBuf ) % 3;

	while( *p )
	{
		if( n != 0 )
		{
			*pBuf = *p;
			++p;
		}
		else
		{
			*pBuf = ',';
		}

		++pBuf;
		n = ( n == 0 ) ? 3 : n - 1;
	}
	
	*pBuf = 0;
}

char* AddComma( DWORD dwMoney )
{
	static char buf[32];
	wsprintf( buf, "%u", dwMoney );
	AddComma( buf );
	return buf;
}

char* RemoveComma( char* str )
{
	static char buf[32];
	char* p = buf;

	while( *str )
	{
		if( *str != ',' )
		{
			*p = *str;
			++p;
		}

		++str;
	}

	*p = 0;

	return buf;
}

// magi82(37) 맵 속성 데이터 - 스크립트로 빠지면서 해당 함수는 주석 처리(더이상 안쓰임)
/*
char* GetMapName(MAPTYPE mapnum)
{
	ASSERT(mapnum != 0);

	static char mapname[256];
#define MAPNAMEDEF(num,Name)	case num:	strcpy(mapname, Name );	break;

	// RaMa - 44번은 이미 있는데 수련장에서 사용.. 여기에서 처리한후 리턴해버리자.
	if( mapnum == 44 )
	{
		strcpy( mapname, CHATMGR->GetChatMsg(922) );
		return mapname;
	}

	switch(mapnum)
	{
	//case hb001m:	strcpy(mapname,"북경");		break;
	MAPNAMEDEF(bukkyung, 	CHATMGR->GetChatMsg(481));
	MAPNAMEDEF(taesan,		CHATMGR->GetChatMsg(482));
	MAPNAMEDEF(namdae,		CHATMGR->GetChatMsg(483));
	MAPNAMEDEF(sanhea,		CHATMGR->GetChatMsg(484));
	MAPNAMEDEF(nakyang_siege,		CHATMGR->GetChatMsg(485));
	MAPNAMEDEF(nakyang,		CHATMGR->GetChatMsg(486));
	MAPNAMEDEF(gaebong,		CHATMGR->GetChatMsg(487));
	MAPNAMEDEF(jungju,		CHATMGR->GetChatMsg(488));
	MAPNAMEDEF(hoangsan,	CHATMGR->GetChatMsg(489));
	MAPNAMEDEF(sungsan,		CHATMGR->GetChatMsg(490));		
	MAPNAMEDEF(youngmun,	CHATMGR->GetChatMsg(491));
	MAPNAMEDEF(jangan,		CHATMGR->GetChatMsg(492));
	MAPNAMEDEF(hoajung,		CHATMGR->GetChatMsg(493));
	MAPNAMEDEF(malijangsung,		CHATMGR->GetChatMsg(494));
	MAPNAMEDEF(jisi,		CHATMGR->GetChatMsg(495));		
	MAPNAMEDEF(gaebong_siege,		CHATMGR->GetChatMsg(496));
	MAPNAMEDEF(nanju,		CHATMGR->GetChatMsg(497));
	MAPNAMEDEF(donhoang_night,	CHATMGR->GetChatMsg(498));
	MAPNAMEDEF(okmun,		CHATMGR->GetChatMsg(499));
	MAPNAMEDEF(myungsa,		CHATMGR->GetChatMsg(500));
	MAPNAMEDEF(hyungju,		CHATMGR->GetChatMsg(501));
	MAPNAMEDEF(makgogul2F,	CHATMGR->GetChatMsg(502));
	MAPNAMEDEF(jangsa,		CHATMGR->GetChatMsg(503));
	MAPNAMEDEF(bukkyung_siege,		CHATMGR->GetChatMsg(504));
	MAPNAMEDEF(dongjung,	CHATMGR->GetChatMsg(505));
	MAPNAMEDEF(akyang,		CHATMGR->GetChatMsg(506));
	MAPNAMEDEF(hangju,		CHATMGR->GetChatMsg(507));
	MAPNAMEDEF(Tournament,	CHATMGR->GetChatMsg(508));
	MAPNAMEDEF(sangju,		CHATMGR->GetChatMsg(509));
	MAPNAMEDEF(hogu,		CHATMGR->GetChatMsg(510));
	MAPNAMEDEF(bota,		CHATMGR->GetChatMsg(511));
	MAPNAMEDEF(hopo,		CHATMGR->GetChatMsg(512));
	MAPNAMEDEF(PKEVENTMAP,		CHATMGR->GetChatMsg(513));
	MAPNAMEDEF(ami,			CHATMGR->GetChatMsg(514));
	MAPNAMEDEF(hoangha,		CHATMGR->GetChatMsg(515));
	MAPNAMEDEF(sunyung,		CHATMGR->GetChatMsg(516));
	MAPNAMEDEF(guul,		CHATMGR->GetChatMsg(517));
	MAPNAMEDEF(hanbing,		CHATMGR->GetChatMsg(518));
	MAPNAMEDEF(gonryun,		CHATMGR->GetChatMsg(519));
	MAPNAMEDEF(tabi,		CHATMGR->GetChatMsg(520));
	MAPNAMEDEF(hohwa,		CHATMGR->GetChatMsg(521));
//	MAPNAMEDEF(ordos,		CHATMGR->GetChatMsg(522));
	MAPNAMEDEF(SURVIVALMAP, CHATMGR->GetChatMsg(1475));
	MAPNAMEDEF(daehung,		CHATMGR->GetChatMsg(523));
	MAPNAMEDEF(kan,			CHATMGR->GetChatMsg(524));
	MAPNAMEDEF(uru,			CHATMGR->GetChatMsg(525));
	MAPNAMEDEF(hwayuam,		CHATMGR->GetChatMsg(526));
	MAPNAMEDEF(bokju,		CHATMGR->GetChatMsg(527));
	MAPNAMEDEF(dwang,		CHATMGR->GetChatMsg(528));
	MAPNAMEDEF(dery,		CHATMGR->GetChatMsg(529));
	MAPNAMEDEF(samtop,		CHATMGR->GetChatMsg(530));
	MAPNAMEDEF(aesan,		CHATMGR->GetChatMsg(531));
	MAPNAMEDEF(sussang,		CHATMGR->GetChatMsg(532));
	MAPNAMEDEF(whangkua,	CHATMGR->GetChatMsg(533));
	MAPNAMEDEF(gooknae,		CHATMGR->GetChatMsg(534));
	MAPNAMEDEF(jangbeak,	CHATMGR->GetChatMsg(535));
	MAPNAMEDEF(janggun,		CHATMGR->GetChatMsg(536));
	MAPNAMEDEF(sangkyung,	CHATMGR->GetChatMsg(537));
	MAPNAMEDEF(dongkyung,	CHATMGR->GetChatMsg(538));
	MAPNAMEDEF(jisi_mid,	CHATMGR->GetChatMsg(539));
	MAPNAMEDEF(donhoang_day,	CHATMGR->GetChatMsg(622));
	MAPNAMEDEF(wallachun,		CHATMGR->GetChatMsg(670));
	MAPNAMEDEF(hwanghajubyun,	CHATMGR->GetChatMsg(685));
	MAPNAMEDEF(yongmunibgu,		CHATMGR->GetChatMsg(686));
	MAPNAMEDEF(hoangha2,		CHATMGR->GetChatMsg(404));
	MAPNAMEDEF(taesanjungsang,	CHATMGR->GetChatMsg(853));
	MAPNAMEDEF(taesannorth,		CHATMGR->GetChatMsg(924));
	MAPNAMEDEF(sorimsa,			CHATMGR->GetChatMsg(925));
	MAPNAMEDEF(BOSSMONSTER_MAP,	CHATMGR->GetChatMsg(884));
	MAPNAMEDEF(QuestRoom,		CHATMGR->GetChatMsg(997));
	MAPNAMEDEF(jangan_siege,	CHATMGR->GetChatMsg(1000));
	MAPNAMEDEF(BOSSMONSTER_2NDMAP,	CHATMGR->GetChatMsg(1445));	
	MAPNAMEDEF(RUNNINGMAP,	CHATMGR->GetChatMsg(1456));	
	MAPNAMEDEF(DIMENSION0,	CHATMGR->GetChatMsg(1638));
	MAPNAMEDEF(DIMENSION,	CHATMGR->GetChatMsg(1638));		
	MAPNAMEDEF(TITANMAP_EUNCHUN,	CHATMGR->GetChatMsg(1590));	
	MAPNAMEDEF(TITANMAP_TRAVOK,	CHATMGR->GetChatMsg(1591));
	MAPNAMEDEF(TITANMAP_GODDNESS,	CHATMGR->GetChatMsg(1593));
    MAPNAMEDEF(TITANMAP_RWAIN,	CHATMGR->GetChatMsg(1592));
	
	default: mapname[0] = 0; break;
	}

	return mapname;
}

WORD GetMapNumForName( char* pStrName )
{
	if( strcmp( pStrName, CHATMGR->GetChatMsg(481) ) == 0 )		return bukkyung;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(482) ) == 0 )		return taesan;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(483) ) == 0 )		return namdae;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(484) ) == 0 )		return sanhea;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(485) ) == 0 )		return nakyang_siege;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(486) ) == 0 )		return nakyang;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(487) ) == 0 )		return gaebong;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(488) ) == 0 )		return jungju;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(489) ) == 0 )		return hoangsan;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(490) ) == 0 )		return sungsan;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(491) ) == 0 )		return youngmun;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(492) ) == 0 )		return jangan;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(493) ) == 0 )		return hoajung;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(494) ) == 0 )		return malijangsung;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(495) ) == 0 )		return jisi;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(496) ) == 0 )		return gaebong_siege;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(497) ) == 0 )		return nanju;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(498) ) == 0 )		return donhoang_night;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(499) ) == 0 )		return okmun;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(500) ) == 0 )		return myungsa;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(501) ) == 0 )		return hyungju;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(502) ) == 0 )		return makgogul2F;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(503) ) == 0 )		return jangsa;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(504) ) == 0 )		return bukkyung_siege;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(505) ) == 0 )		return dongjung;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(506) ) == 0 )		return akyang;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(507) ) == 0 )		return hangju;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(508) ) == 0 )		return Tournament;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(509) ) == 0 )		return sangju;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(510) ) == 0 )		return hogu;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(511) ) == 0 )		return bota;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(512) ) == 0 )		return hopo;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(513) ) == 0 )		return PKEVENTMAP;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(514) ) == 0 )		return ami;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(515) ) == 0 )		return hoangha;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(516) ) == 0 )		return sunyung;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(517) ) == 0 )		return guul;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(518) ) == 0 )		return hanbing;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(519) ) == 0 )		return gonryun;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(520) ) == 0 )		return tabi;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(521) ) == 0 )		return hohwa;
//	if( strcmp( pStrName, CHATMGR->GetChatMsg(522) ) == 0 )		return ordos;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(523) ) == 0 )		return daehung;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(524) ) == 0 )		return kan;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(525) ) == 0 )		return uru;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(526) ) == 0 )		return hwayuam;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(527) ) == 0 )		return bokju;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(528) ) == 0 )		return dwang;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(529) ) == 0 )		return dery;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(530) ) == 0 )		return samtop;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(531) ) == 0 )		return aesan;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(532) ) == 0 )		return sussang;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(533) ) == 0 )		return whangkua;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(534) ) == 0 )		return gooknae;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(535) ) == 0 )		return jangbeak;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(536) ) == 0 )		return janggun;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(537) ) == 0 )		return sangkyung;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(538) ) == 0 )		return dongkyung;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(539) ) == 0 )		return jisi_mid;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(622) ) == 0 )		return donhoang_day;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(670) ) == 0 )		return wallachun;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(685) ) == 0 )		return hwanghajubyun;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(686) ) == 0 )		return yongmunibgu;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(404) ) == 0 )		return hoangha2;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(853) ) == 0 )		return taesanjungsang;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(924) ) == 0 )		return taesannorth;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(925) ) == 0 )		return sorimsa;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(884) ) == 0 )		return BOSSMONSTER_MAP;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(997) ) == 0 )		return QuestRoom;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1000) ) == 0 )	return jangan_siege;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1445) ) == 0 )	return BOSSMONSTER_2NDMAP;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1456) ) == 0 )	return RUNNINGMAP;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1475) ) == 0 )	return SURVIVALMAP;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1638) ) == 0 )	return DIMENSION;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1590) ) == 0 )	return TITANMAP_EUNCHUN;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1591) ) == 0 )	return TITANMAP_TRAVOK;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1592) ) == 0 )	return TITANMAP_RWAIN;
	if( strcmp( pStrName, CHATMGR->GetChatMsg(1593) ) == 0 )	return TITANMAP_GODDNESS;


	return 0;
}
*/
#else

// magi82(37) 맵 속성 데이터 - 스크립트로 빠지면서 해당 함수는 주석 처리(더이상 안쓰임)
/*
char* GetMapName(MAPTYPE mapnum)
{
	static char mapname[256];
#define MAPNAMEDEF(num,Name)	case num:	strcpy(mapname, #Name );	break;
	switch(mapnum)
	{
	//case hb001m:	strcpy(mapname,"북경");		break;
	MAPNAMEDEF(bukkyung, 	bukkyung);
	MAPNAMEDEF(taesan,		taesan);	
	MAPNAMEDEF(namdae,		namdae);	
	MAPNAMEDEF(sanhea,		sanhea);	
	MAPNAMEDEF(nakyang_siege,		nakyang_siege);
	MAPNAMEDEF(nakyang,		nakyang);	
	MAPNAMEDEF(gaebong,		gaebong);	
	MAPNAMEDEF(jungju,		jungju);	
	MAPNAMEDEF(hoangsan,	hoangsan);
	MAPNAMEDEF(sungsan,		sungsan);	
	MAPNAMEDEF(youngmun,	youngmun);
	MAPNAMEDEF(jangan,		jangan);	
	MAPNAMEDEF(hoajung,		hoajung);	
	MAPNAMEDEF(malijangsung,		malijangsung);	
	MAPNAMEDEF(jisi,		jisi);	
	MAPNAMEDEF(gaebong_siege,		gaebong_siege);
	MAPNAMEDEF(nanju,		nanju);	
	MAPNAMEDEF(donhoang_night,	donhoang_night);
	MAPNAMEDEF(okmun,		okmun);	
	MAPNAMEDEF(myungsa,		myungsa);
	MAPNAMEDEF(hyungju,		hyungju);
	MAPNAMEDEF(makgogul2F,	makgogul2F);
	MAPNAMEDEF(jangsa,		jangsa);	
	MAPNAMEDEF(bukkyung_siege,		bukkyung_siege);
	MAPNAMEDEF(dongjung,	dongjung);
	MAPNAMEDEF(akyang,		akyang);	
	MAPNAMEDEF(hangju,		hangju);	
	MAPNAMEDEF(Tournament,	Tournament);	
	MAPNAMEDEF(sangju,		sangju);
	MAPNAMEDEF(hogu,		hogu);	
	MAPNAMEDEF(bota,		bota);	
	MAPNAMEDEF(hopo,		hopo);	
	MAPNAMEDEF(PKEVENTMAP,		PKEVENTMAP);
	MAPNAMEDEF(ami,			ami);		
	MAPNAMEDEF(hoangha,		hoangha);
	MAPNAMEDEF(sunyung,		sunyung);
	MAPNAMEDEF(guul,		guul);	
	MAPNAMEDEF(hanbing,		hanbing);
	MAPNAMEDEF(gonryun,		gonryun);	
	MAPNAMEDEF(tabi,		tabi);	
	MAPNAMEDEF(hohwa,		hohwa);
//	MAPNAMEDEF(ordos,		ordos);	
	MAPNAMEDEF(daehung,		daehung);
	MAPNAMEDEF(kan,			kan);		
	MAPNAMEDEF(uru,			uru);		
	MAPNAMEDEF(hwayuam,		hwayuam);
	MAPNAMEDEF(bokju,		bokju);	
	MAPNAMEDEF(dwang,		dwang);	
	MAPNAMEDEF(dery,		dery);	
	MAPNAMEDEF(samtop,		samtop);
	MAPNAMEDEF(aesan,		aesan);	
	MAPNAMEDEF(sussang,		sussang);
	MAPNAMEDEF(whangkua,	whangkua);
	MAPNAMEDEF(gooknae,		gooknae);	
	MAPNAMEDEF(jangbeak,	jangbeak);
	MAPNAMEDEF(janggun,		janggun);	
	MAPNAMEDEF(sangkyung,	sangkyung);
	MAPNAMEDEF(dongkyung,	dongkyung);
	MAPNAMEDEF(jisi_mid,	jisi_mid);
	MAPNAMEDEF(wallachun,	wallachun);
	MAPNAMEDEF(donhoang_day,	donhoang_day);
	MAPNAMEDEF(yongmunibgu,	yongmunibgu);
	MAPNAMEDEF(hwanghajubyun,	hwanghajubyun);
	MAPNAMEDEF(hoangha2,	hoangha2);
	MAPNAMEDEF(taesanjungsang,	taesanjungsang);
	MAPNAMEDEF(taesannorth,	taesannorth);
	MAPNAMEDEF(sorimsa,	sorimsa);
	MAPNAMEDEF(BOSSMONSTER_MAP,	BOSSMONSTER_MAP);
	MAPNAMEDEF(QuestRoom,	QuestRoom);
	MAPNAMEDEF(jangan_siege,	jangan_siege);
	MAPNAMEDEF(BOSSMONSTER_2NDMAP,	BOSSMONSTER_2NDMAP);
	MAPNAMEDEF(RUNNINGMAP,	RUNNINGMAP);
	MAPNAMEDEF(SURVIVALMAP,	SURVIVALMAP);
	MAPNAMEDEF(DIMENSION0,	DIMENSION0);
	MAPNAMEDEF(DIMENSION,	DIMENSION);	
	MAPNAMEDEF(TITANMAP_EUNCHUN,	TITANMAP_EUNCHUN);
	MAPNAMEDEF(TITANMAP_TRAVOK,	TITANMAP_TRAVOK);
	MAPNAMEDEF(TITANMAP_GODDNESS,	TITANMAP_GODDNESS);
	MAPNAMEDEF(TITANMAP_RWAIN,	TITANMAP_RWAIN);

	default:jisi_mid,	mapname[0] = 0;
	}

	return mapname;
}
*/
#endif

void SetProtocol(MSGBASE* pMsg,BYTE bCategory,BYTE bProtocol)
{
	pMsg->Category = bCategory;
	pMsg->Protocol = bProtocol;	
}


WP_KIND GetWeaponType(WORD mugongIdx)
{
//	if(MIN_MUGONG_INDEX > mugongIdx || mugongIdx >= MAX_MUGONG_INDEX) return WP_ERR;

	if(MIN_ATTACK_MUGONG_INDEX <= mugongIdx && mugongIdx < MAX_ATTACK_MUGONG_INDEX)
	{
		
		return (WP_KIND)((mugongIdx - MIN_ATTACK_MUGONG_INDEX)/ATTACK_MUGONG_INTERVAL);
	}
	else if(MIN_DEFENSE_MUGONG_INDEX <= mugongIdx && mugongIdx < MAX_DEFENSE_MUGONG_INDEX)
	{
		return (WP_KIND)((mugongIdx - MIN_DEFENSE_MUGONG_INDEX)/DEFENSE_MUGONG_INTERVAL);
	}
	else if(MIN_ASSIST_MUGONG_INDEX <= mugongIdx && mugongIdx < MAX_ASSIST_MUGONG_INDEX)
	{
		return (WP_KIND)((mugongIdx - MIN_ASSIST_MUGONG_INDEX)/ASSIST_MUGONG_INTERVAL);
	}

	return WP_ERR;
}


eITEMTABLE GetTableIdxPosition(WORD abs_pos)
{
	if(TP_INVENTORY_START <= abs_pos && abs_pos < TP_INVENTORY_END)
	{
		return eItemTable_Inventory;
	}
	else if(TP_WEAR_START <= abs_pos && abs_pos < TP_WEAR_END)
	{
		return eItemTable_Weared;
	}
	else if(TP_PYOGUK_START <= abs_pos && abs_pos < TP_PYOGUK_END)
	{
		return eItemTable_Pyoguk;
	}	
	/*else if(TP_STREETSTALL_START <= abs_pos && abs_pos < TP_STREETSTALL_END)
	{
		return eItemTable_StreetStall;
	}*/
	else if(TP_QUICK_START <= abs_pos && abs_pos < TP_QUICK_END)
	{
		return eItemTable_Quick;
	}
#ifdef _JAPAN_LOCAL_
	else if(TP_MUGONG_START <= abs_pos && abs_pos < TP_MUGONG_END)
	{
		return eItemTable_Mugong;
	}
#elif defined _HK_LOCAL_
	else if(TP_MUGONG_START <= abs_pos && abs_pos < TP_MUGONG_END)
	{
		return eItemTable_Mugong;
	}
#elif defined _TL_LOCAL_
	else if(TP_MUGONG_START <= abs_pos && abs_pos < TP_MUGONG_END)
	{
		return eItemTable_Mugong;
	}
#else
	else if(TP_MUGONG1_START <= abs_pos && abs_pos < TP_MUGONG2_END)
	{
		return eItemTable_Mugong;
	}
#endif // _JAPAN_LOCAL_
	else if(TP_JINBUB_START <= abs_pos && abs_pos < TP_JINBUB_END)
	{
		return eItemTable_Mugong;
	}
/*	else if(TP_MUNPAWAREHOUSE_START <= abs_pos && abs_pos < TP_MUNPAWAREHOUSE_END)
	{
		return eItemTable_MunpaWarehouse;
	}*/
	else if(TP_ABILITY_START <= abs_pos && abs_pos < TP_ABILITY_END)
	{
		return eItemTable_Ability;
	}
	else if(TP_SHOPINVEN_START <= abs_pos && abs_pos < TP_SHOPINVEN_END)
	{
		return eItemTable_ShopInven;
	}
	else if(TP_PETINVEN_START <= abs_pos && abs_pos < TP_PETINVEN_END)
	{
		return eItemTable_PetInven;
	}
	else if(TP_TITANWEAR_START <= abs_pos && abs_pos < TP_TITANWEAR_END)
	{
		return eItemTable_Titan;
	}
	// magi82 - Titan(070911) 타이탄 무공업데이트
	else if(TP_TITANMUGONG_START <= abs_pos && abs_pos < TP_TITANMUGONG_END)
	{
		return eItemTable_Mugong;
	}
	// magi82(26)
	else if(TP_TITANSHOPITEM_START <= abs_pos && abs_pos < TP_TITANSHOPITEM_END)
	{
		return eItemTable_TitanShopItem;
	}

	return eItemTable_Max;
}

eITEM_KINDBIT GetItemKind(WORD iconIdx)
{
#ifdef _MHCLIENT_
//	ASSERT( ITEMMGR->GetItemInfo(iconIdx) );	//KES 040503
	if( ITEMMGR->GetItemInfo(iconIdx) )
		return (eITEM_KINDBIT)ITEMMGR->GetItemInfo(iconIdx)->ItemKind;
#endif
#ifdef __MAPSERVER__
	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(iconIdx);
	if(pInfo)
		return (eITEM_KINDBIT)pInfo->ItemKind;
	else
	{
		return eKIND_ITEM_MAX;
	}
#endif

	return (eITEM_KINDBIT)0;
}

eQUICKICON_KIND GetIconKind(WORD wIdx)
{
	if(wIdx >= MIN_ABILITY_INDEX)
		return eABILITY_ICON;	
	if(wIdx >= MIN_YOUNGYAKITEM_INDEX)
		return eITEM_ICON;
	else
		return eMUGONG_ICON;
}

eMUGONGITEM_KIND GetItemKindDetail(WORD iconIdx)
{
	eITEM_KINDBIT kind = GetItemKind(iconIdx);
	if( (kind & 1024) == FALSE)
		return eERROR_BOOK;

	if(kind == 1038)
	{
		return eJINBUBBOOK;
	}
	if(kind == 1037)
	{
		return eSIMBUBBOOK;
	}
	return eMUGONGBOOK;
}

POSTYPE ConvAbsPos2MugongPos(POSTYPE abspos)
{
#ifdef _JAPAN_LOCAL_

	if(abspos < TP_MUGONG_START)
	{
		ASSERT(0);
		return 0;
	}
	if(abspos >= TP_JINBUB_END)
	{
		ASSERT(0);
		return 0;
	}
	
	return abspos - TP_MUGONG_START;


#elif defined _HK_LOCAL_
	if(abspos < TP_MUGONG_START)
	{
		ASSERT(0);
		return 0;
	}
	if(abspos >= TP_JINBUB_END)
	{
		ASSERT(0);
		return 0;
	}
	
	return abspos - TP_MUGONG_START;

#elif defined _TL_LOCAL_
	if(abspos < TP_MUGONG_START)
	{
		ASSERT(0);
		return 0;
	}
	if(abspos >= TP_JINBUB_END)
	{
		ASSERT(0);
		return 0;
	}
	
	return abspos - TP_MUGONG_START;


#else

	// magi82 - Titan(070910) 타이탄 무공업데이트
	if( abspos >= TP_MUGONG1_START && abspos < TP_TITANMUGONG_END )
		return abspos - TP_MUGONG1_START;
	/*
	else if( abspos >= TP_JINBUB_START && abspos < TP_JINBUB_END )
		return abspos - TP_JINBUB_START + SLOT_MUGONG_NUM*2;
	*/
	else
	{
		ASSERT(0);
		return 0;
	}

#endif
}


DWORD GetMainTargetID(MAINTARGET* pMainTarget)
{
	if(pMainTarget->MainTargetKind == MAINTARGET::MAINTARGETKIND_OBJECTID)
		return pMainTarget->dwMainTargetID;
	else
		return 0;
}

#ifdef _MAPSERVER_
#ifndef _MURIMNET_
#include "UserTable.h"
#include "CharMove.h"
#endif
#endif
#ifdef _MHCLIENT_
#include "ObjectManager.h"
#endif

BOOL GetMainTargetPos(MAINTARGET* pMainTarget,VECTOR3* pRtPos, CObject ** ppObject)
{	
	if(pMainTarget->MainTargetKind == MAINTARGET::MAINTARGETKIND_POS)
	{
		pMainTarget->cpTargetPos.Decompress(pRtPos);
		if(ppObject)
			*ppObject = NULL;
		return TRUE;
	}
	else
	{
		
#ifdef _MAPSERVER_
		CObject* pTarget;
		pTarget = g_pUserTable->FindUser(pMainTarget->dwMainTargetID);
		if(ppObject)
			*ppObject = pTarget;
		if(pTarget == NULL)
			return FALSE;
		*pRtPos = *CCharMove::GetPosition(pTarget);
#endif
#ifdef _MHCLIENT_
		CObject* pTarget;
		pTarget = OBJECTMGR->GetObject(pMainTarget->dwMainTargetID);
		if(ppObject)
			*ppObject = pTarget;
		if(pTarget == NULL)
			return FALSE;
		pTarget->GetPosition(pRtPos);
#endif
		return TRUE;
	}
}

void SetVector3(VECTOR3* vec,float x,float y,float z)
{
	vec->x = x;
	vec->y = y;
	vec->z = z;
}

#ifdef _MHCLIENT_
void VRand(VECTOR3* pResult,VECTOR3* pOriginPos,VECTOR3* pRandom)
{
	int temp;
	temp = (int)pRandom->x;
	pResult->x = temp ? pOriginPos->x + (rand() % (2*temp)) - temp : pOriginPos->x;
	temp = (int)pRandom->y;
	pResult->y = temp ? pOriginPos->y + (rand() % (2*temp)) - temp : pOriginPos->y;
	temp = (int)pRandom->z;
	pResult->z = temp ? pOriginPos->z + (rand() % (2*temp)) - temp : pOriginPos->z;
}

void TransToRelatedCoordinate(VECTOR3* pResult,VECTOR3* pOriginPos,float fAngleRadY)
{
	if(pResult == pOriginPos)
	{
		VECTOR3 tempOri = *pOriginPos;
		pOriginPos = &tempOri;
	}
	static BOOL bb = FALSE;
	if(bb == FALSE)
	{
		MATRIX4 my;
		SetRotationYMatrix(&my,-fAngleRadY);
		
		TransformVector3_VPTR2(pResult,pOriginPos,&my,1);
	}
	else
	{
		RotateVectorAxisY(pResult,pOriginPos,fAngleRadY);
	}
}

void RotateVectorAxisX(VECTOR3* pResult,VECTOR3* pOriginVector,float fAngleRadX)
{
	if(fAngleRadX  == 0)
	{
		*pResult = *pOriginVector;
	}
	else
	{
		MATRIX4 mx;
		SetRotationXMatrix(&mx,-fAngleRadX);
		
		TransformVector3_VPTR2(pResult,pOriginVector,&mx,1);
	}
}

void RotateVectorAxisZ(VECTOR3* pResult,VECTOR3* pOriginVector,float fAngleRadZ)
{
	if(fAngleRadZ  == 0)
	{
		*pResult = *pOriginVector;
	}
	else
	{
		pResult->x = pOriginVector->x*cosf(fAngleRadZ) - pOriginVector->y*sinf(fAngleRadZ);
		pResult->y = pOriginVector->x*sinf(fAngleRadZ) + pOriginVector->y*cosf(fAngleRadZ);
		pResult->z = pOriginVector->z;
	}
}

void RotateVector( VECTOR3* pResult, VECTOR3* pOriginVector, float x, float y, float z )
{
	RotateVectorAxisX( pResult, pOriginVector, x );
	RotateVectorAxisY( pResult, pResult, y );
	RotateVectorAxisZ( pResult, pResult, z );
}

void RotateVector( VECTOR3* pResult, VECTOR3* pOriginVector, VECTOR3 vRot )
{
	RotateVectorAxisX( pResult, pOriginVector, vRot.x );
	RotateVectorAxisY( pResult, pResult, vRot.y );
	RotateVectorAxisZ( pResult, pResult, vRot.z );
}

#endif


void RotateVectorAxisY(VECTOR3* pResult,VECTOR3* pOriginVector,float fAngleRadY)
{
	if(fAngleRadY == 0)
	{
		*pResult = *pOriginVector;
	}
	else
	{
		pResult->x = pOriginVector->x*cosf(fAngleRadY) - pOriginVector->z*sinf(fAngleRadY);
		pResult->y = pOriginVector->y;
		pResult->z = pOriginVector->x*sinf(fAngleRadY) + pOriginVector->z*cosf(fAngleRadY);
	}
}

void BeelinePoint(VECTOR3 * origin, VECTOR3 * dir, float dist, VECTOR3 * result)
{
/*
	float m = dir->x / dir->z;
	result->x = origin->x + dist*cosf(m);
	result->z = origin->z + dist*sinf(m);
*/
//KES
	float rrr = sqrt( dir->x*dir->x + dir->z*dir->z );
	result->x = origin->x + dir->x * dist / rrr;
	result->z = origin->z + dir->z * dist / rrr;
}
void AdjacentPointForOriginPoint(VECTOR3 * origin, VECTOR3 * target, float dist, VECTOR3 * result)
{
	float rrr = CalcDistanceXZ(origin,target);

	if(rrr < dist)
	{
		result->x = target->x;
		result->z = target->z;
		return;
	}

//KES
	result->x = origin->x + ( target->x - origin->x ) * dist / rrr;
	result->z = origin->z + ( target->z - origin->z ) * dist / rrr;

/*
	float m = (origin->x - target->x)/(origin->z - target->z);
	// taiyo 2004/5/18 : origin->x + , origin->z+
	result->x = origin->x + dist*cosf(m);
	result->z = origin->z + dist*sinf(m);
*/
}
float CalcDistanceXZ(VECTOR3* v1,VECTOR3* v2)
{
//	float x = v1->x - v2->x;
//	float z = v1->z - v2->z;
//	return sqrtf(x*x + z*z);
	return sqrtf( (v1->x - v2->x)*(v1->x - v2->x) + (v1->z - v2->z)*(v1->z - v2->z) );
}

BOOL IsSameDay(char* strDay,WORD Year,WORD Month,WORD Day)
{
	//ex) 2002.02.08
	char temp[64];
	sprintf(temp,"%d.%02d.%02d",Year,Month,Day);
	return strcmp(strDay,temp) == 0;
}



#endif	//_MURIMNET_


#ifdef _MHCLIENT_
extern DWORD gUserID;
void SendAssertMsg(char* pStrFileName,int Line,char* pMsg)
{
	//////////////////////////////////////////////////////////////////////////
	// 메세지 박스
	if(g_bAssertMsgBox)
	{
		wsprintf(_g_AssertErrorMessage, "ASSERT() file:%s,line:%d (%s) do you debug?", pStrFileName, Line, pMsg);
		if(IDYES == MessageBox(NULL, _g_AssertErrorMessage, NULL, MB_YESNO|MB_TOPMOST))
			__asm int 3;
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	char temp[256];
	if(pMsg)
		sprintf(temp,"%s\t%d\t%s",pStrFileName,Line,pMsg);
	else
		sprintf(temp,"%s\t%d",pStrFileName,Line);
		
	MSG_ASSERT msg;
	msg.SetMsg(gUserID,gHeroID,temp);
	NETWORK->Send(&msg,sizeof(msg));
}

#endif

void WriteAssertMsg(char* pStrFileName,int Line,char* pMsg)
{
	//////////////////////////////////////////////////////////////////////////
	// 메세지 박스
	if(g_bAssertMsgBox)
	{
		wsprintf(_g_AssertErrorMessage, "ASSERT() file:%s,line:%d (%s) do you debug?", pStrFileName, Line, pMsg);
		if(IDYES == MessageBox(NULL, _g_AssertErrorMessage, NULL, MB_YESNO|MB_TOPMOST))
			__asm int 3;
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	char temp[256];
	//SW Log 계정명 제거
	/*if(pMsg)
		sprintf(temp,"[%s] %s line:%d, msg:%s, ",g_szHeroIDName ,pStrFileName,Line,pMsg);
	else
		sprintf(temp,"[%s] %s line:%d, ",g_szHeroIDName ,pStrFileName,Line);*/

	if(pMsg)
		sprintf(temp,"%s line:%d, msg:%s, ",pStrFileName,Line,pMsg);
	else
		sprintf(temp,"%s line:%d, ",pStrFileName,Line);
	
	SYSTEMTIME time;
	char szFile[256] = {0,};
	GetLocalTime( &time );
#ifdef _MAPSERVER_
	sprintf( szFile, "./Log/Assert_%02d_%4d%02d%02d.txt", g_pServerSystem->GetMapNum(), time.wYear, time.wMonth, time.wDay );
#else
	sprintf( szFile, "./Log/Assert_%4d%02d%02d.txt", time.wYear, time.wMonth, time.wDay );
#endif

	//파일의 용량검사.

	static BOOL bWrite = FALSE;

	if( bWrite == FALSE )
	{


		

		DWORD dwFileSize;
		DWORD dwSizeLow;
		DWORD dwSizeHigh;
		HANDLE hFile;
		OFSTRUCT data;

		hFile = (HANDLE)::OpenFile(szFile,&data,OF_READ);

		if((HFILE)hFile == 0xffffffff)
		{

			::CloseHandle(hFile);

			//파일이 존재하지 않는다면

			//while(1)
			{
				FILE* fp = fopen(szFile,"a+");
				if(fp)
				{
					fprintf(fp,"%stime:%s\n",temp,GetCurTimeToString());

					fclose(fp);
				}

				//hFile = hFile = (HANDLE)::OpenFile(szFile,&data,OF_READ);
				//dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
				//dwFileSize = dwSizeHigh << 16 | dwSizeLow;
				//::CloseHandle(hFile);

			//	if(dwFileSize > 1000000) break;
			}


			return ;
		}

		dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
		dwFileSize = dwSizeHigh << 16 | dwSizeLow;
		::CloseHandle(hFile);


		if(dwFileSize <= 1000000) //1M 보다 작다면 기록
		{
			FILE* fp = fopen(szFile,"a+");
			if(fp)
			{
				fprintf(fp,"%stime:%s\n",temp,GetCurTimeToString());

				fclose(fp);
			}
		}
		else
		{
			bWrite = TRUE;
			
		}

		

	}

}

void CriticalAssertMsg(char* pStrFileName,int Line,char* pMsg)
{
	wsprintf(_g_AssertErrorMessage, "ASSERT() file:%s,line:%d (%s) do you debug?", pStrFileName, Line, pMsg);
	if(IDYES == MessageBox(NULL, _g_AssertErrorMessage, NULL, MB_YESNO|MB_TOPMOST))
		__asm int 3;
	return;
}


void SafeStrCpy( char* pDest, const char* pSrc, int nDestBufSize )
{
	strncpy( pDest, pSrc, nDestBufSize -1 );
	pDest[nDestBufSize -1] = 0;
}


void SafeStrCpyEmptyChange( char* pDest, const char* pSrc, int nDestBufSize )
{
	// Hard.. Hard.. Temp.. 난중에 유연하게 바꾸도록 하자.. ㅋㅋㅋ
	// char b, char a   b->a 로 인자로 받아서 바꾸도록..
	char tmp[256] = { 0, };
	strncpy(tmp, pSrc, nDestBufSize-1);

	for(int i=0; i<256; i++)
	{
		if(tmp[i] == '_')		pDest[i] = ' ';
		else					pDest[i] = tmp[i];		
	}

	pDest[nDestBufSize-1] = 0;
}

char* GetCurTimeToString()
{
	static char temp[256];
	SYSTEMTIME ti;
	GetLocalTime(&ti);
	sprintf(temp,"%d월/%d일 %d시%d분%d.%d초",		// 월/일 시:분:초
		ti.wMonth,ti.wDay,
		ti.wHour,ti.wMinute,ti.wSecond,ti.wMilliseconds);
	return temp;
}

#ifdef _MHCLIENT_
char* GetCurTimeToString(DWORD milliSec)
{
	DWORD dwCurTime = milliSec / 1000;
	DWORD year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;
	static CHAR strTime[256] = { 0, };
	CHAR strTemp[128] = { 0, };

	ZeroMemory(strTime, sizeof(strTime));

	year = dwCurTime / YEARTOSECOND;
	dwCurTime = dwCurTime % YEARTOSECOND;

	month = dwCurTime / MONTHTOSECOND;
	dwCurTime = dwCurTime % MONTHTOSECOND;

	day = dwCurTime / DAYTOSECOND;
	dwCurTime = dwCurTime % DAYTOSECOND;

	hour = dwCurTime / HOURTOSECOND;
	dwCurTime = dwCurTime % HOURTOSECOND;

	min = dwCurTime / MINUTETOSECOND;
	dwCurTime = dwCurTime % MINUTETOSECOND;

	sec = dwCurTime;

	if(year)
	{
		sprintf(strTemp, CHATMGR->GetChatMsg(1600), year);
		strcat(strTime, strTemp);
	}
	if(month)
	{
		if(year)	strcat(strTime, " ");

		sprintf(strTemp, CHATMGR->GetChatMsg(1601), month);
		strcat(strTime, strTemp);
	}
	if(day)
	{
		if(year||month)	strcat(strTime, " ");

		sprintf(strTemp, CHATMGR->GetChatMsg(1602), day);
		strcat(strTime, strTemp);
	}
	if(hour)
	{
		if(year||month||day)	strcat(strTime, " ");

		sprintf(strTemp, CHATMGR->GetChatMsg(1603), hour);
		strcat(strTime, strTemp);
	}
	if(min)
	{
		if(year||month||day||hour)	strcat(strTime, " ");

		sprintf(strTemp, CHATMGR->GetChatMsg(1604), min);
		strcat(strTime, strTemp);
	}
	if(sec)
	{
		if(year||month||day||hour||min)	strcat(strTime, " ");

		sprintf(strTemp, CHATMGR->GetChatMsg(1605), sec);
		strcat(strTime, strTemp);
	}

	return strTime;
}

char* GetCurTimeToString(stTIME* stTime)
{
	static CHAR strTime[256] = { 0, };
	CHAR strTemp[128] = { 0, };
	DWORD year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;

	ZeroMemory(strTime, sizeof(strTime));

	year = stTime->GetYear();
	month = stTime->GetMonth();
	day = stTime->GetDay();
	hour = stTime->GetHour();
	min = stTime->GetMinute();
	sec = stTime->GetSecond();

	if(year)
	{
		sprintf(strTemp, CHATMGR->GetChatMsg(1600), year);
		strcat(strTime, strTemp);
	}
	if(month)
	{
		if(year)	strcat(strTime, " ");

		sprintf(strTemp, CHATMGR->GetChatMsg(1601), month);
		strcat(strTime, strTemp);
	}
	if(day)
	{
		if(year||month)	strcat(strTime, " ");

		sprintf(strTemp, CHATMGR->GetChatMsg(1602), day);
		strcat(strTime, strTemp);
	}
	if(hour)
	{
		if(year||month||day)	strcat(strTime, " ");

		sprintf(strTemp, CHATMGR->GetChatMsg(1603), hour);
		strcat(strTime, strTemp);
	}
	if(min)
	{
		if(year||month||day||hour)	strcat(strTime, " ");

		sprintf(strTemp, CHATMGR->GetChatMsg(1604), min);
		strcat(strTime, strTemp);
	}
	if(sec)
	{
		if(year||month||day||hour||min)	strcat(strTime, " ");

		sprintf(strTemp, CHATMGR->GetChatMsg(1605), sec);
		strcat(strTime, strTemp);
	}

	return strTime;
}
#endif

DWORD GetCurTimeValue()
{
	char temp2[256];

	SYSTEMTIME ti;
	GetLocalTime(&ti);
	sprintf(temp2,"%d%02d%02d",ti.wYear,ti.wMonth,ti.wDay);
	DWORD v2 = atoi(temp2);

	return v2;
}

DWORD GetCurTime()
{
	// 시간계산
	SYSTEMTIME systime;
	GetLocalTime( &systime );
	stTIME time;
	time.SetTime( systime.wYear-2000, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond );

	return time.value;
}

DWORD GetCurTimeToSecond()
{
	stTIME curtime;
	curtime.value = GetCurTime();

	DWORD timetosecond = 0;
	timetosecond += curtime.GetYear() * YEARTOSECOND;
	timetosecond += curtime.GetMonth() * MONTHTOSECOND;
	timetosecond += curtime.GetDay() * DAYTOSECOND;
	timetosecond += curtime.GetHour() * HOURTOSECOND;
	timetosecond += curtime.GetMinute() * MINUTETOSECOND;
	timetosecond += curtime.GetSecond();

	return timetosecond;
}

void StringtimeToSTTIME(stTIME* Time, char* strTime)
{
 	char year[3]		= { 0, };
 	char month[3]		= { 0, };
 	char day[3]			= { 0, };
 	char hour[3]		= { 0, };
 	char minute[3]		= { 0, };

	strncpy(year, &strTime[2], 2);
 	strncpy(month, &strTime[5], 2);
 	strncpy(day, &strTime[8], 2);
 	strncpy(hour, &strTime[11], 2);
 	strncpy(minute, &strTime[14],2);
 	Time->SetTime( atoi(year), atoi(month), atoi(day), atoi(hour), atoi(minute), 0 );
}

BOOL CheckValidPosition(VECTOR3& pos)
{
	if(pos.x < 0 || pos.z < 0)
		return FALSE;
	if(pos.x >= 51200 || pos.z >= 51200)
		return FALSE;
	return TRUE;
}


float roughGetLength( float fDistX,float fDistY )
{
	if( fDistX < 0.0f ) fDistX = -fDistX;
	if( fDistY < 0.0f ) fDistY = -fDistY;
	
	int min = (int)(( fDistX > fDistY ) ? fDistY : fDistX);

	return ( fDistX + fDistY - ( min >> 1 ) - ( min >> 2 ) + ( min >> 4 ) );
}

//////////////////////////////////////////////////////////////////////////
// Effect번호를 파일이름으로 찾기 위한 부분
map<string,DWORD> g_fileTable;

void LoadEffectFileTable(char* pListFileName)
{
#ifdef _MHCLIENT_
	CMHFile file;
	file.Init(pListFileName,"rb",MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT);

	DWORD MaxEffectDesc = file.GetDword();
	
	DWORD effnum;
	char efffile[256];
	for(DWORD n=0;n<MaxEffectDesc;++n)
	{
		//ASSERT(!pFile->IsEOF());

		effnum = file.GetDword();
		file.GetString(efffile);
		strupr(efffile);
		if(strcmp(efffile,"NULL") == 0)
			continue;
		
		g_fileTable.insert(map<string,DWORD>::value_type(efffile,effnum));
	}	

	file.Release();
#endif
}

int FindEffectNum(char* pFileName)
{
#ifdef _MHCLIENT_
	if(pFileName[0] == '0' && pFileName[1] == 0)
		return 0;

	static char tempbuf[64];
	strcpy(tempbuf,pFileName);
	map<string,DWORD>::iterator iter;
	iter = g_fileTable.find(strupr(tempbuf));
	if(iter == g_fileTable.end())
	{
//		ASSERTMSG(0,pFileName);
		return -1;
	}
	else
		return iter->second;
#else
	// 06. 04. 스킬 버그 수정 - 이영준
    // 맵서버에서는 이펙트가 있는지 없는지만 가지고 있는다
	if(pFileName[0] == '0' && pFileName[1] == 0)
		return 0;
	else
		return 1;
#endif
}

void ERRORBSGBOX(char* str,...)
{
	char msg2[255];
	va_list argList;

	va_start(argList, str);
	vsprintf(msg2,str,argList);
	va_end(argList);

	MessageBox(NULL,msg2,NULL,NULL);
	
}

// Weather System
const char* GetDay(WORD nDay)
{
	return Days[nDay];
}

WORD GetDay(const char* strDay)
{
	for(WORD i = 0; i < 7; i++)
	{
		if(strcmp(strDay, Days[i]) == 0)
			return i;
	}

	return -1;
}

const char* GetWeather(WORD nWeatherState)
{
	return WeatherState[nWeatherState];
}

WORD GetWeather(const char* strWeather)
{
	for(WORD i = 0; i < eWS_Max; i++)
	{
		if(strcmp(strWeather, WeatherState[i]) == 0)
			return i;
	}

	return -1;
}

DWORD Convert2MilliSecond( DWORD Day, DWORD Hour, DWORD Minute, DWORD Second )
{
	DWORD dwValue = 0;

	dwValue += Day * DAYTOSECOND * SECTOMILLISEC;
	dwValue += Hour * HOURTOSECOND * SECTOMILLISEC;
	dwValue += Minute * MINUTETOSECOND * SECTOMILLISEC;
	dwValue += Second * SECTOMILLISEC;

	return dwValue;
}

//2007. 10. 25. CBH - 오브젝트 종류가 어떤 그룹인지 반환하는 함수
eObjectKindGroup GetObjectKindGroup(WORD wObjectKind)
{
	if( (wObjectKind == eObjectKind_Mining) || (wObjectKind == eObjectKind_Collection) || (wObjectKind == eObjectKind_Hunt) )
	{
		return eOBJECTKINDGROUP_JOB;
	}

	return eOBJECTKINDGROUP_NONE;
}

float GetAlphaValue(DWORD dwTime, DWORD dwStartTime, BOOL bFlag)
{
	DWORD ToTime = dwTime;
	DWORD ElTime = gCurTime - dwStartTime;
	float alp = 0.0f;

	if(bFlag)	// 데코레이션 생성
	{
		alp = ElTime / (float)ToTime + 0.01f;
	}
	else	// 데코레이션 해제
	{
		alp = 1 - (ElTime / (float)ToTime);
	}

	return alp;
}

void WriteDebugFile( char* pStrFileName, int Line, char* pMsg )
{
	char temp[512] = {0,};
	if(pMsg)
        sprintf(temp,"%s line:%d, msg:%s, ", pStrFileName, Line, pMsg);
	else
		sprintf(temp,"%s line:%d, ", pStrFileName, Line);

	SYSTEMTIME time;
	char szFile[256] = {0,};
	GetLocalTime( &time );
	sprintf( szFile, "./Log/Debug_%4d%02d%02d.txt", time.wYear, time.wMonth, time.wDay );

	//파일의 용량검사.
	static BOOL bWrite = FALSE;
	if( bWrite == FALSE )
	{
		DWORD dwFileSize;
		DWORD dwSizeLow;
		DWORD dwSizeHigh;
		HANDLE hFile;
		OFSTRUCT data;

		hFile = (HANDLE)::OpenFile(szFile,&data,OF_READ);

		if((HFILE)hFile == 0xffffffff)
		{
			::CloseHandle(hFile);

			//파일이 존재하지 않는다면
			FILE* fp = fopen(szFile,"a+");
            if(fp)
			{
				fprintf(fp,"%stime:%s\n",temp,GetCurTimeToString());
				fclose(fp);
			}
			return ;
		}
		dwSizeLow = GetFileSize(hFile, &dwSizeHigh);
		dwFileSize = dwSizeHigh << 16 | dwSizeLow;
		::CloseHandle(hFile);

		if(dwFileSize <= 1000000) //1M 보다 작다면 기록
		{
			FILE* fp = fopen(szFile,"a+");
			if(fp)
			{
				fprintf(fp,"%stime:%s\n",temp,GetCurTimeToString());

				fclose(fp);
			}
		}
		else
		{
			bWrite = TRUE;
		}
	}
}
