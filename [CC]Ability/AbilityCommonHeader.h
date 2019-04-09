#ifndef _ABILITYHEADER_
#define _ABILITYHEADER_

WORD GetAbilityIndex_WithKindPosition(BYTE Kind,BYTE Position);
void GetKindPosition_WithAbilityIndex(WORD AbilityIndex,BYTE* pOutKind,BYTE* pOutPosition);
BYTE GetAbilityLevel_WithDBValue(BYTE DBValue);
// 06. 01 이영준 - 단축창 변경
// 이 함수를 사용하지 않는다
//WORD GetAbilityQuickPosition(BYTE DBValue);

#define ABILITYUPDATE_CHARACTERLEVEL_CHANGED			0x00000001
#define ABILITYUPDATE_ABILITYUPITEM_USE					0x00000002
#define ABILITYUPDATE_ABILITYLEVEL_CHANGED				0x00000004
#define ABILITYUPDATE_ABILITYEXP_CHANGED				0x00000008
#define ABILITYUPDATE_ABILITYQUICK_CHANGED				0x00000010
#define ABILITYUPDATE_ABILITYINITED						0x00000020
#define ABILITYUPDATE_ABILITYLEVEL_CHANGEDBYSHOPITEM	0x00000100
#define ABILITYUPDATE_CHARACTERSTAGE_CHANGED			0x00000200

enum eAbilityGradeUpMethod
{
	eAGM_OnOff,		//0: 1회성 성장형 : 한번 수련하여 익히면 효과를 가지게 되며 그 이상의 효과증진은 없다. (On/Off형)
	eAGM_GradeUp,	//1: 등급 성장형 : 특기치를 사용하여 계속해서 성장해 나갈수 있는 형태
	eAGM_Restrict,	//2: 제한 성장형 : 특정 특기가 해당 레벨에 도달하기 전에는 특기를 성장시킬수 없는 형태
};

enum eAbilityUseType
{
	eAUT_Use,		//0: 1회성 타입
	eAUT_Passive,	//1: 패시브 타입
	eAUT_Active,	//2: ON/OFF
};

enum eAbilityGroupKind
{
	eAGK_Battle,
	eAGK_KyungGong,
	eAGK_Society,
	eAGK_Job,

	eAGK_Max,
};

#define ABILITYLEVEL_DB_KEY		'A'

#define ABILITYQUICKPOSITION_NULL	' '
#define ABILITYQUICKPOSITION_KEY	'A'

#define ABILITY_MAX_LEVEL		30

#define MAX_TOTAL_ABILITY	(MAX_ABILITY_NUM_PER_GROUP*eAGK_Max)
#define MAX_ABILITY_NUM_PER_GROUP	40
#define MAX_ABILITY_LEVEL 30

#define ABILITY_STARTINDEX_BATTLE		100
#define ABILITY_STARTINDEX_KYUNGGONG	200
#define ABILITY_STARTINDEX_SOCIETY		300
#define ABILITY_STARTINDEX_JOB			400
#define ABILITY_STARTINDEX_INTERVAL		100


struct ABILITY_STATS
{
	DWORD PhyAttackUp[WP_MAX];
	ATTRIBUTE_VAL<float> AttAttackUp;
	ATTRIBUTE_VAL<float> AttRegistUp;
	DWORD DefenceUp;

	DWORD LifeUp;
	DWORD NaeRyukUp;
	DWORD ShieldUp;
	
	DWORD UngiUpVal;

	// Change Stage
	DWORD StatGen;	// 근
	DWORD StatMin;	// 민
	DWORD StatChe;	// 체
	DWORD StatSim;	// 심
	float Kyunggong;	// 경신법
	WORD  KyunggongLevel;
	float fNoAttrib;	// 무허공
	DWORD SkillDamage;	// 무공데미지
	DWORD CriticalDamage;	// 일격데미지
	DWORD dwTitanRidingPlusTime;	//2007. 11. 6. CBH - 타이탄 탑승 시간

	DWORD GetPhyAttackUp(WORD WeaponKind)
	{
		return PhyAttackUp[WeaponKind-1];
	}
	void SetPhyAttackUp(WORD WeaponKind,DWORD UpVal)
	{
		PhyAttackUp[WeaponKind-1] = UpVal;
	}
	void Clear()
	{
		memset(this,0,sizeof(ABILITY_STATS));
	}
};

struct ABILITY_CALC
{
	DWORD	dwPhyAttack;	// 물리 공격
	float	fAttribAttack;	// 속성 공격
	float	fAttribRegist;	// 속성 저항
	DWORD	dwLife;			// 생명력
	DWORD	dwDeffence;		// 방어력
	DWORD	dwNearyuk;		// 내력
	DWORD	dwShield;		// 호신
	DWORD	dwUngi;			// 운기
	DWORD	dwStat;			// 근, 민, 체, 심
	float	fKyunggong;		// 경공스피드
	float	fNoAttrib;		// 무허공
	DWORD	dwSkillDamage;	// 무공데미지
	DWORD	dwCriticalDamage;	// 일격데미지
	DWORD	dwTitanRidingPlusTime;	//2007. 11. 6. CBH - 타이탄 탑승 시간
};

enum eABILITY_USE_KIND
{
	eAUK_KYUNGGONG = 1,
	eAUK_JOB,

	eAUK_PHYATTACK,
	eAUK_ATTRIB,

	eAUK_MAXLIFE,
	eAUK_DEFENCE,
	eAUK_MAXNEARYUK,
	eAUK_MAXSHIELD,
	eAUK_UNGISPEED,

	eAUK_SOCIETY = 10,	//이건 10이라고 정했다.
	eAUK_PET	= 11,	//11 abilitybaseinfo.bin -> effect_kind
};

enum eABILITY_USE_KIND_STAGE	// 전직시 효과들 11~100
{
	eAUK_STAT1				= 11,	// 근
	eAUK_STAT2				= 12,	// 민
	eAUK_STAT3				= 13,	// 체
	eAUK_STAT4				= 14,	// 심
	eAUK_KYUNGGONG_SPEED	= 15,	// 경신법
	eAUK_JUSOOL				= 16,	// 주술
	eAUK_NOATTRIB			= 17,	// 무허공
	eAUK_SKILL_DAMAGE		= 18,	// 무공데미지
	eAUK_CRITICAL_DAMAGE	= 19,	// 일격데미지
};

enum eABILITY_USE_JOB
{
	eAUKJOB_Ungijosik = 101,	// 운기조식
	eAUKJOB_Vimu = 102,			// 비무신청
	eAUKJOB_Party = 103,		// 방파창설
	eAUKJOB_Guild = 104,		// 문파창설
	eAUKJOB_Deal = 105,			// 거래
	eAUKJOB_StreetStall = 106,  // 노점상개설

	eAUKJOB_Restraint = 108,	// 견제
	eAUKJOB_Attack = 109,		// 공격

	eAUKJOB_StallFind = 110,	// 노점상 검색 by Stiner(8)
	eAUKJOB_AutoNote = 111,	// 오토노트
	eAUKJOB_PartyFind = 112,	//2008. 6. 4. CBH - 방파 찾기 추가

	eAUKJOB_TitanExpert = 120,	//타이탄 숙련

	eAUKJOB_Upgrade = 201,		//등급업
	eAUKJOB_Mix = 202,			//조합
	eAUKJOB_Reinforce = 203,	//강화
	eAUKJOB_Dissolve = 204,		//분해
};

enum eABILITY_USE_PET
{
	eAUKPET_State	= 150,
	eAUKPET_Inven	= 151,
	eAUKPET_Skill	= 152,
	eAUKPET_Rest	= 153,
	eAUKPET_Seal	= 154,
};

enum eABILITY_USE_SOCIETY
{
	eAUKJOB_Bow		= 301,		// 인사(포권)
	eAUKJOB_Happy	= 302,		// 기쁨
	eAUKJOB_Sad		= 303,		// 슬픔
	eAUKJOB_Yes		= 304,		// 긍정
	eAUKJOB_No		= 305,		// 부정
	eAUKJOB_Sit		= 306,		// 앉기/일어나기
//	eAUKJOB_Stand	= 307,		// 일어나기
	eAUKJOB_BigBow	= 307,		// 격식인사
};

enum eAbilityIconState
{
	// 06. 01. 어빌리티창 인터페이스 수정 - 이영준
	// 보이는 조건 변경
	eAIS_NotUsable,
	eAIS_NotAvailable,
	eAIS_OnlyVisible,
	eAIS_Usable,
};


enum AbilityAcquireKind
{
	eAAK_Item,				//아이템을 사용하여 특기를 얻음
	eAAK_Quest,				//퀘스트를 통하여 특기를 얻음
	eAAK_OtherAbility,		//특정 특기가 일정 등급 이상이 도달하면 얻는다.
	eAAK_CharacterLevel,	//캐릭터의 레벨이 특정단계에 도달하면 습득
	eAAK_CharacterStage,	//전직했을경우 습득
	
	eAAK_Max,
};


#define ABILITYINDEX_UNGI	401	//운기조식
#define ABILITYINDEX_VIMU	402	//비무신청
#define ABILITYINDEX_PARTY	403	//방파창설
#define ABILITYINDEX_GUILD	404	//문파창설
#define ABILITYINDEX_EXCHANGE	405	//거래
#define ABILITYINDEX_STALL	406	//노점상개설


#define ABILITYINDEX_ITEMUPGRADE	407	//아이템등급업
#define ABILITYINDEX_ITEMMIX		408	//아이템조합
#define ABILITYINDEX_ITEMREINFORCE	409	//아이템강화
#define ABILITYINDEX_ITEMDISSOLVE	410	//아이템분해
#define ABILITYINDEX_LEADERSHIP		411	//지도력
#define	ABILITYINDEX_RESTRAINT		412	// 견제

/*DB 작업후
#define ABILITYINDEX_PETSTATE		501	//펫상태정보
#define ABILITYINDEX_PETINVEN		502	//펫인벤토리
#define ABILITYINDEX_PETSKILL		503	//펫스킬
#define ABILITYINDEX_PETREST		504	//펫휴식상태
#define ABILITYINDEX_PETSEAL		505	//펫봉인
*/
//임시
#define ABILITYINDEX_PETSTATE		450	//펫상태정보
#define ABILITYINDEX_PETINVEN		451	//펫인벤토리
#define ABILITYINDEX_PETSKILL		452	//펫스킬
#define ABILITYINDEX_PETREST		453	//펫휴식상태
#define ABILITYINDEX_PETSEAL		454	//펫봉인

#define ABILITYINDEX_TACTIC_HWA		122	//전투-진법(주술)
#define ABILITYINDEX_TACTIC_GEUK	128	//전투-진법(주술)
#define ABILITYINDEX_TACTIC_MUSSANG	129 //무쌍


#endif
