#ifndef _COMMONTOOLDEFINE_
#define _COMMONTOOLDEFINE_

#define SAFE_DELETE(a)			if((a))	{ delete (a); (a)=NULL; }
#define SAFE_DELETE_ARRAY(a)	if((a))	{ delete [] (a); (a)=NULL; }
#define SAFE_RELEASE(a)			if((a))	{ (a)->Release(); (a)=NULL; }

#define COLOR_EDIT_CHANGED			RGB(255,0,0)
#define COLOR_EDIT_DEFAULT			RGB(0,0,0)
#define COLOR_BUTTON_CHANGED		RGB(255,0,0)
#define COLOR_BUTTON_DEFAULT		RGB(0,0,0)

#define WM_CHANGEDATA	WM_USER+200

//////////////////////////////////////////////////////////////////////////
// DocTemplate Kind
enum TEMPKIND
{
	eTemp_UserInfo = 1,
	eTemp_Character,
	eTemp_MunpaInfo, 
	eTemp_Log, 
	eTemp_Admin,
	eTemp_OperLog,

	eTemp_Max,
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ¿¬°á Á¤º¸
struct MASINFO
{
	char	strIP[16];
	int		nPort;
	int		nServerSet;
	char	strServerName[256];
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// ¿î¿µÀÚ Á¤º¸
enum OPERPOWER
{
	ePower_Admin,		// ÀüÃ¼ ÀÌ¿ë°¡
	ePower_Operator,	// ¿î¿µÀÚ °ü¸® Á¢±Ù ºÒ°¡ 
	ePower_SubOper,		// ¿î¿µLog È®ÀÎ ºÒ°¡
	ePower_Guest,		// È®ÀÎ¸¸ °¡´É (¼öÁ¤ ºÒ°¡)
};

struct OPERATORINFO
{
	DWORD	dwOperIdx;						// Idx
	char	strOperID[20];					// ID
	char	strOperPWD[20];					// PWD
	char	strOperIP[20];					// IP
	WORD	wOperPower;						// POWER
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Á¤º¸ Ç¥½Ã Ã¢
enum eUserTempInfo
{
	eUserTemp_ID = 0, eUserTemp_PWD, eUserTemp_NAME, eUserTemp_IDCARD, eUserTemp_PHONE, eUserTemp_MAIL, eUserTemp_BIRTH,
	eUserTemp_LOGIN, eUserTemp_LOGOUT, eUserTemp_LEVEL, eUserTemp_MAX, eUserTemp_TOTAL,
};

enum eCharTempInfo
{
	eCharTemp_NAME = 0, eCharTemp_NICK, eCharTemp_LEVEL, eCharTemp_STEP, eCharTemp_EXP, eCharTemp_MUNPA, eCharTemp_MUNPAPOS,
	eCharTemp_FAME, eCharTemp_BADFAME, eCharTemp_GENGOL, eCharTemp_ATTACK, eCharTemp_CRITICAL, eCharTemp_MINCHUB, eCharTemp_LONGATTACK, eCharTemp_RANGE,
	eCharTemp_CHERYUK, eCharTemp_LIFE, eCharTemp_DEFFENCE,  eCharTemp_SIMMAK, eCharTemp_MANA, eCharTemp_SHIELD, eCharTemp_POINT,
	eCharTemp_FRESI, eCharTemp_WRESI, eCharTemp_TRESI, eCharTemp_GRESI, eCharTemp_TORESI,
	eCharTemp_INVENMONEY, eCharTemp_PYOGUKMONEY, eCharTemp_ABILITYPOINT, eCharTemp_LoginPoint, eCharTemp_MAX,
};

enum eMunpaTempInfo
{
	eMunpaTemp_NAME = 0, eMunpaTemp_MUNJU, eMunpaTemp_BIRTH, eMunpaTemp_COUNT, eMunpaTemp_FAME,
	eMunpaTemp_MONEY, eMunpaTemp_LOCATION, eMunpaTemp_KIND, eMunpaTemp_LEVEL, eMunpaTemp_MAX,
};

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Loading Flag
enum FLAGLOADING_ADMIN
{
	eAL_None				= 0x00000000,
	eAL_OperatorInfo		= 0x00000001,
	eAL_IpInfo				= 0x00000010,

	eAL_LoadingAll = eAL_OperatorInfo | eAL_IpInfo,
};

enum FLAGLOADING_CHAR
{
	eCL_None				= 0x00000000,
	eCL_BaseInfo			= 0x00000001,
	eCL_InvenInfo			= 0x00000010,
	eCL_PyogukInfo			= 0x00000100,
	eCL_PyogukItemInfo		= 0x00001000,
	eCL_MugongInfo			= 0x00010000,
	eCL_AbillityInfo		= 0x00100000,
	eCL_ItemOptionInfo		= 0x01000000,

	eCL_LoadingAll = eCL_BaseInfo | eCL_InvenInfo | eCL_PyogukInfo | eCL_PyogukItemInfo | eCL_MugongInfo | eCL_AbillityInfo | eCL_ItemOptionInfo,
};

enum FLAGLOADING_MUNPA
{
	eML_None				= 0x00000000,
	eML_BaseInfo			= 0x00000001,
	eML_ItemInfo			= 0x00000010,
	eML_MemberInfo			= 0x00000100,
	eML_ItemOptionInfo		= 0x00001000,

	eML_LoadingAll = eML_BaseInfo | eML_ItemInfo | eML_MemberInfo,
};

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Change Manager Define
enum OPER_KIND 
{
	eOp_Insert = 0, 
	eOp_Update, 
	eOp_Delete,
};

enum TABLE_KIND
{
	eTK_None = 0,
	eTK_CharacterInfo,
	eTK_MugongInfo,
	eTK_PyogukInfo,

	eTK_InventoryItem,
	eTK_AbilityInfo,
	eTK_PyogukItem,

	eTK_MunpaInfo,
	eTK_MunpaItem,

	eTK_PyogukAdd,
};

enum CHARACTER_SAVEFALG
{
	eCF_None			= 0x00000000,
	eCF_CharacterInfo	= 0x00000001,
	eCF_MugongInfo		= 0x00000010,
	eCF_PyogukInfo		= 0x00000100,
	eCF_InventoryItem	= 0x00001000,
	eCF_AbilityInfo		= 0x00010000,
	eCF_PyogukItem		= 0x00100000,
	eCF_PyogukAdd		= 0x01000000
};

enum MUNPA_SAVEFALG
{
	eMF_None			= 0x00000000,
	eMF_MunpaInfo		= 0x00000001,
	eMF_MunpaItem		= 0x00000010,
};

enum INFOFIELD {
	eIF_INVENTORY = 0,
	eIF_WEAR,
	eIF_PHYOGUK,
	eIF_MUGONG,
	eIF_JINBUB,
	eIF_SIMBUB,
	eIF_MUNPA,
};

enum MODALRETURN
{
	eMR_Cancel = 0,
	eMR_Edit,
	eMR_Abort,
	eMR_Delete,
};

//////////////////////////////////////////////////////////////////////////

#endif //_COMMONTOOLDEFINE_