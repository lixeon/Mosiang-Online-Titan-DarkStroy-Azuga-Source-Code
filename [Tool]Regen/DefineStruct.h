
/* -----------------------------------------------
	filename:	DefineStruct.h
	coder	:	Park Jung Hwan(ilil5@kornet.net) 
	comp.	:	eSofnet 
	compiler:	vc++ 6.0
	date	:	2003-05-02¿ÀÀü 11:24:55
----------------------------------------------- */

#ifndef __DefineStruct_h__
#define __DefineStruct_h__
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DyuchiGRX_common/typedef.h"

typedef WORD MAPTYPE;
typedef DWORD EXPTYPE;
typedef WORD LEVELTYPE;

#define TILE_MAX_WIDTH 1024
#define TILE_MAX_HEIGHT 1024
#define COLLISIONTILE_SIZE 50

#define MAX_MAP_NUM 37
#define MAX_MONSTER_NUM 2000
#define MAX_FUNCKIND_NUM 6
#define MAX_TILEKIND_NUM 5

#define GENDER_MALE 0
#define GENDER_FEMALE 1
#define GENDER_MAX 2

#define MAKESINGLETON(classname)	static classname* GetInstance()	{	static classname gInstance;		return &gInstance;	}
#define GETINSTANCE(ClassName) static ClassName * GetInstance(){ static ClassName g_Instance; return &g_Instance; }

#define SAFE_DELETE(a)			if((a))	{ delete (a); (a)=NULL; }
#define SAFE_DELETE_ARRAY(a)	if((a))	{ delete [] (a); (a)=NULL; }
#define SAFE_RELEASE(a)			if((a))	{ (a)->Release(); (a)=NULL; }

#define gPHI					3.141592f
//#define DEGTORAD(deg)			( deg / 180.f * gPHI )
//#define RADTODEG(rad)			( rad / gPHI * 180.f )

#define RGBA_MAKE(r, g, b, a)   ((DWORD) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))

extern DWORD gCurTime;
extern DWORD gTickTime;

#define SAFE_DELETE(a)			if((a))	{ delete (a); (a)=NULL; }
#define SAFE_DELETE_ARRAY(a)	if((a))	{ delete [] (a); (a)=NULL; }
#define SAFE_RELEASE(a)			if((a))	{ (a)->Release(); (a)=NULL; }

#define MAX_NAME_LENGTH			16
#define MAX_FILE_NAME		32
//------------------------------------
// ½ºÅ©·Ñ¹öÆ° °£°Ý¼±¾ð
//------------------------------------
#define LINE_SIZE	5
#define PAGE_SIZE	15

//------------------------------------
// Func_Kind ÄÞº¸ ÀÎÀÚµé¼±¾ð
//------------------------------------
#define FDEFAULT			"----Function Select----"
#define FMONSTER_REGEN	"Monster_Regen"
#define FNPC_REGEN		"Npc_Regen"
#define FLOGIN_POINT		"Login_Point"
#define FCHANGEMAP_POINT "ChangeMap_Point"
#define FMAP_ATTRIBUTE	"Map_Attribute"

//------------------------------------
// ¸®½ºÆ® ·Îµù½Ã »ç¿ë
//------------------------------------
#define CMD_ST(a)	char buffer[255]; strcpy(buffer, a); if(0){
#define CMD_CS(b)		}else if(strcmp(buffer, b) == 0){
#define CMD_EN			}


typedef BYTE AREATILE;

#ifndef GLOBAL_FUNC_DLL
#define GLOBAL_FUNC_DLL extern "C" __declspec(dllimport) 
#endif

GLOBAL_FUNC_DLL void	__stdcall VECTOR3_SUB_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1,VECTOR3* pv3Arg2);
GLOBAL_FUNC_DLL float	__stdcall VECTOR3Length(VECTOR3* pv3);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_ADD_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1,VECTOR3* pv3Arg2);

#define SKILLAREA_ATTR_DAMAGE			0x00000001
#define SKILLAREA_ATTR_BLOCK			0x00000002
#define SKILLAREA_ATTR_SAFETYZONE		0x00000003
#define SKILLAREA_ATTR_OTHEREFFECT		0x00000004

#define BASIC_MONSTER	32
#define BOSS_MONSTER	33
#define SPECIAL_MONSTER	34

extern float gTickPerFrame;

HRESULT typedef (__stdcall *CREATE_INSTANCE_FUNC)(void* ppv);

enum
{
	eDefault = 0,
	eMonster_Regen,
	eNpc_Regen,
	eLogin_Point,
	eChangeMap_Point,
	eMap_Attribute
};
//------------------------------------
// SubCombo Text Setting
//------------------------------------
#define TDEFAULT			"-------"
#define TMONSTER			"Monster"
#define TNPC				"Npc"
#define TLOGIN			"Login"
#define TCHANGEMAP		"ChangeMap"
#define TATTRIBUTE		"Attribute"
//------------------------------------
// ¹öÆ° Flagº¯¼ö ¼±¾ð
//------------------------------------
enum
{
	eZOOMOUT = 0,
	eZOOMIN,
	eSELECT,
	eMSELECTNOAREA,
	eMSELECTAREA,
	eMOVE,
	eDELETE,
	MAX_BUTTON,
};
//------------------------------------
// ¸Ê ¸®½ºÆ® Á¤º¸
//------------------------------------
typedef struct _MAP_LIST
{
	WORD MapKind;
	char FieldName[20];
	char LocalName[20];
	char PositionName[20];
	char FileName[20];
}MAP_LIST;

//-----------------------------------------------------------------------
// ¸ó½ºÅÍ ¸®½ºÆ® Á¤º¸
//-----------------------------------------------------------------------
typedef struct _MONSTER_LIST
{
	WORD MonsterKind;// ¹øÈ£
	char Name[17];// ¸ó½ºÅÍÀÌ¸§	
	char EngName[40];// ¿µ¾îÀÌ¸§
	BYTE Grade;// ¸ó½ºÅÍ·¹º§	
	int MoveDramaNum;// ÀÌµ¿¿¬Ãâ ¹øÈ£	
	WORD D3Num;// ¸ó½ºÅÍ CHX_Index	
	float Size;// ¸ó½ºÅÍ Å©±â	
	BYTE Gravity;	// ¸ó½ºÅÍ ºñÁß
	float Weight;// ¸ó½ºÅÍ ¹«°Ô	
	DWORD MonsterRadius;
	WORD Attack1Index;	//°ø°Ý A ¹«°ø ÀÎµ¦½º
	WORD Attack2Index;	//°ø°Ý A ¹«°ø ÀÎµ¦½º
	
	int  Damage1DramaNum;// ¸ÂÀ½ 1 ¿¬Ãâ ¹øÈ£	
	int  Die1DramaNum;// Á×À½ 1 ¿¬Ãâ ¹øÈ£	
	int  Die2DramaNum;// Á×À½ 2 ¿¬Ãâ ¹øÈ£	

	
	DWORD Life;// »ý¸í·Â	
	
	
	WORD Attack1PhysicalMin;// °ø°Ý ÃÖ¼Ò¹°¸® °ø°Ý·Â	
	WORD Attack1PhysicalMax;// °ø°Ý ÃÖ´ë¹°¸® °ø°Ý·Â	
	WORD PhysicalDefense;// ¹°¸® ¹æ¾î·Â	
	WORD PropertyDefense1;// ¼Ó¼ºÀúÇ×·Â(È­)% 	
	WORD PropertyDefense2;// ¼Ó¼ºÀúÇ×·Â(¼ö)% 	
	WORD PropertyDefense3;// ¼Ó¼ºÀúÇ×·Â(¸ñ)% 	
	WORD PropertyDefense4;// ¼Ó¼ºÀúÇ×·Â(±Ý)% 	
	WORD PropertyDefense5;// ¼Ó¼ºÀúÇ×·Â(Åä)% 	

	WORD ExpPint;// °æÇèÄ¡

	
	WORD MinDropMoney;// ÃÖ¼Òµ· ¾×¼ö	
	WORD MaxDropMoney;// ÃÖ´ë µ· ¾×¼ö	
	BYTE MoneyPercent;// µ· È®·ü(%)	
	BYTE ItemPercent;// ¾ÆÀÌÅÛ È®·ü(%)	
	BYTE NoItemPercent;// ³ëÅÛ È®·ü(%)	
	WORD ItemListNum;// ÇØ´ç¾ÆÀÌÅÛ¸®½ºÆ®Ç¥¹øÈ£	
	BYTE MaxDropItemNum;// ÃÖ´ë¾ÆÀÌÅÛ °¹¼ö
	BYTE AttackNum;// °ø°Ý º¸À¯¼ö	
	
	BYTE Attack1Percent;// °ø°Ý A ¹ßµ¿È®·ü(%)	
	BYTE Attack2Percent;// °ø°Ý B ¹ßµ¿È®·ü(%)	

	// AI ÀÎÀÚ ºÎºÐ
	BYTE StandRate;
	BYTE WalkRate;
	BYTE RunRate;
	WORD SearchArea;// ¸ó½ºÅÍ ½Ã¾ß	
	BYTE SearchAngle;// ½Ã¾ß °¢µµ	
	WORD WalkMove;// ¸ó½ºÅÍ°È±âÀÌµ¿·Â	
	WORD RunMove;// ¸ó½ºÅÍ¶Ù±âÀÌµ¿·Â	
	WORD RunawayMove;// ¸ó½ºÅÍµµ¸ÁÀÌµ¿·Â	
	WORD DomainRange;// ¿µ¿ª¹üÀ§

	DWORD ForgiveDist;	// ÃßÀû Æ÷±â °Å¸®
	DWORD ForgiveTime;	// ÃßÀû Æ÷±â ½Ã°£
	BYTE BeforeAttack;	// ¼±°ø°Ý¿©ºÎ
	WORD SearchSelect;	// Å¸°Ù ¼±È£µµ ¼±ÅÃ		ÃÖ´Ü°Å¸® or ¹ß°ß
	WORD TargetSelect; // °ø°Ý ¹ÞÀ»½Ã ¼±ÅÃ
	WORD Tall;

	WORD RunawayCondition;
	WORD RunawayProperty;
	WORD RunawayDistance;	//????

	WORD Talk1;				// ´ë»ç 1
	WORD Talk2;				
	WORD Talk3;

	WORD HelpRequestCondition;
	WORD HelpRequestDistance;
	WORD HelpRequestTargetNum;

	WORD RecallCondition;
	WORD RecallObjectID;
	WORD RecallObjectNum;
	
}MONSTER_LIST;


class BASE_MONSTER_LIST
{
public:
	
	WORD		MonsterKind;			// ¹øÈ£
	char		Name[17];				// ¸ó½ºÅÍÀÌ¸§	
	char		EngName[17];			// ¿µ¾îÀÌ¸§
	LEVELTYPE	Level;					// ¸ó½ºÅÍ·¹º§	
	int			MoveDramaNum;				// ÀÌµ¿¿¬Ãâ ¹øÈ£	
	DWORD		MotionID;
	char		ChxName[256];					// ¸ó½ºÅÍ CHX_Index	
	float		Scale;					// ¸ó½ºÅÍ Å©±â	
	BYTE		Gravity;				// ¸ó½ºÅÍ ºñÁß
	float		Weight;					// ¸ó½ºÅÍ ¹«°Ô	
	DWORD		MonsterRadius;
	DWORD		Life;					// »ý¸í·Â	
	DWORD		Shield;					// È£½Å°­±â	
	EXPTYPE		ExpPoint;				// °æÇèÄ¡
	WORD		Tall;					// Å°
};

typedef struct _Tile_List
{
	DWORD TileAtt;
	char TileName[17];// TileÀÌ¸§
	WORD D3Num;// ¸ó½ºÅÍ CHX_Index	
}TILE_LIST;

typedef struct _MonsterPoint
{
	int MonsterGrade;
	CPoint point;
	WORD MonsterGroup;
	DWORD MonsterIndex;
	WORD MonsterKind;
	BOOL RegenFlag;
	WORD SelectCount;
	int SelCount;	//¸®½ºÆ®¹Ú½ºÀÇ ¶óÀÎ¹øÈ£
	GXOBJECT_HANDLE MHandle;
}MONSTER_POINT;

// AddCondition value
typedef struct _AddProperty
{
	int SelCount;
	CString szTargetGroupID;
	CString szCondition;
	CString szRegenTime;
}ADD_PROPERTY;

// List Condition Value
typedef struct _ListCondition
{
	WORD wRegenCount;
	BYTE ConditionNum;
	CString szGroupNum;
	CString szGroupName;
	CString szGroupProperty;
	ADD_PROPERTY AddCondition[100];
}LIST_CONDITION;


/*
struct MONSTER_TOTALINFO
{
	WORD Life;
	WORD MonsterKind;
	BYTE Group;	
	BYTE MapNum;
};

struct NPC_TOTALINFO
{
	WORD NpcKind;
	WORD Group;
	BYTE NpcJob;
	BYTE MapNum;
};

struct BASEMOVE_INFO
{
	VECTOR3 CurPosition;
	VECTOR3 TargetPosition;

	bool bMoving;
	bool MoveMode;		 // 0:¶Ù±â 1:°È±â

	WORD KyungGongIdx;	// °æ°øIdx 0: °æ°øÁßÀÌ ¾Æ´Ï´Ù  !0: »ç¿ëÁßÀÎ °æ°ø¹øÈ£
};

struct BASEOBJECT_INFO
{
	DWORD dwObjectID;
	char ObjectName[MAX_NAME_LENGTH+1];
	BYTE ObjectState;
};

struct StaticString
{
	StaticString()	{	Str = NULL;	}
	~StaticString()	{	SAFE_DELETE_ARRAY(Str);	}

	char* Str;

	void operator = (char* pchar)
	{
		ASSERT(pchar);
		ASSERT(Str == NULL);

		Str = new char[strlen(pchar)+1];
		strcpy(Str,pchar);
	}
	operator LPCTSTR() const
	{
		return Str;
	}
	operator PCHAR() const
	{
		return Str;
	}
	
};
*/
/*
typedef struct _MOD_LIST
{
	_MOD_LIST()
	{
		MaxModFile = 0;
		ModFile = NULL;
	}
	~_MOD_LIST()
	{
		if(MaxModFile == 0)
			return;
		SAFE_DELETE_ARRAY(ModFile);
	}
	DWORD MaxModFile;
	StaticString* ModFile;
	StaticString BaseObjectFile;
	
}MOD_LIST;
*/
typedef struct _MONSTERCHX
{
	StaticString FileName;
}MONSTERCHX;

/*
enum JOB
{
	eJOB_MuSa,
	eJOB_GwunSa,
	eJOB_DoJuk,
	eJOB_GungSa,
	eJOB_SangIn,
	eJOB_EuWon,
	eJOB_SamRyuMuSa,
	eJOB_HwangJok,

	eJOB_Max

};
*/

typedef struct _CAIGROUP
{
	DWORD dwGroupID;
	DWORD dwMaxObjectNum;
	DWORD curObjectNum;
	WORD wProperty;
	char szGroupName[16];
}CAIGROUP;


#endif //__DefineStruct_h__

