#ifndef __GAMERESOURCESTRUCT_H__
#define __GAMERESOURCESTRUCT_H__


class ITEM_MIX_INFO;
class ITEM_MIX_RES;
class ITEM_MIX_MATERIAL;

class CSkillInfo;

//#define MAX_ITEMNAME_LENGTH	30

//-----------------------------------------------------------------------
//	직업별 기본 포인트 수치와 레벨업시 포인터 가중치
//-----------------------------------------------------------------------
struct CharBasicStats
{
	WORD	wGenGol;						// 근골
	WORD	wMinChub;							// 민첩
	WORD	wCheRyuk;							// 체력
	WORD	wSimMek;						// 심맥
	//새로운 기획 수정으로 없어짐
	//BYTE	BonusPoint;						// 보너스 포인트 general-15
};
class RECALLINFO
{
public:
	DWORD	RecallType;
	DWORD	RecallID;
	DWORD	RecallMonsterNum;
	DWORD	RecallRange;
};
class ATTACKPATTERNINFO
{
public:
	WORD	AttackPatternNum;
	WORD	* AttackPattern;		// 0->1->2->2->1->....
};
class CAttackPatternInfo
{
public:
	ATTACKPATTERNINFO * pAttackPatternInfo;
	virtual ~CAttackPatternInfo()
	{
		delete [] pAttackPatternInfo;
	}
	void Init(WORD wTotalNum)
	{
		MaxAttackNum = wTotalNum;
		pAttackPatternInfo = new ATTACKPATTERNINFO[MaxAttackNum];
		memset(pAttackPatternInfo, 0, sizeof(ATTACKPATTERNINFO)*MaxAttackNum);
	}
	void InitPattern(WORD PNum, WORD wTotalNum)
	{
		pAttackPatternInfo[PNum].AttackPatternNum = wTotalNum;
		pAttackPatternInfo[PNum].AttackPattern = new WORD[wTotalNum];
		memset(pAttackPatternInfo[PNum].AttackPattern, 0, sizeof(WORD)*wTotalNum);
	}
	void SetVal(WORD PNum, WORD PIdx, WORD PVal)
	{
		pAttackPatternInfo[PNum].AttackPattern[PIdx] = PVal;
	}
	WORD		MaxAttackNum;
};
class CRecallInfo
{
public:
	RECALLINFO * pRecallInfo;

	CRecallInfo(){ memset(this, 0, sizeof(CRecallInfo)); }
	virtual ~CRecallInfo()
	{
		if(pRecallInfo)
		delete [] pRecallInfo;
	}
	void Init(DWORD nRecall)
	{
		MaxRecallNum = nRecall;
		pRecallInfo = new RECALLINFO[MaxRecallNum];
		memset(pRecallInfo, 0, sizeof(RECALLINFO)*MaxRecallNum);
	}
	
	RECALLINFO * GetRecallInfo(DWORD dwIdx)
	{
		ASSERT(dwIdx < MaxRecallNum);
		return &pRecallInfo[dwIdx];
	}
	void SetRecallType(DWORD dwIdx, DWORD dwType)
	{
		pRecallInfo[dwIdx].RecallType = dwType;
	}
	void SetRecallID(DWORD dwIdx, DWORD dwID)
	{
		pRecallInfo[dwIdx].RecallID = dwID;
	}
	void SetRecallMonsterNum(DWORD dwIdx, DWORD dwNum)
	{
		pRecallInfo[dwIdx].RecallMonsterNum = dwNum;
	}
	void SetRecallRange(DWORD dwIdx, DWORD dwRange)
	{
		pRecallInfo[dwIdx].RecallRange = dwRange;
	}
protected:
	DWORD	MaxRecallNum;
	
};
//-----------------------------------------------------------------------
// 몬스터 리스트 정보
//-----------------------------------------------------------------------

class BASE_MONSTER_LIST
{
public:
	
	WORD		MonsterKind;			// 번호
	WORD		ObjectKind;				// Object번호 32 일반 33 보스 35 필드보스 36 필드보스부하
	char		Name[MAX_MONSTER_NAME_LENGTH +1];				// 몬스터이름	
	char		EngName[MAX_NAME_LENGTH+1];			// 영어이름
	LEVELTYPE	Level;					// 몬스터레벨	
	int			MoveDramaNum;				// 이동연출 번호	
	DWORD		MotionID;
	char		ChxName[MAX_CHXNAME_LENGTH];			// 몬스터 CHX_Index	
	float		Scale;					// 몬스터 크기	
	DWORD		Gravity;				// 몬스터 비중
	float		Weight;					// 몬스터 무게	
	DWORD		MonsterRadius;
	DWORD		Life;					// 생명력	
	DWORD		Shield;					// 호신강기	
	DWORD		ExpPoint;				// 경험치
	WORD		Tall;					// 키

	WORD		AttackPhysicalMin;// 공격 최소물리 공격력	
	WORD		AttackPhysicalMax;// 공격 최대물리 공격력
	WORD		CriticalPercent;// 크리티컬 퍼센트
	WORD		PhysicalDefense;// 물리 방어력	
	ATTRIBUTEREGIST AttribResist;		// 속성저항력

	WORD		WalkMove;// 몬스터걷기이동력	
	WORD		RunMove;// 몬스터뛰기이동력	
	WORD		RunawayMove;// 몬스터도망이동력

	int			Damage1DramaNum;// 맞음 1 연출 번호	
	int			Die1DramaNum;// 죽음 1 연출 번호	
	int			Die2DramaNum;// 죽음 2 연출 번호	

	DWORD		StandTime;
	BYTE		StandRate;
	BYTE		WalkRate;
	BYTE		RunRate;
	DWORD		DomainRange;					// 영역 범위 : 반지름
	DWORD		PursuitForgiveTime;
//	DWORD		PursuitForgiveStartTime;		//not inclue in txt list
	DWORD		PursuitForgiveDistance;	

	BOOL		bForeAttack;					// 선공격
	DWORD		SearchPeriodicTime;				// 탐색 주기
	WORD		TargetSelect;					// 타겟 선택 : FIND_CLOSE, FIND_FIRST
	DWORD		SearchRange;
	DWORD		SearchAngle;
	WORD		TargetChange;

	WORD		AttackKind;
	DWORD		AttackNum;
	DWORD *		AttackIndex;
	DWORD *		AttackRate;
	CSkillInfo ** SkillInfo;	
	
	
	WORD		InitHelpType;		// 0 : no request, 1~~
	BOOL		bHearing;
	DWORD		HearingDistance;
};

class SMONSTER_LIST : public BASE_MONSTER_LIST
{
public:
	/*
	virtual ~SMONSTER_LIST()
	{
		for(int i = 0 ; i < MaxAttackNum ; ++i )
			delete [] AttackNumPattern[i];
		delete [] AttackNumPattern;
		
		delete [] MaxAttackIndexNum;
	}
	WORD		MaxAttackNum;
	WORD		* MaxAttackIndexNum;
	WORD		** AttackNumPattern;
	*/

	CAttackPatternInfo AttackPatternClass;
	CRecallInfo RecallInfoClass;
};

class MONSTEREX_LIST : public BASE_MONSTER_LIST
{
public:
	MONEYTYPE MinDropMoney;				// 최소돈 액수	
	MONEYTYPE MaxDropMoney;				// 최대 돈 액수	

	
	WORD DropRate[eDROPITEMKIND_MAX];
	
	// 서버에서 사용
	// 초기화가 필요!!
	WORD CurDropRate[eDROPITEMKIND_MAX];


	WORD ItemListNum1;				// 해당아이템1리스트표번호
	WORD ItemListNum2;				// 해당아이템2리스트표번호 - 추가 
	WORD ItemListNum3;				// 해당아이템3리스트표번호 - 추가 


	BYTE MaxDropItemNum;			// 최대아이템 갯수

	float fRunawayLifeRate;			// 생명력 확률 0~100
	WORD RunawayRate;				// 도망 확률
	WORD RunawayHelpRate;			// 도망 도움확률
	WORD RunawayDistance;			// 도망 거리

	WORD Talk1;						// 대사 1
	WORD Talk2;				
	WORD Talk3;

	WORD HelpRequestCondition;
	WORD HelpRequestDistance;
	WORD HelpRequestTargetNum;

	WORD RecallCondition;
	WORD RecallObjectID;
	WORD RecallObjectNum;	
};
//////////////////////////////////////////////////////////////////////////
// 몬스터 chx 파일의 리스트
//////////////////////////////////////////////////////////////////////////
typedef struct _CHXLIST
{
	StaticString FileName;
}CHXLIST;

//-----------------------------------------------------------------------
// 펫 리스트 정보
//-----------------------------------------------------------------------
#define MAX_PET_GRADE	3
struct BASE_PET_LIST
{
	DWORD	PetKind;	//펫 인덱스
	char	Name[MAX_NAME_LENGTH+1];	//이름
	//PetGrade;
	DWORD	MotionID;	//모션덱ID
	WORD	Pet2DIdx;
	char	ChxName_PerGrade[MAX_PET_GRADE][MAX_CHXNAME_LENGTH];
	float	Scale;		//스케일
	WORD	SummonLevel;	//소환렙제
	WORD	Tall[MAX_PET_GRADE];	//이름출력위치
	float	SpeedFromHero;	//주인과의 속도 차이/기획서에 없음
	WORD	SkillIdx[MAX_PET_GRADE];	//사용스킬인덱스
	CSkillInfo*	SkillInfo[MAX_PET_GRADE];//사용스킬정보
	DWORD	SkillRecharge;	//스킬재충전시간(속도)
//	DWORD	SkillGuageMax;	//스킬총충전량
	DWORD	StaminaDecreasePerSkill[MAX_PET_GRADE];	//단계별 스킬 사용시 소모 스태미나
	DWORD	StaminaMax[MAX_PET_GRADE];	//팻스테미너(최대치):공식에의해변화
	WORD	InventoryTapNum[MAX_PET_GRADE];	//소유인벤토리탭수
	int		DieDramaNum[MAX_PET_GRADE];		//단계별 죽음 연출 번호
	WORD	BuffList[MAX_PET_GRADE][3];

//	WORD	EvolutionStep;	//진화단계
};

struct PET_RULE
{
	DWORD ResummonVilidTime;
	DWORD GradeUpProb1to2;
	DWORD GradeUpProb2to3;
	DWORD MaxFriendship;
	DWORD DefaultFriendship;
	DWORD RevivalFriendship;
	DWORD RevivalFriendship_Shop;
	int FriendShipPerSkill[3];
	int FriendShipPer10Sec[3];
	int FriendShipPerMasterDie;
	int FriendShipPerStaminaZero;
	int FriendShipPerTrade;
	int FriendShipPerUpgradeFail;
	int StaminaStand;
	int StaminaMove;
};

struct PET_BUFF_LIST
{
	WORD Idx;
	char BuffName[MAX_NAME_LENGTH+1];
	BYTE BuffKind;
	DWORD BuffSuccessProb;
	DWORD BuffValueData;
	DWORD BuffAdditionalData;
};

//SW070127 타이탄
//-----------------------------------------------------------------------
// 타이탄 리스트 정보
//-----------------------------------------------------------------------
#define MAX_TITAN_GRADE	3

//#define MAX_CHXNAME_LENGTH	32
struct TITANINFO_GRADE
{
	//char	BodyChxName[MAX_CHXNAME_LENGTH];
	//char	FaceModName[MAX_CHXNAME_LENGTH];
	//char	CloakModName[MAX_CHXNAME_LENGTH];
	//char	ShoulderModName[MAX_CHXNAME_LENGTH];

	DWORD	Speed;
	WORD	KyungGongSpeed[3];
	float	Scale;
	WORD	Tall;

	DWORD	MaxFuel;			// 연료, 캐릭터의 체력에 해당
	DWORD	MaxSpell;			// 마력, 캐릭터의 내력에 해당
	//DWORD	Endurance;		// 데미지당 내구도
	DWORD	Critical;
	float	AttackRange;

	WORD	MeleeAttackPowerMin;
	WORD	MeleeAttackPowerMax;
	WORD	RangeAttackPowerMin;
	WORD	RangeAttackPowerMax;

	//float	PhyAtk;			// 물리 공격
	float	PhyDef;			// 물리 방어

	ATTRIBUTEATTACK	AttAtk;			// 속성 공격
	ATTRIBUTEREGIST	AttReg;			// 속성 방어

	DWORD	RecallReduceTime;		//타이탄 재소환시간
	DWORD	dwRecallSpellTime;			//2007. 11. 19. CBH - 타이탄 소환 캐스팅 시간
};

struct BASE_TITAN_LIST
{
	DWORD	TitanIdx;						//타이탄 번호
	char	TitanName[MAX_NAME_LENGTH+1];	//타이탄 이름
	char	TitanBaseChxName[MAX_CHXNAME_LENGTH+1];
	char	TitanfaceModName[MAX_CHXNAME_LENGTH+1];
	//DWORD	MotionID;						//모션덱 ID
	WORD	Titan2DImage;						//
	
	TITANINFO_GRADE	GradeInfo[MAX_TITAN_GRADE];
};

struct TITAN_SKILLLINK
{
	//DWORD	linkIdx;
	DWORD	CharSkillIdx;
	DWORD	TitanSkillIdx;
#ifdef _DEBUG
	char	skillName[MAX_NAME_LENGTH+1];
#endif

};

struct TITAN_EQUIPITEM_ENDURANCE
{
	DWORD titanEquipItemIdx;
	DWORD decreaseEndurance;
};

struct TITAN_RULE
{
	DWORD	TitanRecallValidTime_ZeroFuel;	// 판타지맵 마력 소진으로 강제 해제되었을 때 재소환 제약 시간.
	DWORD	TitanRecallValidTime_Seal;		// 판타지맵 재소환 제약 시간.
	DWORD	TitanRecallValidTime_General;		// magi82(24) - Titan(071108) 일반맵 재소환 제약 시간.(일반맵은 무조건 강제 해제 재소환시간 적용)
	DWORD	TitanVampValidTime;			// 주인 체력 흡수 제약 시간.
	DWORD	TitanFuelRate_ForVamp;			// 주인 체력 흡수 조건. 타이탄 체력% 이하일 때.
	DWORD	MasterLifeRate_AllowForTitanVamp;			// 주인 체력 흡수 제한. 주인 체력이 전체 체력의 % 이상일 때 흡수.
	DWORD	MasterLifeRate_FromTitanVamp;			// 주인 체력 흡수 비율. 주인 전체 체력의 % 흡수.
	DWORD	FuelConvertRate_FromTitanVamp;			// 주인 체력 흡수 시 변환율 %.
	DWORD	MasterLifeRate_forTitanCall;			// 타이탄 소환 가능한 주인 체력 %.
	DWORD	TitanDamageShareRate;				// 주인과 분배하는 데미지 %.
	DWORD	Prob_DecreaseEndurance;				// 내구도 하락 확률 %.
	DWORD	Prob_DecreaseArmorEndurance_Atk;	// 공격시 방어구 내구도가 하락할 확률 %.
	DWORD	Prob_DecreaseArmorEndurance_Def;	// 방어시 방어구 내구도가 하락할 확률 %.
	DWORD	TitanMaintainTime;					// magi82(25)
};

//-----------------------------------------------------------------------
// 맵전환 좌표기억 구조체
//-----------------------------------------------------------------------
typedef struct _MAPCHANGE_INFO
{
	WORD Kind;
	char CurMapName[MAX_NAME_LENGTH+1];
	char ObjectName[MAX_NAME_LENGTH+1];
	WORD CurMapNum;
	WORD MoveMapNum;
	VECTOR3 CurPoint;
	VECTOR3 MovePoint;
	WORD chx_num;
	
	// 방향설정
}MAPCHANGE_INFO;

//-----------------------------------------------------------------------
// LogIn 좌표기억 구조체
//-----------------------------------------------------------------------
typedef struct _LOGINPOINT_INFO
{
	WORD Kind;
	char MapName[MAX_NAME_LENGTH+1];
	WORD MapNum;
	BYTE MapLoginPointNum;
	VECTOR3 CurPoint[10];
	WORD chx_num;

	// 방향설정
}LOGINPOINT_INFO;

//-----------------------------------------------------------------------
// NPC 정보
//-----------------------------------------------------------------------
typedef struct _NPC_LIST
{
	WORD NpcKind;			// Npc 종류
	char Name[MAX_NAME_LENGTH+1];			// Npc 이름
	WORD ModelNum;			// MonsterChxList에서의 번호
	WORD JobKind;			// 직업종류별구분
	float Scale;			// Npc 크기
	WORD Tall;				// 키
	BOOL ShowJob;
}NPC_LIST;

//-----------------------------------------------------------------------
// NPC 리젠
//-----------------------------------------------------------------------
typedef struct _NPC_REGEN
{

	_NPC_REGEN():dwObjectID(0),NpcKind(0)
	{
		Pos.x = Pos.y = Pos.z = 0;
	}
	DWORD	dwObjectID;		// 신규 추가 taiyo
	MAPTYPE	MapNum;			// Map번호
	WORD	NpcKind;		// NpcKind
	char	Name[MAX_NAME_LENGTH+1];		// Npc 이름
	WORD	NpcIndex;		// NpcIndex(고유번호로 사용)
	VECTOR3 Pos;			// 몬스터 위치
	float	Angle;			// 몬스터 각도
}NPC_REGEN;


//-----------------------------------------------------------------------
// MOD 리스트 
//-----------------------------------------------------------------------
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

//---------------<아이템 통합 리스트>------------------------------------------------------
struct ITEM_INFO
{
	WORD ItemIdx;			//		Item_Index	
	char ItemName[MAX_ITEMNAME_LENGTH+1];		//		Item_name
	WORD ItemTooltipIdx;	//		Item설명 툴팁 index
	WORD Image2DNum;		//		2D_Image	
	WORD ItemKind;			//		아이템종류	"0 : 영약 1 : 무공서 2 : 장착아이템 3 : 기타"
	DWORD BuyPrice;			//		구입가격	
	DWORD SellPrice;		//		판매가격	
//	WORD Rarity;			//		희소가치	
	DWORD Rarity;			//		희소가치	
	WORD WeaponType;		//		무기유형	장착 / 무공서
	WORD GenGol;			//		근골+(영구)	영약과 장착 아이템에 공통사용
	WORD MinChub;			//		민첩+(영구)	영약과 장착 아이템에 공통사용
	WORD CheRyuk;			//		체력+(영구)	영약과 장착 아이템에 공통사용
	WORD SimMek;			//		심맥+(영구)	영약과 장착 아이템에 공통사용
	DWORD Life;				//		생명력+(영구)	영약과 장착 아이템에 공통사용
	DWORD Shield;			//		호신강기+(영구)	영약과 장착 아이템에 공통사용
	WORD NaeRyuk;			//		내력+(영구)	영약과 장착 아이템에 공통사용
	ATTRIBUTEREGIST AttrRegist;		//		화 속성 저항력+(영구)	영약과 장착 아이템에 공통사용
							//		수 속성 저항력+(영구)	영약과 장착 아이템에 공통사용
							//		목 속성 저항력+(영구)	영약과 장착 아이템에 공통사용
							//		금 속성 저항력+(영구)	영약과 장착 아이템에 공통사용
							//		토 속성 저항력+(영구)	영약과 장착 아이템에 공통사용
	WORD LimitJob;			//		장착가능 직업	장착 / 무공서에만 사용 / 상승무공-화경, 극마
	WORD LimitGender;		//		장착가능 남녀	장착 / 무공서에만 사용
	LEVELTYPE LimitLevel;		//		장착가능 레벨	장착 / 무공서에만 사용
	WORD LimitGenGol;		//		장착가능 근골	장착 / 무공서에만 사용
	WORD LimitMinChub;		//		장착가능 민첩	장착 / 무공서에만 사용
	WORD LimitCheRyuk;		//		장착가능 체력	장착 / 무공서에만 사용
	WORD LimitSimMek;		//		장착가능 심맥	장착 / 무공서에만 사용

	WORD ItemGrade;			//	장착아이템	아이템 등급	
	WORD RangeType;			//				거리유형	
	WORD EquipKind;			//				장착종류	
	WORD Part3DType;		//				3D파츠번호	
	WORD Part3DModelNum;	//				3D파츠모델번호	
	WORD MeleeAttackMin;	//				근거리 최소공격력
	WORD MeleeAttackMax;	//				근거리 최대공격력
	WORD RangeAttackMin;	//				원거리 최소공격력
	WORD RangeAttackMax;	//				원거리 최대공격력
	WORD CriticalPercent;	//				크리티컬 퍼센트
	ATTRIBUTEATTACK AttrAttack;		//				화속성공격력+%
							//				수속성공격력+%
							//				목속성공격력+%
							//				금속성공격력+%
							//				토공격공격력+%
	WORD PhyDef;			//				물리 방어력+ 
	WORD Plus_MugongIdx;	//				특정 무공+ index
	WORD Plus_Value;		//				특정 무공+ 정도
	WORD AllPlus_Kind;		//				무공 유형별 무공+ 
	WORD AllPlus_Value;		//				무공 유형별 무공+ 정도

	WORD MugongNum;			//	무공서	연결 무공  번호
	WORD MugongType;		//			무공유형

	WORD LifeRecover;		//	영약	생명력+(회복) 고정수치	영약 전용
	float LifeRecoverRate;		//			생명력+(회복)  	영약 전용
	WORD NaeRyukRecover;	//			내력+(회복) 고정수치	영약 전용
	float NaeRyukRecoverRate;	//			내력+(회복)    	영약 전용

	WORD ItemType;			//	기타	아이템 속성	=>퀘스트, 비극서, 재료
	WORD wItemAttr;			// 아이템속성(화, 수, 목, 금, 토) => 일본 로컬관련
	WORD wAcquireSkillIdx1;	// 필요한 스킬 인덱스 => 일본로컬관련
	WORD wAcquireSkillIdx2;	// 필요한 스킬 인덱스 => 일본로컬관련
	WORD wDeleteSkillIdx;	// 지울 스킬 인덱스 => 일본로컬관련

	WORD wSetItemKind;		// 세트아이탬 종류  *** 2007. 6. 7. CBH ***
};


//// 2007. 6. 8. CBH - SETITEM 옵션 관련 구조체 추가 ////////////////////////////////////////////
struct SET_ITEM_OPTION
{
	WORD wIndex;									// 세트아이탬 옵션 인덱스
	WORD wSetItemKind;								// 세트아이탬 종류
	char szSetItemName[MAX_NAME_LENGTH+1];			// 세트아이탬 접두어 이름
	
	WORD wGenGol;									//	근골
	WORD wMinChub;									//	민첩
	WORD wCheRyuk;									//	체력
	WORD wSimMek;									//	심맥
	DWORD dwLife;									//	생명력
	DWORD dwShield;									//	호신강기
	DWORD dwNaeRyuk;									//	내력
	ATTRIBUTEREGIST AttrRegist;						//	화 속성 저항력+(영구)
													//	수 속성 저항력+(영구)
													//	목 속성 저항력+(영구)
													//	금 속성 저항력+(영구)
													//	토 속성 저항력+(영구)

	LEVELTYPE wLimitLevel;							//	장착가능 레벨
	char szEffect[100];								//	적용 이펙트
	WORD wLimitGenGol;								//	장착가능 근골
	WORD wLimitMinChub;								//	장착가능 민첩
	WORD wLimitCheRyuk;								//	장착가능 체력
	WORD wLimitSimMek;								//	장착가능 심맥	
	
	WORD wMeleeAttackMin;							//	근거리 최소공격력
	WORD wMeleeAttackMax;							//	근거리 최대공격력
	WORD wRangeAttackMin;							//	원거리 최소공격력
	WORD wRangeAttackMax;							//	원거리 최대공격력
	WORD wCriticalPercent;							//	크리티컬 퍼센트
	ATTRIBUTEATTACK AttrAttack;						//	화속성공격력+%
													//	수속성공격력+%
													//	목속성공격력+%
													//	금속성공격력+%
													//	토공격공격력+%
	
	WORD wPhyDef;									//	물리 방어력+ 
	WORD wLifeRecover;								//	생명력+(회복) 고정수치
	float fLifeRecoverRate;							//	생명력+(회복)
	WORD wNaeRyukRecover;							//	내력+(회복) 고정수치
	float fNaeRyukRecoverRate;						//	내력+(회복)
	WORD wSetValue;									//	세트 갯수
	WORD wApplicationValue;							//  이펙트 적용 세트 값
};

struct SET_ITEM_INFO
{
	WORD wKind;
	WORD wCount;
};
///////////////////////////////////////////////////////////////////////////

//!//
struct PET_NAME
{
	WORD PetIdx;			//		Item_Index	
	char ItemName[MAX_PETNAME_LENGTH+1];		//		Item_name

};
/*
class ITEM_REINFORCE_INFO
{
public:
	WORD			wItemIdx;
	char			szItemName[MAX_ITEMNAME_LENGTH+1];
	LEVELTYPE		ItemLevel;
	WORD			ReinforceType;
	float			w;
};
*/


//---아아템강화
struct sITEM_REINFORCE_INFO
{
	WORD			wItemIdx;
	WORD			ReinforceType;
	float			fMaxReinforce;
	float			fw;
};
//-----------------------------------
struct sITEM_RAREREINFORCE_INFO	: public sITEM_REINFORCE_INFO
{
	WORD			wMaterialGravity;
};

//---아아템분해
struct sITEM_DISSOLUTIONITEM
{
	WORD			wUnitItemIdx;					//분해될 아이템 인덱스
	WORD			wUnitItemNum;						//분해될 아이템 개수
};

struct sITEM_DISSOLUTION_INFO
{
	WORD					wDissolutionItemIdx;	//분해할 베이스아이템 인덱스
	WORD					wUnitKindNum;			//분해될 아이템 종류 개수
	WORD					wUnitTotalNum;			//분해될 아이템 총 개수
	sITEM_DISSOLUTIONITEM*	pUnitItem;				//분해아이템 인포
};
//-----------------------------------



class ITEM_MIX_INFO
{
public:
	WORD			wItemIdx;
	char			szItemName[MAX_ITEMNAME_LENGTH+1];
	
	WORD			wResultItemNum;
	ITEM_MIX_RES	* psResultItemInfo;
};

class ITEM_MIX_RES
{
public:
	WORD		wResItemIdx;
	char		szResItemName[MAX_ITEMNAME_LENGTH+1];
	WORD		wMixKind;
	MONEYTYPE	Money;
	WORD		SuccessRatio;
	WORD		wMaterialItemNum;
	ITEM_MIX_MATERIAL * psMaterialItemInfo;
};

class ITEM_MIX_MATERIAL
{
public:
	WORD		wMatItemIdx;
	char		szMatItemName[MAX_ITEMNAME_LENGTH+1];
	WORD		wMatItemNum;
};

struct HILEVEL_ITEM_MIX_RATE_INFO	// +10템 조합 기능 추가 작업 by Stiner(2008/06/11)-10+ItemMix
{
	WORD	wItemLevel;
	DWORD	dwBaseRate;
	WORD	dwModulate;
};

// magi82 - Titan(070129)
struct TITAN_PARTS_KIND
{
	DWORD	dwPartsIdx;
	DWORD	dwPartsKind;
	DWORD	dwResultTitanIdx;
};

struct TITAN_STATSINFO
{
	DWORD	titanIdx;						// 타이탄인덱스
	DWORD	titanType;						// 타입
	DWORD	minAtt;							// 최소공격력
	DWORD	maxAtt;							// 최대공격력
	DWORD	woigongCri;						// 외공일격
	DWORD	naegongCri;						// 내공일격
	DWORD	minlongAtt;						// 최소원거리공격력
	DWORD	maxlongAtt;						// 최대원거리공격력
	DWORD	distance;						// 사정거리
	DWORD	Life;							// 생명력
	DWORD	defense;						// 방어력
	DWORD	Mana;							// 내력
	DWORD	Hosin;							// 호신강기
	WORD	AttrFire;						// 화
	WORD	AttrWater;						// 수
	WORD	AttrTree;						// 목
	WORD	AttrIron;						// 금
	WORD	AttrEarth;						// 토
};
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070131)
struct TITAN_UPGRADE_MATERIAL
{
    DWORD dwIndex;
	DWORD dwCount;
};

struct TITAN_UPGRADEINFO
{
    DWORD	dwTitanIdx;
	DWORD	dwNextTitanIdx;
	DWORD	dwMoney;
	WORD	wMaterial_Num;
	TITAN_UPGRADE_MATERIAL* pTitanUpgradeMaterial;

	TITAN_UPGRADEINFO::TITAN_UPGRADEINFO()
	{
		pTitanUpgradeMaterial = NULL;
	}
	TITAN_UPGRADEINFO::~TITAN_UPGRADEINFO()
	{
		if( pTitanUpgradeMaterial )
		{
			delete [] pTitanUpgradeMaterial;
			pTitanUpgradeMaterial = NULL;
		}
	}
};
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070205)
struct TITAN_BREAK_MATERIAL
{
	DWORD	dwMaterialIdx;
	WORD	wMaterCnt;
	WORD	wRate;
};

struct TITAN_BREAKINFO
{
	DWORD	dwIdx;
	DWORD	dwMoney;
	WORD	wTotalCnt;
	WORD	wGetCnt;
	TITAN_BREAK_MATERIAL* pTitanBreakMaterial;

	TITAN_BREAKINFO::TITAN_BREAKINFO()
	{
		dwIdx = 0;
		wTotalCnt = 0;
		wGetCnt = 0;
		pTitanBreakMaterial = NULL;
	}
	TITAN_BREAKINFO::~TITAN_BREAKINFO()
	{
		if( pTitanBreakMaterial )
		{
			delete [] pTitanBreakMaterial;
			pTitanBreakMaterial = NULL;
		}
	}
};

struct BobusangPosPerMap
{
	VECTOR3	ApprPos;
	float	ApprDir;
};
//////////////////////////////////////////////////////////////////////////

// magi82 - UniqueItem(070625)
struct UNIQUE_ITEM_OPTION_INFO
{
	DWORD	dwItemIdx;								// 유니크 아이템 인덱스
	char	strManEffectName[MAX_CHXNAME_LENGTH];	// 유니크 아이템 이펙트 파일 이름(남자)
	char	strWomanEffectName[MAX_CHXNAME_LENGTH];	// 유니크 아이템 이펙트 파일 이름(여자)
	BYTE	ExchangeFlag;							// 거래 가능 유무
	BYTE	MixFlag;								// 조합 가능 유무
	DWORD	dwCurseCancellation;					// 저주해제된 아이템의 인덱스

	// HP(+/-)
	int		nHp;
	// MP(+/-)
	int		nMp;
	// 물리데미지(+/-)
	int		nPhyDamage;
	// 일격데미지(+/-)
	int		nCriDamage;
	// 일격확률(+/-)
	int		nCriRate;
	// 근골(+/-)
	int		nGengol;
	// 민첩(+/-)
	int		nMinChub;
	// 체력(+/-)
	int		nCheRyuk;
	// 심맥(+/-)
	int		nSimMek;
	// 방어력(+/-)
	int		nDefen;
	// 사정거리(+/-)
	int		nRange;
	// 속성공격(+/-)
	int		nAttR;
	// 적방어력 감소
	int		nEnemyDefen;
	// 호신강기(+/-)
	int		nShield;
    
	UNIQUE_ITEM_OPTION_INFO::UNIQUE_ITEM_OPTION_INFO()
	{
		dwItemIdx			= 0;
		ZeroMemory(strManEffectName, sizeof(strManEffectName));
		ZeroMemory(strWomanEffectName, sizeof(strWomanEffectName));
		ExchangeFlag		= 0;
		MixFlag				= 0;

		nHp					= 0;
		nMp					= 0;
		nPhyDamage			= 0;
		nCriDamage			= 0;
		nCriRate			= 0;
		nGengol				= 0;
		nMinChub			= 0;
		nCheRyuk			= 0;
		nSimMek				= 0;
		nDefen				= 0;
		nRange				= 0;
		nAttR				= 0;
		nEnemyDefen			= 0;
		nShield				= 0;
	}
};

struct UNIQUE_ITEM_MIX_MATERIAL_INFO
{
	DWORD	dwMaterialIdx;
	WORD	wMaterialNum;
};

struct UNIQUE_ITEM_MIX_RESULT_INFO
{
	DWORD	dwResultIdx;
	WORD	wResultRate;
};

struct UNIQUE_ITEM_MIX_INFO 
{
	DWORD	dwItemIdx;
	WORD	wMaterialKindNum;
	
	UNIQUE_ITEM_MIX_MATERIAL_INFO*	sUniqueItemMixMaterial;
	UNIQUE_ITEM_MIX_RESULT_INFO		sUniqueItemMixResult[UNIQUE_ITEM_MIX_RESULT_INDEX];
};

//// 2007. 12. 6. CBH - 스킨 SETITEM 옵션 관련 구조체 추가 ////////////////////////////////////////////
struct SKIN_SELECT_ITEM_INFO
{
	DWORD dwIndex;							// 스킨 인덱스
	char szSkinName[MAX_ITEMNAME_LENGTH+1];	//스킨 이름
	DWORD dwLimitLevel;						// 장착 레벨
	WORD wEquipItem[SKINITEM_LIST_MAX];		//스킨 부위별 아이탬 인덱스 배열

	SKIN_SELECT_ITEM_INFO::SKIN_SELECT_ITEM_INFO()
	{
		dwIndex = 0;	//초기 값은 -1로 해준다. 선택한 컨트롤 리스트의 반환값과 맞추기위해 설정
		dwLimitLevel = 0;
		memset( wEquipItem, 0, sizeof(WORD)*SKINITEM_LIST_MAX );		
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------------------------------------//
//		클라이언트만 사용하는 부분
#ifdef _CLIENT_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//

typedef struct _PRELOAD
{
	StaticString FileName;
}PRELOAD;



struct SETSHOPITEM
{
	DWORD ItemIdx;
	char Name[MAX_ITEMNAME_LENGTH+1];
};


#ifdef _TESTCLIENT_

//trustpak
struct TESTCHARACTER_INFO
{
	BYTE byGender;
	float fX;
	float fY;
	float fZ;
	float fAngle;
};

#define MAX_TEST_CHARACTER_NO 8
//


struct TESTCLIENTINFO {
	TESTCLIENTINFO()
	{
		//trustpak
		memset(this, 0, sizeof(TESTCLIENTINFO));
		//

		LightEffect = 1;
		Effect = 0;
		MugongIdx = 0;
		WeaponIdx = MIN_EQUIPMENTITEM_INDEX+1;
		Gender = GENDER_MALE;
		x = 30000.f;
		z = 30000.f;
		MonsterEffect = 1;
		MonsterNum = 1;
		MonsterKind = 13;
		Map = 17;

		DressIdx = 0;
		HatIdx = 0;
		ShoesIdx = 0;
		HairType = 0;
		FaceType = 0;
		

		CharacterNum = 0;
		BossMonsterNum = 0;
		BossMonsterKind = 1000;
		
		memset( &titaninfo, 0, sizeof(titaninfo) );
		titaninfo.TitanGrade = 1;
		titaninfo.TitanKind = 1;
		titaninfo.TitanScale = 100;
	}
	float x,z;

	BOOL bInTitan;

	WORD DressIdx;
	WORD HatIdx;
	WORD ShoesIdx;

	BYTE HairType;
	BYTE FaceType;

	WORD MonsterKind;
	BOOL bMonsterDie;

	WORD BossMonsterKind;
	WORD BossMonsterNum;
	char BossInfofile[17];

	int Effect;
	WORD MugongIdx;
	WORD WeaponIdx;
	BYTE Gender;
	int MonsterEffect;
	WORD MonsterNum;
	WORD CharacterNum;
	MAPTYPE Map;

	BOOL LightEffect;
	
	float Width;
	float Height;

	//trustpak
	DWORD				dwTestCharacterNo;
	TESTCHARACTER_INFO	aTestCharacterInfo[MAX_TEST_CHARACTER_NO];
	
	//
	TITAN_APPEARANCEINFO	titaninfo;

	int nTestEffectNum;
};
#endif

//-----------------------------------------------------------------------
// game config 정보 
//-----------------------------------------------------------------------
struct GAMEDESC_INI
{
	GAMEDESC_INI()
	{
		strcpy(DistributeServerIP,"211.233.35.36");
		DistributeServerPort = 400;

		AgentServerPort = 100;
		
		bShadow = MHSHADOW_CIRCLE;
		
		dispInfo.dwWidth = 1024;		//WINDOW SIZE
		dispInfo.dwHeight = 768;
		dispInfo.dwBPS = 4;
		dispInfo.dispType = WINDOW_WITH_BLT;
		//dispInfo.bWindowed = TRUE;
		dispInfo.dwRefreshRate = 70;

		MaxShadowTexDetail = 256;
		MaxShadowNum = 4;

		FramePerSec = 30;
		TickPerFrame = 1000/(float)FramePerSec;

		MasterVolume = 1;
		SoundVolume = 1;
		BGMVolume = 1;
		
		CameraMinAngleX = 0;
		CameraMaxAngleX = 89.f;
		CameraMinDistance = 200.f;
		CameraMaxDistance = 1000.f;

		LimitDay = 0;
		LimitID[0] = 0;
		LimitPWD[0] = 0;

		strWindowTitle[0] = 0;
	}

	float MasterVolume;
	float SoundVolume;
	float BGMVolume;

	char DistributeServerIP[32];
	WORD DistributeServerPort;
	
	WORD AgentServerPort;
	
	DISPLAY_INFO dispInfo;

	BYTE bShadow;
	DWORD MaxShadowNum;
	DWORD MaxShadowTexDetail;

	DWORD FramePerSec;
	float TickPerFrame;

	char MovePoint[64];

	float CameraMinAngleX;
	float CameraMaxAngleX;
	float CameraMinDistance;
	float CameraMaxDistance;

	DWORD LimitDay;
	char LimitID[MAX_NAME_LENGTH+1];
	char LimitPWD[MAX_NAME_LENGTH+1];

	char strWindowTitle[128];
};

struct SEVERLIST
{
	char	DistributeIP[16];
	WORD	DistributePort;
	char	ServerName[64];
	WORD	ServerNo;
	BOOL	bEnter;

	SEVERLIST()
	{
		ServerNo = 1;
		strcpy( DistributeIP, "211.233.35.36" );
		DistributePort = 400;
		strcpy( ServerName, "Test" );
		bEnter = TRUE;
	}
};


//-----------------------------------------------------------------------------------------------------------//
#endif //_CLIENT_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//


//-----------------------------------------------------------------------------------------------------------//
//		서버만 사용하는 부분
#ifdef _SERVER_RESOURCE_FIELD_

//-----------------------------------------------------------------------------------------------------------//
#include "ServerTable.h"
//-----------------------------------------------------------------------
// 캐릭터 IN/OUT POINT 설정
//-----------------------------------------------------------------------
typedef struct _CHARACTERINOUTPOINT
{
	WORD MapNum;
	VECTOR3 MapInPoint[MAX_MAP_NUM];
	VECTOR3 MapOutPoint[MAX_MAP_NUM];
}CHARACTERINOUTPOINT;


//-----------------------------------------------------------------------------------------------------------//
#endif //_SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
#endif //__GAMERESOURCESTRUCT_H__
