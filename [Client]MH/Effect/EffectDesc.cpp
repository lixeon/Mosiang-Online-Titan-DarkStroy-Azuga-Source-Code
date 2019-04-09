// EffectDesc.cpp: implementation of the CEffectDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectDesc.h"
#include "GameResourceManager.h"

#include "EffectUnitDesc.h"
#include "Effect.h"

#include "..\MHFile.h"
#include "LightEffectUnitDesc.h"
#include "ObjectEffectUnitDesc.h"
#include "AnimationEffectUnitDesc.h"
#include "DamageEffectUnitDesc.h"
#include "CameraEffectUnitDesc.h"
#include "SoundEffectUnitDesc.h"
#include "MoveEffectUnitDesc.h"


#include "EffectTrigger.h"
#include "EffectTriggerOnUnitDesc.h"
#include "EffectTriggerOffUnitDesc.h"
#include "EffectTriggerMoveUnitDesc.h"
#include "EffectTriggerAttachUnitDesc.h"
#include "EffectTriggerDetachUnitDesc.h"
#include "EffectTriggerCameraRotateUnitDesc.h"
#include "EffectTriggerCameraAngleUnitDesc.h"
#include "EffectTriggerCameraZoomUnitDesc.h"
#include "EffectTriggerChangeCameraUnitDesc.h"
#include "EffectTriggerSetBaseMotionUnitDesc.h"
#include "EffectTriggerCameraShakeUnitDesc.h"
#include "EffectTriggerLinkUnitDesc.h"
#include "EffectTriggerFadeOutUnitDesc.h"
#include "EffectTriggerAnimateUnitDesc.h"
#include "EffectTriggerIllusionUnitDesc.h"
#include "EffectTriggerGravityMoveUnitDesc.h"


#include "..\Engine\EngineObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectDesc::CEffectDesc()
{
	m_MaxEffectUnitDesc = 0;
	m_MaxEffectTriggerDesc = 0;
	m_ppEffectUnitDescArray = NULL;
	m_ppEffectTriggerUnitDesc = NULL;
	m_EffectEndTime = 10000;
	m_bRepeat = FALSE;
	m_NextEffect = 0;
	m_OperatorAnimationTime = 0;
}

CEffectDesc::~CEffectDesc()
{
	if(m_ppEffectUnitDescArray)
	{
		for(DWORD n=0;n<m_MaxEffectUnitDesc;++n)
		{
			if(m_ppEffectUnitDescArray[n])
				delete m_ppEffectUnitDescArray[n];
		}
		delete [] m_ppEffectUnitDescArray;
	}
	if(m_ppEffectTriggerUnitDesc)
	{
		for(DWORD n=0;n<m_MaxEffectTriggerDesc;++n)
		{
			if(m_ppEffectTriggerUnitDesc[n])
				delete m_ppEffectTriggerUnitDesc[n];
		}
		delete [] m_ppEffectTriggerUnitDesc;
	}
}

BOOL CEffectDesc::LoadEffectDesc(int EffectKind,CMHFile* pFile)
{
	m_EffectKind = EffectKind;

	if(pFile->IsInited() == NULL)
	{
		m_MaxEffectUnitDesc = 0;
		m_MaxEffectTriggerDesc = 0;
		
		return FALSE;
	}
	m_FileName = pFile->GetFileName();

	char buf[128];
	char buf2[128];
	char cmd[128];
	DWORD time;
	DWORD unitnum;
	CEffectUnitDesc* pUnit;
	DWORD TriggerNum = 0;

	//////////////////////////////////////////////////////////////////////////
	// Damage Percent 검증용
	BOOL bDamageUnit = FALSE;
	float Percent = 0;
	while(1)
	{
		if(pFile->IsEOF())
			break;
		
		pFile->GetString(buf);
		
		CMD_ST(buf)
			CMD_CS("#REPEAT")
				m_bRepeat = pFile->GetBool();				

			CMD_CS("#NEXTEFFECT")
				m_NextEffect = FindEffectNum(pFile->GetString());			

			CMD_CS("#EFFECTENDTIME")
				pFile->GetString(buf2);
				if(buf2[0] == 'f' || buf2[0] == 'F')
				{
					if(strlen(buf2) <= 1)
					{
						MessageBox(NULL,"f다음에 숫자를 붙여써주세요!!!!",NULL,NULL);//pjslocal
						return FALSE;
					}

					float frame = (float)atof(&buf2[1]);
					float temp = GAMERESRCMNGR->m_GameDesc.TickPerFrame;
					time = (DWORD)(temp * frame);		// 프레임일경우 시간으로 변환
				}
				else
				{
					time = atoi(buf2);
				}
				m_EffectEndTime = time;

			CMD_CS("#MAXEFFECTUNIT")
				m_MaxEffectUnitDesc = pFile->GetDword();
				m_ppEffectUnitDescArray = new CEffectUnitDesc*[m_MaxEffectUnitDesc];
				memset(m_ppEffectUnitDescArray,0,sizeof(CEffectUnitDesc*)*m_MaxEffectUnitDesc);

			CMD_CS("#NEWEFFECTUNIT")
				unitnum = pFile->GetDword();
				pFile->GetString(buf2);
				CMD_ST(buf2)
					CMD_CS("LIGHT")
						BOOL bDO = pFile->GetBool();
						pUnit = new CLightEffectUnitDesc(bDO);
						pUnit->ParseScript(pFile);
					CMD_CS("OBJECT")
						BOOL bDO = pFile->GetBool();
						pUnit = new CObjectEffectUnitDesc(bDO);
						pUnit->ParseScript(pFile);
					CMD_CS("ANIMATION")
						BOOL bDO = pFile->GetBool();
						pUnit = new CAnimationEffectUnitDesc(bDO);
						pUnit->ParseScript(pFile);
					CMD_CS("DAMAGE")
						BOOL bDO = pFile->GetBool();
						pUnit = new CDamageEffectUnitDesc(bDO);
						pUnit->ParseScript(pFile);
						
					CMD_CS("CAMERA")
						BOOL bDO = pFile->GetBool();
						pUnit = new CCameraEffectUnitDesc(bDO);
						pUnit->ParseScript(pFile);
					CMD_CS("SOUND")
						BOOL bDO = pFile->GetBool();
						pUnit = new CSoundEffectUnitDesc(bDO);
						pUnit->ParseScript(pFile);
					CMD_CS("MOVE")
						BOOL bDO = pFile->GetBool();
						pUnit = new CMoveEffectUnitDesc(bDO);
						pUnit->ParseScript(pFile);
					
				CMD_EN
				if(m_MaxEffectUnitDesc <= unitnum)
				{
					MessageBox(NULL,"Check EffectUnit Num and MaxUnitNum!!!",0,0);
					return FALSE;
				}
				m_ppEffectUnitDescArray[unitnum] = pUnit;
			
			CMD_CS("#MAXTRIGGER")
				m_MaxEffectTriggerDesc = pFile->GetDword();
				m_ppEffectTriggerUnitDesc = new CEffectTriggerUnitDesc*[m_MaxEffectTriggerDesc];
				memset(m_ppEffectTriggerUnitDesc,0,sizeof(CEffectTriggerUnitDesc*)*m_MaxEffectTriggerDesc);

			CMD_CS("#TRIGGER")
				pFile->GetString(buf2);
				if(buf2[0] == 'f' || buf2[0] == 'F')
				{
					float frame = (float)atof(&buf2[1]);
					float temp = GAMERESRCMNGR->m_GameDesc.TickPerFrame;
					time = (DWORD)(temp * frame);		// 프레임일경우 시간으로 변환
				}
				else
				{
					time = atoi(buf2);
				}
				unitnum = pFile->GetDword();
				pFile->GetString(cmd);
				
				CEffectTriggerUnitDesc* pDesc = NULL;
				CMD_ST(cmd)
					CMD_CS("ON")
						pDesc = new CEffectTriggerOnUnitDesc(time,unitnum);

#ifdef _TESTCLIENT_
						if( m_MaxEffectUnitDesc > unitnum )
						if(m_ppEffectUnitDescArray[unitnum]->m_EffectUnitType == eEffectUnit_Damage)
						{
							pUnit = m_ppEffectUnitDescArray[unitnum];
							bDamageUnit = TRUE;
							Percent += ((CDamageEffectUnitDesc*)pUnit)->GetDamageRate();
						}
#endif
					CMD_CS("OFF")
						pDesc = new CEffectTriggerOffUnitDesc(time,unitnum);
					CMD_CS("MOVE")
						pDesc = new CEffectTriggerMoveUnitDesc(time,unitnum);
					CMD_CS("GRAVITYMOVE")
						pDesc = new CEffectTriggerGravityMoveUnitDesc(time,unitnum);
					CMD_CS("ATTACH")
						pDesc = new CEffectTriggerAttachUnitDesc(time,unitnum);
					CMD_CS("DETACH")
						pDesc = new CEffectTriggerDetachUnitDesc(time,unitnum);
					CMD_CS("CAMERAROTATE")
						pDesc = new CEffectTriggerCameraRotateUnitDesc(time,unitnum);
					CMD_CS("CAMERAZOOM")
						pDesc = new CEffectTriggerCameraZoomUnitDesc(time,unitnum);
					CMD_CS("CAMERAANGLE")
						pDesc = new CEffectTriggerCameraAngleUnitDesc(time,unitnum);
					CMD_CS("CAMERACHANGE")
						pDesc = new CEffectTriggerChangeCameraUnitDesc(time,unitnum);
					CMD_CS("SETBASEMOTION")
						pDesc = new CEffectTriggerSetBaseMotionUnitDesc(time,unitnum);
					CMD_CS("CAMERASHAKE")
						pDesc = new CEffectTriggerCameraShakeUnitDesc(time,unitnum);
					CMD_CS("LINK")
						pDesc = new CEffectTriggerLinkUnitDesc(time,unitnum);
					CMD_CS("FADEOUT")
						pDesc = new CEffectTriggerFadeOutUnitDesc(time,unitnum);
					CMD_CS("FADEIN")
						pDesc = new CEffectTriggerFadeOutUnitDesc(time,unitnum);
					CMD_CS("ANIMATE")
						pDesc = new CEffectTriggerAnimateUnitDesc(time,unitnum);
					CMD_CS("ILLUSION")
						pDesc = new CEffectTriggerIllusionUnitDesc(time,unitnum);
					
				CMD_EN
				ASSERT(pDesc);
				pDesc->ParseScript(pFile);
				
				
				if(m_MaxEffectTriggerDesc <= TriggerNum)
				{
					MessageBox(NULL,"Check TriggerNum and MaxTriggerNum!!!",0,0);
					return FALSE;
				}
				m_ppEffectTriggerUnitDesc[TriggerNum++] = pDesc;
		
		CMD_EN
	}
	
	if(m_MaxEffectTriggerDesc != TriggerNum)
	{
		MessageBox(NULL,"MaxTriggerNum is too BIG!!!",0,0);
		return FALSE;
	}

	if(bDamageUnit && (0.99 > Percent || Percent > 1.01))
	{
#ifdef _TESTCLIENT_
		MessageBox(NULL,"데미지 Percent의 합이 100이 아닙니다!!!",0,0);
		return FALSE;
#endif
	}

	return TRUE;
}

BOOL CEffectDesc::GetEffect(EFFECTPARAM* pParam,CEffect* pOutEffect)
{
	pOutEffect->Init(m_MaxEffectUnitDesc,pParam,this,m_MaxEffectTriggerDesc,m_ppEffectTriggerUnitDesc);

	CEffectUnit* pEffectUnit;
	for(DWORD n=0;n<m_MaxEffectUnitDesc;++n)
	{

		if(m_ppEffectUnitDescArray[n] == NULL)
		{			
#ifdef _DEBUG
//			MessageBox(NULL,"이펙트 유닛이 비어있습니다.",NULL,NULL);
			ASSERT(0);
#endif
			return FALSE;
		}
		pEffectUnit = m_ppEffectUnitDescArray[n]->GetEffectUnit(pParam);
		
		//////////////////////////////////////////////////////////////////////////
		// virtual 바인딩 시점 문제 때문에 CEffectUnit의 생성자에서 여기로 옮김 [4/29/2003]
		if(m_ppEffectUnitDescArray[n]->IsDangledToOperator())
		{
			TARGETSET Targetset;
			Targetset.pTarget = pParam->m_pOperator;
			pEffectUnit->InitEffect(1,&Targetset);
		}
		else
		{
			if(pParam->m_nTargetNum)
				pEffectUnit->InitEffect(pParam->m_nTargetNum,pParam->m_pTargetSet);
		}
		//////////////////////////////////////////////////////////////////////////

		pOutEffect->SetEffectUnit(n, pEffectUnit);
	}

	return TRUE;
}

DWORD CEffectDesc::GetOperatorAnimatioEndTime(CEngineObject* pEngineObject)
{	
	if(m_OperatorAnimationTime != 0)
	{
		return m_OperatorAnimationTime;
	}

	for(DWORD n=0;n<m_MaxEffectUnitDesc;++n)
	{
		if(m_ppEffectUnitDescArray[n]->m_EffectUnitType == eEffectUnit_Animation &&
			m_ppEffectUnitDescArray[n]->IsDangledToOperator())
		{
			CAnimationEffectUnitDesc* pDesc = (CAnimationEffectUnitDesc*)m_ppEffectUnitDescArray[n];
			
			DWORD AniTime = pEngineObject->GetAnimationTime(pDesc->GetMotionNum());
			
			for(DWORD trig=0;trig<m_MaxEffectTriggerDesc;++trig)
			{
				if(m_ppEffectTriggerUnitDesc[trig]->GetUnitNum() == n)
				{
					DWORD AniStartTime = m_ppEffectTriggerUnitDesc[trig]->GetStartTime();
					if(AniStartTime+AniTime >= m_OperatorAnimationTime)
					{
						m_OperatorAnimationTime = AniStartTime+AniTime;
						
//						if(m_OperatorAnimationTime > 2000)
//						{
//							int a=0;
//						}
						break;
					}
				}
			}
		}
	}
	
	return m_OperatorAnimationTime;

}
