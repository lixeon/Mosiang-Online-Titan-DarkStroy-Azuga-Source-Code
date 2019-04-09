// MiniMapDlg.h: interface for the CMiniMapDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINIMAPDLG_H__72DF8430_5EB4_480A_B4D4_06BB23A02329__INCLUDED_)
#define AFX_MINIMAPDLG_H__72DF8430_5EB4_480A_B4D4_06BB23A02329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "MiniMapHeroIcon.h"
#include "cImageSelf.h"

class cStatic;
class cPushupButton;

struct MINIMAPIMAGE
{
	cImageSelf pMiniMapImage;
	DWORD ImageWidth, ImageHeight;
};

//SW050802 snake_legs
struct stParticle
{
	VECTOR2 vPos;
	VECTOR2 vDir;
	stParticle()
	{
		vPos.x = vPos.y = 0;
		vDir.x = vPos.y = 0;
	}
};

struct stNumImage	//SW050722 for jackpot number dialog
{
	cImage* pImage;
	DWORD dwW,dwH;
	stNumImage():pImage(NULL),dwW(0),dwH(0){}
};

struct stCipherNum	//SW050722 for jackpot number dialog
{
	stCipherNum():dwNumber(99),dwRealCipherNum(0),bIsAni(FALSE){}
	DWORD dwNumber;// 자리수 숫자 (출력용)
	DWORD dwRealCipherNum;// 자리수 숫자 (실제값)
	bool bIsAni;// 숫자 애니 진행 중인지..(애니용)
};


class CMiniMapDlg : public cDialog  
{
//	DWORD TempID;
	
	cStatic* m_pMapName;
	cStatic* m_pPosXY;	

	cPushupButton* m_pZoom;	

	MAPTYPE m_MapNum;

	CMiniMapHeroIcon m_HeroIcon;	
	CYHHashTable<CMiniMapIcon> m_IconTable;
	CMemoryPoolTempl<CMiniMapIcon> m_IconPool;
	CMiniMapIconImage m_pIconImage[eMNMICON_MAX];
	
	int m_CurMode;
	MINIMAPIMAGE m_MinimapImage[eMINIMAPMODE_MAX];

//	cImage m_HeroImage;

//KES
	cImage m_TooltipImage;
//	
	float m_DisplayMinX,m_DisplayMinZ;		
	float m_DisplayMaxX,m_DisplayMaxZ;		
	VECTOR2 m_DiplayOriginPosOfImage;
	VECTOR2 m_ImageDisplayWH;

	void InitIconImage();

	void SetZoomRate(float zoomrate);

	CMiniMapIcon* GetEmptyMiniMapIcon(WORD Kind,CObject* pObject);
	CMiniMapIcon* GetEmptyMiniMapIcon(WORD Kind,PARTY_MEMBER* pMember);
	void ReleaseMinniMapIcon(CMiniMapIcon* pIcon);

	void LoadMinimapImageInfo(MAPTYPE MapNum);

	void AddIcon(WORD Kind,CObject* pObject);
	void AddIcon(WORD Kind,PARTY_MEMBER* pMember);

//SW050722 
#define NUMIMAGE_W 8
#define NUMIMAGE_H 14

#define BASIC_ANI_TIMELENGTH	2000
#define BETWEEN_ANI_TIMELENGTH	500		//interval between Numbers
#define NUM_CHANGE_TIMELENGTH	10		//interval
#define	DOT_NUM					2

	enum{DEFAULT_IMAGE = 99, NUM_0 = 0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, IMG_DOT, IMAGE_COUNT, CIPHER_NUM = 9};

	stNumImage	m_stNumImage[IMAGE_COUNT];	// 숫자 이미지
	VECTOR2		m_vPos[CIPHER_NUM];		// 숫자창 자리수 위치
	VECTOR2		m_vDotPos[DOT_NUM];		//SW050805	자리수 구분점 위치 추가

	DWORD		m_dwTotalMoney;			// 총금액	// 출력을 위한 가변 금액
	DWORD		m_dwOldTotalMoney;			// 저장 금액(변화비교) // 비교를 위한 실제 금액
	DWORD		m_dwTempMoney;			// @사족	//

	stCipherNum m_stCipherNum[CIPHER_NUM];	// 각 자리별 출력할 숫자
	DWORD		m_dwAniStartTime;		// 숫자 애니 시작 시간
	DWORD		m_dwNumChangeTime;		// 자리 애니 변환 간격	// 숫자 갱신 간격
	DWORD		m_dwIntervalAniTime;	// 자리간 애니 변환 간격 // 먼저 자리 애니가 끝나면 ..지나 다음 자리 애니 종료 하는 식~
	DWORD		m_dwMaxCipher;			// 금액 총 자리수 (애니용)
	DWORD		m_dwCipherCount;		// 자리수 (애니용)
	bool		m_bIsAnimationing;			// 전체 애니 진행 중인지..(애니용)
	bool		m_bDoSequenceAni;			// ForSequenceAni

/*	//SW050802 snake_legs
#define RND_RANGE	10
#define PARTICLE_PER_NUM 36
#define GRAVITY	0.5f
	stParticle	m_stPtc[CIPHER_NUM][36];
	DWORD	m_BeforeTime;*/
public:
	CMiniMapDlg();
	virtual ~CMiniMapDlg();
//KES
	virtual DWORD ActionEvent( CMouse* mouseInfo );
//	
	void InitMiniMap(MAPTYPE MapNum);
	
	void Linking();
	virtual void SetActive( BOOL val );

	void ToggleMinimapMode();
	
	void AddMonsterIcon(CObject* pObject);
	void AddBossMonsterIcon(CObject* pObject);
	void AddStaticNpcIcon(CObject* pObject);

	void AddPartyMemberIcon(PARTY_MEMBER* pMember);
	void AddPartyMasterIcon(CObject* pObject);

	void AddQuestIcon( CObject* pTarget, int nKind );
	
	void AddHeroIcon(CObject* pObject);
//	void SetPartyIcon();

	void RemoveIcon(DWORD ObjectID);

	void FitToCenterIcon(CMiniMapIcon* pIcon);
	void Process();
	void Render();

	void SetPartyIconTargetPos(DWORD PlayerID, int posX, int posZ);
	void SetPartyIconObjectPos(DWORD PlayerID, int posX, int posZ);
	
	int GetCurMode()	{ return m_CurMode; }
	
	BOOL CanActive();

//	CMiniMapIconImage* GetMiniMapImage(WORD Kind) {	return &m_pIconImage[Kind]; }

	void RefreshMode();
	void ShowQuestMarkIcon( CObject* pTarget, int nKind );

//SW050722 
	void InitNumImage();				// 숫자 이미지 등록
	void ReleaseNumImage();				// 숫자 이미지 해제
	void SetNumImagePos();				// 숫자 이미지 출력 좌표 지정	
	void ConvertCipherNum();			// 단위별 숫자
	bool IsNumChanged();				// 숫자 변화 체크	// Check Num charnges
//	void DoNumChangeAni();				// 숫자 변환 애니	// Ticktime control
	void DoAni();
	void DoSequenceAni();
	void JPRender();
	void JPProcess();

	//SW050802 snake_legs
/*	void SetParticlePos();
	void CalcMotion();*/
	
	
	void InitForAni();
	void InitForSequenceAni();
	void JPInit();
};


#endif // !defined(AFX_MINIMAPDLG_H__72DF8430_5EB4_480A_B4D4_06BB23A02329__INCLUDED_)
