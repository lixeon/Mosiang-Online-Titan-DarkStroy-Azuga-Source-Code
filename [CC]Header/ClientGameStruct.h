#ifndef __CLIENTGAMESTRUCT_H__
#define __CLIENTGAMESTRUCT_H__

//------------------------------------------------------------------------------------
//
//	ClientGameStruct.h: Å¬¶óÀÌ¾ðÆ® °ÔÀÓ½Ã½ºÅÛ¿¡¼­¸¸ »ç¿ëÇÏ´Â struct Á¤ÀÇ
//
//	»ç¿ë ¿øÄ¢:
//	1. comment¸¦ ÇÑ´Ù
//	2. ÀÌ ÆÄÀÏ¿¡ »ç¿ëÇÏ±â¿¡ Àû´çÇÑÁö ´Ù½Ã ÇÑ¹ø »ý°¢ÇÑ´Ù
//
//------------------------------------------------------------------------------------
class CObject;
typedef CObject* LPOBJECT;

template<class T>
class ObjectArray {
public:
	DWORD nTempCount;	// ¼øÂ÷Á¢±ÙÀ» À§ÇÑ º¯¼ö
	DWORD nObjectNum;
	T pObjectArray[MAX_TARGET_NUM];
	void Init()	{	nObjectNum = 0;	memset(pObjectArray, 0, sizeof(T)*MAX_TARGET_NUM); }
	void AddObject(T Object)
	{
		pObjectArray[nObjectNum++] = Object;
	}
	void SetPositionHead()	{	 nTempCount = 0;	}
	T GetNextObject()		{
		if(nTempCount >= nObjectNum)
			return NULL;	
		else
			return pObjectArray[nTempCount++];
	}
};

struct GameLoadingParam
{
	DWORD selectedCharNum;
	DWORD m_MapNum;
};

#include "cStatic.h"
#include "cGuagen.h"

struct TITANSTATS_ATTRDEF
{
	ATTRIBUTE_VAL<cStatic*> pStatic;
	ATTRIBUTE_VAL<cGuagen*> pGuage;

	void SetValue(WORD attrib,WORD value,DWORD color=0)
	{
		char temp[32];
		sprintf(temp,"%d%%",value);
		pStatic.GetElement_Val(attrib)->SetStaticText(temp);
		pGuage.GetElement_Val(attrib)->SetValue(value*0.01f);
		if( color )
			pStatic.GetElement_Val(attrib)->SetFGColor(color);
	}
};

// magi82 - Titan(070423) 타이탄 스텟 관련 컨트롤 구조체
struct TITAN_STATS_CTRL
{
	cStatic*			titanType;			// 종류
	cStatic*			titanAtt;			// 근접공격
	cStatic*			titanLongAtt;		// 원거리공격
	cStatic*			titanMagicAtt;		// 마력공격
	cStatic*			titanCritical;		// 일격
	cStatic*			titanDistance;		// 사정거리
	cStatic*			titanDef;			// 방어력
	cStatic*			titanLife;			// 연료
	//cStatic*			titanMana;			// 내력
	TITANSTATS_ATTRDEF	TitanAttrDef;		// 각종 저항력(화,수,목,금,토)
};
//////////////////////////////////////////////////////////////////////////

////////// 2007. 5. 18. CBH - ScriptManager Parse Type 관련 struct 추가 //////////////
struct PARSE_TYPE_INFO
{
	int nType;
	char cpTypeName[MAX_PARSE_TYPE_NAME_LENGTH];
	int nKeyValue;
};
/////////////////////////////////////////////////////////////////////////

// magi82 - SOS(070724)
// SOS 멤버 관련 구조체
struct SOSMEMBERINFO
{
	char strName[MAX_NAME_LENGTH+1];
	DWORD level;
};

#endif //__CLIENTGAMESTRUCT_H__
