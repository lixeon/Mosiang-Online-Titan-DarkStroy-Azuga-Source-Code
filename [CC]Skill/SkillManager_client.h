// SkillManager.h: interface for the CSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLMANAGER_H__77A8371D_54A8_40CF_AEBC_7D8C21051312__INCLUDED_)
#define AFX_SKILLMANAGER_H__77A8371D_54A8_40CF_AEBC_7D8C21051312__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillInfo.h"
#include "SkillObject_Client.h"
#include "SkillAreaManager.h"

class CHero;
class CActionTarget;

//#define SKILLMGR USINGTON(CSkillManager)
#define SKILLMGR	CSkillManager::GetInstance()

class CSkillManager  
{
	CYHHashTable<CSkillInfo>	m_SkillInfoTable;
//	CYHHashTable<CSkillInfo>	m_DummySkillInfoTable;
	CYHHashTable<CSkillObject>	m_SkillObjectTable;
	cPtrList m_SkillChangeList;

	CSkillInfo* m_pSkillInfo;	// 마지막으로 사용한 무공 정보

	CSkillAreaManager m_SkillAreaMgr;

	// 화경, 극마
	CYHHashTable<CSkillInfo>	m_HwaKyungSkillTable;
	CYHHashTable<CSkillInfo>	m_GeukMaSkillTable;

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	CYHHashTable<SKILLOPTION>	m_SkillOptionTable;
	CYHHashTable<SKILLOPTION>	m_SkillOptionByItemTable;
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안	
	CYHHashTable<SPECIAL_STATE_INFO> m_SpecialStateInfoTable;
	//////////////////////////////////////////////////////////////////////////

	////////// 2007. 6. 28. CBH - 전문기술 확률 리스트 //////////////////////
	CYHHashTable<JOB_SKILL_PROBABILITY_INFO>	m_JobSkillProbabilityTable;
	/////////////////////////////////////////////////////////////////////////

	// 시전 연출
	void SkillStartEffect(CObject* pObject,CSkillInfo* pSkillInfo);
	BOOL HeroSkillStartEffect(CHero* pHero,CActionTarget* pTarget,CSkillInfo* pSkillInfo);
	
	// SkillObject 등록 및 해제
	CSkillObject* CreateSkillObject(MSG_SKILLOBJECT_ADD* pSkillObjectAddInfo);
	CSkillObject* CreateTempSkillObject(CSkillInfo* pSkillInfo,CHero* pHero,CActionTarget* pTarget);
	void ChangeTempObjToRealObj(MSG_SKILLOBJECT_ADD* pSkillObjectAddInfo);

	void DoCreateSkillObject(CSkillObject* pSObj, SKILLOBJECT_INFO* pSOInfo,CTargetList* pTList);

	void ReleaseSkillObject(CSkillObject* pObject);
	
public:
	CSkillManager();
	 ~CSkillManager();
//	MAKESINGLETON(CSkillManager);

	GETINSTANCE(CSkillManager);

	void Init();
	void Release();

	void LoadSkillInfoList();
	void LoadSkillTreeList();
	void LoadSkillChangeInfoList();
	void LoadJobSkillList();
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	void LoadSkillOptionList();
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안
	void LoadStateList();
	//////////////////////////////////////////////////////////////////////////

	CSkillObject* GetSkillObject(DWORD SkillObjectID);
	CSkillInfo*	GetSkillInfo(WORD SkillInfoIdx);
	CSkillInfo*	GetDummySkillInfo(WORD SkillInfoIdx);
	SKILL_CHANGE_INFO * CSkillManager::GetSkillChangeInfo(WORD wMugongIdx);
	
	WORD GetSkillTooltipInfo(WORD SkillInfoIdx);
	WORD GetComboSkillIdx(CHero* pHero);
//	WORD GetMugongSkillIdx(CHero* pHero);
	void SetSkill(CSkillInfo* pSkillInfo) { m_pSkillInfo = pSkillInfo; }

	CSkillArea* GetSkillArea(CObject* pObject, CActionTarget* pTarget, CSkillInfo* pSkillInfo);
	CSkillArea* GetSkillArea(DIRINDEX dir,WORD SkillAreaIndex);
	void ReleaseSkillArea(CSkillArea* pSkillArea);
	
	void GetMultiTargetList(CSkillInfo* pSkillInfo,CHero* pHero,CActionTarget* pTarget);

	BOOL IsDeadlyMugong(WORD wMugongIdx);
	
	// 스킬을 사용
	BOOL ExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,BOOL bMugong);
	BOOL ExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,CSkillInfo* pSkillInfo);
	BOOL ExcuteTacticSkillSYN(CHero* pHero,TACTIC_TOTALINFO* pTInfo,BYTE OperNum);

	BOOL RealExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,CSkillInfo* pSkillInfo);

		
	// 다음 콤보 스킬을 지정
	void SetNextComboSkill(CHero* pHero,CActionTarget* pTarget,BOOL bMugong);

	// 이벤트 핸들 함수들
	BOOL OnSkillCommand(CHero* pHero,CActionTarget* pTarget,BOOL bMugong);
	void OnComboTurningPoint(CHero* pHero);
	void OnExcuteSkillNACKed(SKILLOBJECT_INFO* pInfo);
	
	// 스킬오브젝트의 타겟 리스트들을 변경한다.
	void UpdateSkillObjectTargetList(CObject* pObject);
	
	void RemoveTarget(CObject* pObject,BYTE bTargetKind);
	
	//
	CSkillInfo* IsHwaKungSkill( DWORD SkillIdx )		{	return m_HwaKyungSkillTable.GetData( SkillIdx );	}
	CSkillInfo* IsGeukMaSkill( DWORD SkillIdx )			{	return m_GeukMaSkillTable.GetData( SkillIdx );	}


	void OnReceiveSkillObjectAdd(MSG_SKILLOBJECT_ADD* pmsg);
	void OnReceiveSkillObjectRemove(MSG_DWORD* pmsg);
	void OnReceiveSkillStartNack();
	void OnReceiveSkillSingleResult(MSG_SKILL_SINGLE_RESULT* pmsg);
	void OnReceiveSkillObjectTargetListAdd(DWORD SkillObjectID,DWORD AddedObjectID,BYTE bTargetKind);
	void OnReceiveSkillObjectTargetListRemove(DWORD SkillObjectID,DWORD RemovedObjectID);
	void OnReceiveSkillStartEffect(DWORD SkillOperator,DWORD SkillIdx);
	void OnReceiveSkillObjectOperate(MSG_SKILL_OPERATE* pmsg);

	void NetworkMsgParse(BYTE Protocol,void* pMsg);

	void MakeSAT();

	//////////////////////////////////////////////////////
	// 06. 04. 스킬 버그 수정 - 이영준
	// 임시스킬객체를 강제로 지워주는 함수
	void DeleteTempSkill();
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	SKILLOPTION* GetSkillOption(WORD index) { return (SKILLOPTION*)(m_SkillOptionTable.GetData(index)); }
	SKILLOPTION* GetSkillOptionByItemIndex(WORD index) { return (SKILLOPTION*)(m_SkillOptionByItemTable.GetData(index)); }
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안
	SPECIAL_STATE_INFO* GetSpecialStateInfo(WORD index) { return (SPECIAL_STATE_INFO*)(m_SpecialStateInfoTable.GetData(index)); }
	//////////////////////////////////////////////////////////////////////////

	////////// 2007. 6. 28. CBH - 전문기술 확률 관련 함수 추가 ////////////////////
	BOOL LoadJobSkillProbability();
	BOOL IsJobSkill(CHero* pHero,CActionTarget* pTarget, CSkillInfo* pSkillInfo);
	BOOL CheckSkillKind(WORD wSkillKind);
	///////////////////////////////////////////////////////////////////////////////

	BOOL CheckTitanWeapon(CHero* pHero, SKILLINFO* SkillInfo);	//2007. 10. 15. CBH - 타이탄 무공과 무기 체크(SkillInfo에서 이동)

	//debug용
	int	m_nSkillUseCount;
};
//EXTERNGLOBALTON(CSkillManager);
#endif // !defined(AFX_SKILLMANAGER_H__77A8371D_54A8_40CF_AEBC_7D8C21051312__INCLUDED_)
