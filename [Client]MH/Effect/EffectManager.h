// EffectManager.h: interface for the CEffectManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTMANAGER_H__C97B73EA_067D_41CA_A990_2459F04ED62B__INCLUDED_)
#define AFX_EFFECTMANAGER_H__C97B73EA_067D_41CA_A990_2459F04ED62B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef DWORD HEFFPROC;
class CObject;
class CEffect;
class CDamageNumber;
class CEngineObject;
class CPlayer;

#include "PtrList.h"
#include "EffectDescManager.h"

#define EFFECTMGR USINGTON(CEffectManager)

#define EFFECT_FLAG_NORMAL				0x00000000
#define EFFECT_FLAG_HEROATTACK			0x00000001
#define EFFECT_FLAG_FATALDAMAGE			0x00000004
#define EFFECT_FLAG_LINKTOOBJECT		0x00000008
#define EFFECT_FLAG_ABSOLUTEPOS			0x00000010
#define EFFECT_FLAG_DIE					0x00000020
#define EFFECT_FLAG_GENDERBASISTARGET	0x00000040
#define EFFECT_FLAG_RELATEDTOHERO		0x00000080


#define EFFECT_OPTION_ALL				0
#define EFFECT_OPTION_ONLYHERO			1
#define EFFECT_OPTION_NONE				2

extern int g_EffectOption;

enum eEffectDescKind
{
	eEffectForMan = 0,
	eEffectForWoman,
		
	eEffect_Max,
};
class CEffectManager  
{
	CYHHashTable<void>	m_IllisionEffectMaterialTable;

	CMemoryPoolTempl<CEffect>* m_pEffectPool;
	CMemoryPoolTempl<CDamageNumber>* m_pDNPool;
	BOOL m_bInited;

	CEffectDescManager m_EffectDescMgr[eEffect_Max];
	
	CYHHashTable<CEffect> m_EffectProcessTable;

	cPtrList m_DamageNumberList;
	
	DWORD m_CurNewEffectID;
	
	DWORD GetNewEffectID();

	void EndProcess(CEffect* pEffect);
	void ResetProcess(CEffect* pEffect);
	
	void AddDamageNumber(CDamageNumber* pNumber);
public:

//	MAKESINGLETON(CEffectManager);
	CEffect* GetEffect(HEFFPROC heff);

	CEffectManager();
	virtual ~CEffectManager();

	void Init();
	void Release();
	void RemoveAllEffect();

	void LoadIllusionMaterial();
	void* GetIllusionMaterial(DWORD MaterialNum);

	HEFFPROC StartEffectProcess(int EffectDescKnd,int EffectNum,EFFECTPARAM* pParam,DWORD Key,DWORD RefCount);

	//SW050913 수정
	HEFFPROC StartEffectProcess(int EffectNum,CObject* pEffectOperator,
		TARGETSET* pEffectTargetSetArray,int nEffectTargetNum, DWORD MainTarget,
		DWORD dwFlag = EFFECT_FLAG_NORMAL,VECTOR3* pAddPos = NULL, CObject* pSkillOperator = NULL );	

	HEFFPROC StartHeroEffectProcess(int EffectNum);

	void OnEffectTimeOut(HEFFPROC handle);
	BOOL ForcedEndEffect(HEFFPROC handle);

	void Process(DWORD CurTime);
	void Process(DWORD CurTime,HEFFPROC handle);
	BOOL SetDamage(HEFFPROC handle,TARGETSET* TargetSetArray,int MaxTargetSet,MAINTARGET* pMainTarget);
	
	void AddDamageNumber(DWORD Damage,CObject* pOperator,CObject* pTarget,WORD DamageKind,BOOL bCritical, BOOL bDecisive);
	void AddDamageNumberDodge(CObject* pTarget);
	
	DWORD GetEffectEndTime(HEFFPROC heff);
	void SetEffectEndFlag(HEFFPROC heff);
	
	DWORD GetOperatorAnimatioEndTime(int EffectNum,int EffectGenderKind,CEngineObject* pEngineObject);

	//////////////////////////////////////////////////////////////////////////
	// TargetList 적용
	HEFFPROC StartEffectProcessWithTargetList(int EffectNum,CObject* pEffectOperator,
		CTargetList* pTList, MAINTARGET* pMainTarget,
		DWORD dwFlag = EFFECT_FLAG_NORMAL,VECTOR3* pAddPos = NULL);
	BOOL SetTargetArray(HEFFPROC handle,CTargetList* pTList,MAINTARGET* pMainTarget);

	//SW050913 수정
	HEFFPROC StartSkillMainTainEffect(int EffectNum,CObject* pOperator,DWORD dwFlag = EFFECT_FLAG_NORMAL,VECTOR3* pAddPos = NULL,CObject* pSkillOperator = NULL );

	void SetEndFlagAllEffect();
	
	void IncEffectRefCount(HEFFPROC heff);

	//
	HEFFPROC StartPlayerEffectProcess(CPlayer* pPlayer, int EffectNum);

};
EXTERNGLOBALTON(CEffectManager);
#endif // !defined(AFX_EFFECTMANAGER_H__C97B73EA_067D_41CA_A990_2459F04ED62B__INCLUDED_)
