#ifndef __COMMONGAMESTRUCT_H__
#define __COMMONGAMESTRUCT_H__


//////////////////////////////////////////////////////////////////////////
// ¼Ó¼º °ü·Ã
#ifdef _JAPAN_LOCAL_
enum ATTRIBUTE_ELEMENT
{
	ATTR_NONE = 0,
	ATTR_FIRE = 1,
	ATTR_EARTH = 2,
	ATTR_IRON = 3,
	ATTR_WATER = 4,
	ATTR_TREE = 5,

	ATTR_MAX = 5,

	ATTR_NOATTR = 6,		// ¹«¼Ó¼º	
	ATTR_ALLATTR = 7,		// Àü¼Ó¼º
};
#else
enum ATTRIBUTE_ELEMENT
{
	ATTR_NONE = 0,
	ATTR_FIRE = 1,
	ATTR_WATER = 2,
	ATTR_TREE = 3,
	ATTR_IRON = 4,
	ATTR_EARTH = 5,

	ATTR_MAX = 5,

	ATTR_NOATTR = 6,		// ¹«¼Ó¼º	
	ATTR_ALLATTR = 7,		// Àü¼Ó¼º
};
#endif


template < class T >
union ATTRIBUTE_VAL
{
private:
	struct{
	T FireElement;			//È­¼Ó¼º
	T WaterElement;			//¼ö¼Ó¼º
	T TreeElement;			//¸ñ¼Ó¼º
	T GoldElement;			//±Ý¼Ó¼º
	T EarthElement;			//Åä¼Ó¼º
	T NoneElement;			//¹«¼Ó¼º
	};

	T Element[ATTR_MAX+1];
public:
	ATTRIBUTE_VAL()
	{
		NoneElement = 0;
	}
	T GetElement_Val(WORD Attrib)
	{
		if(Attrib == ATTR_NONE)		// ¼Ó¼º ¾øÀ½
			return 0;
//		if(Attrib == ATTR_NOATTR)	// ¹«¼Ó¼ºÀÏ°æ¿ì 0
//			return 0;

/*
#ifndef _RMTOOL_
		ASSERTMSG(Attrib != ATTR_ALLATTR,"전속성은 값을 불러올 수 없습니다.(Call to yunho plz~ ^-^)");
#else
		ASSERT(Attrib != ATTR_ALLATTR);
#endif _RMTOOL_
*/

		if(Attrib > ATTR_MAX+1)
		{
/*
			#ifndef _RMTOOL_
						ASSERTMSG(0,"윤호를 불러주세요~ -0-");
			#else
						ASSERT(0);
			#endif _RMTOOL_
						return 0;*/
			
		}
		return Element[Attrib-1];
	}
	void SetElement_Val(WORD Attrib,T val)
	{
		if(Attrib == ATTR_NONE)		// ¼Ó¼º ¾øÀ½
			return;
//		if(Attrib == ATTR_NOATTR)	// ¹«¼Ó¼ºÀÏ°æ¿ì 0
//			return;
/*
#ifndef _RMTOOL_		
		ASSERTMSG(Attrib != ATTR_ALLATTR,"전속성은 값을 불러올 수 없습니다.(Call to yunho plz~ ^-^)");
		if(Attrib > ATTR_MAX+1)
		{
			ASSERTMSG(0,"윤호를 불러주세요~ -0-");
			return;
		}
#endif*/

		Element[Attrib-1] = val;
	}
	void AddATTRIBUTE_VAL(ATTRIBUTE_VAL<T>& attr_val)
	{
		for(int n=0;n<ATTR_MAX+1;++n)
			Element[n] += attr_val.Element[n];
	}
	void AddATTRIBUTE_VAL(ATTRIBUTE_VAL<T>& attr_val,float fRate)
	{
		for(int n=0;n<ATTR_MAX+1;++n)
			Element[n] += (T)(attr_val.Element[n]*fRate);
	}
	void CheckLimit(T max_val)
	{
		for(int n=0;n<ATTR_MAX+1;++n)
		{
			if(Element[n] > max_val)
				Element[n] = max_val;
		}
	}
};

typedef ATTRIBUTE_VAL<float>	ATTRIBUTEATTACK;	// ¼Ó¼º °ø°Ý·Â // ¹ÎÇö¾¾°¡ floatÀÌ¶ó°í ¸»ÇÔ [9/22/2003]
typedef ATTRIBUTE_VAL<float>	ATTRIBUTEREGIST;	// ¼Ó¼º ÀúÇ×·Â // [0~1]
//////////////////////////////////////////////////////////////////////////



//------------------------------------------------------------------------------------------------------
// »õ·Î ÀÛ¾÷ÇÑ ½ºÅÝ 
// 2003-05-12 taiyo
// »ç¿ëµÇÁö ¾ÊÀ¸¸é ¹Ýµå½Ã 0À¸·Î ¼ÂÆÃÇØÁÖ¾î¾ß ÇÔ.
// 1. WORD GenGol;WORD MinChub;
// 2. pItem_stats->MeleeAttackPowerMax + pItem_stats->RangeAttackPowerMax
struct player_calc_stats
{
	WORD GenGol;
	WORD MinChub;
	WORD CheRyuk;
	WORD SimMaek;

	// ¹°¸® °ø°Ý
	WORD MeleeAttackPowerMin;		// ¾ÆÀÌÅÛ
	WORD MeleeAttackPowerMax;
	WORD RangeAttackPowerMin;
	WORD RangeAttackPowerMax;
	
	// ¹°¸® °ø°Ý Ãß°¡ µ¥¹ÌÁö(È£È¯)
	float MeleeAttackPowerAddRateMin;
	float MeleeAttackPowerAddRateMax;
	float RangeAttackPowerAddRateMin;
	float RangeAttackPowerAddRateMax;

	DWORD MaxLife;
	DWORD MaxShield;
	DWORD MaxNaeRyuk;
	
	// Å©¸®Æ¼ÄÃ
	DWORD Critical;
	//////////////////////////////////////////////////////
	// 06. 07 내공 적중(일격) - 이영준
	DWORD Decisive;
	//////////////////////////////////////////////////////
	// ¹°¸® ¹æ¾î
	WORD PhysicalDefense;

	// ¼Ó¼º °ø°Ý
	ATTRIBUTEATTACK AttributeAttack;			// ¾ÆÀÌÅÛ(ÀåÂø½Ã °ø°Ý·Â »ó½Â)

	// ¼Ó¼º ÀúÇ×
	ATTRIBUTEREGIST AttributeResist;			// ¾ÆÀÌÅÛ(ÀåÂø½Ã ÀúÇ×·Â »ó½Â)
};

struct titan_calc_stats
{
	DWORD MaxFuel;
	DWORD MaxSpell;

	DWORD Critical;

	WORD PhysicalDefense;

	WORD MeleeAttackPowerMin;
	WORD MeleeAttackPowerMax;
	WORD RangeAttackPowerMin;
	WORD RangeAttackPowerMax;

	float AttackRange;
	float MoveSpeed;

	ATTRIBUTEATTACK AttributeAttack;
	ATTRIBUTEREGIST AttributeResist;
};

struct monster_stats
{
	WORD	Attack1OccurProportion;					// °ø°Ý1 ¹ß»ý È®·ü
	// ¹°¸® °ø°Ý
	WORD	PhysicalAttackMin1;						// °ø°Ý1 ÃÖ¼Ò °ø°Ý·Â
	WORD	PhysicalAttackMax1;
	WORD	PhysicalAttackMin2;						// °ø°Ý1 ÃÖ¼Ò °ø°Ý·Â
	WORD	PhysicalAttackMax2;

	// ¼Ó¼º °ø°Ý
	WORD	AttributeAttackMin1;
	WORD	AttributeAttackMax1;
	WORD	AttributeAttackMin2;
	WORD	AttributeAttackMax2;

	// ¹°¸® ¹æ¾î
	WORD	PhysicalDefense;	
	// ¼Ó¼º ÀúÇ×
	ATTRIBUTEREGIST AttributeResist;
};

//------------------------------------------------------------------------------------------------------



//////////////////////////////////////////////////////////////////////////
// Tile¸Þ´ÏÁ®¿¡¼­ ¾²´Â ºÎºÐ

struct TILEINDEX
{
	int nx,nz;	
	inline TILEINDEX operator +(const TILEINDEX &ti)
	{
		TILEINDEX result;
		result.nx	=	this->nx + ti.nx;
		result.nz	=	this->nz + ti.nz;
		return	result;
	}
	inline BOOL operator ==(const TILEINDEX &ti)
	{
		return (this->nx == ti.nx) && (this->nz == ti.nz);
	}
};

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// LBS 03.10.22 NPC ´ë»ç¿¡ »ç¿ëÇÑ´Ù.

#define MAX_REGIST_HYPERLINK  70

enum LINKTYPE {
	emLink_Null,	// ÀÏ¹Ý ÅØ½ºÆ®
	emLink_Page,	// Page Link
	emLink_End,		// Á¾·á Link
	emLink_Open,	// °Å·¡Ã¢ Link
	emLink_Quest,	// Äù½ºÆ® °ü·Ã	-- 4
	emLink_QuestLink,			//---- 5
	emLink_QuestStart,			//---- 6 
	emLink_QuestContinue,		//---- 7
	emLink_QuestEnd,			//---- 8
	emLink_MapChange,			//---- 9
	emLink_EventQuestStart,
	emLink_Image,	// ÀÌ¹ÌÁö °ü·Ã
};

struct DIALOGUE {
	void Init()
	{
		wType = 0;
		dwColor = RGB(255,255,255);
		memset(str, 0, 256);
		wLine = 0;
	}
	WORD	wType; // 0 : ÅØ½ºÆ®, 1 : ÀÌ¹ÌÁö, 2 : HyperLink
	DWORD	dwColor;
	char	str[256];
	WORD	wLine;
};

struct HYPERLINK
{
	WORD		wLinkId;
	WORD		wLinkType;
	DWORD		dwData;
};

struct  HYPER
{
	void Init()
	{
		bUse = FALSE;
		dwListItemIdx = 0;
	}

	HYPERLINK	sHyper;			// ÇÏÀÌÆÛ ¸µÅ© Á¤º¸
	DWORD		dwListItemIdx;	// ¸®½ºÆ®¿¡¼­ÀÇ À§Ä¡ÁÂÇ¥
	BOOL		bUse;
};
//////////////////////////////////////////////////////////////////////////



// RaMa

struct QString
{
	QString()	{	Str = NULL;	Line=0;	}
	~QString()	{	if(Str) delete [] Str;	Str=0;}

	char* Str;
	int		Line;
	DWORD	Color;

	void operator = (char* pchar)
	{
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

//--



#endif //__COMMONGAMESTRUCT_H__