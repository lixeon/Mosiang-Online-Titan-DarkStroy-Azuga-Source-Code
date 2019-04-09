// EngineObject.cpp: implementation of the CEngineObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EngineObject.h"
#include "GraphicEngine.h"

#include "../MHFile.h"




#ifndef _RESOURCE_WORK_NOT_APPLY_

#include "EngineObjectCache.h"

#endif



CAniInfoManager CEngineObject::m_staAniMgr;
extern VECTOR3 gHeroPos;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEngineObject::CEngineObject()
{
#ifndef _RESOURCE_WORK_NOT_APPLY_
	m_pCacheListData = NULL;
	m_fAlpha = 1.0f;	
#endif
	m_GXOHandle = NULL;
	m_CurMotionNum = -1;  
	m_bMotionPause = FALSE;
	m_BaseMotionNum = 1;
	m_AniInfo = NULL;
	m_bApplyHeight = FALSE;
	m_pObjectLinkTo = NULL;
}

CEngineObject::~CEngineObject()
{
	Release();
}

void CEngineObject::PreLoadObject(char* filename)
{
	g_pExecutive->PreLoadGXObject(filename);
}


BOOL CEngineObject::Init(char* filename, CObjectBase* pObject, BYTE EngineObjectType, BYTE AttachObjectType)
{
	m_NotProcessedAnimateKey = 0;
	
	if(m_GXOHandle == NULL)
		m_CurMotionNum = 1;

	Release();
	
	//m_CurMotionNum = -1;
	//m_bAnimationRoop = TRUE;
	m_bMotionPause = FALSE;	
	m_bEndMotion = FALSE;

	m_pObject = pObject;
	if(m_pObject)
		m_pObject->m_EngineObjectType = EngineObjectType;
	
	DWORD dwFlag = 0;
	if( EngineObjectType == eEngineObjectType_Effect )
		dwFlag = GXOBJECT_CREATE_TYPE_EFFECT;	

	//trustpak 2005/06/27
	if ( EngineObjectType == eEngineObjectType_GameSystemObject)
	{
		dwFlag = GXOBJECT_CREATE_TYPE_EFFECT;
	}

	if (EngineObjectType == eEngineObjectType_Shadow)
	{
		dwFlag = GXOBJECT_CREATE_TYPE_EFFECT;
	}
	///

	if( AttachObjectType == eAttachObjectType_TargetObject || 
		AttachObjectType == eAttachObjectType_withAni )
		dwFlag = GXOBJECT_CREATE_TYPE_NOT_OPTIMIZE;

	if(EngineObjectType == eEngineObjectType_Character)
		DIRECTORYMGR->SetLoadMode(eLM_Character);
	else if(EngineObjectType == eEngineObjectType_Monster)
		DIRECTORYMGR->SetLoadMode(eLM_Monster);
	else if(EngineObjectType == eEngineObjectType_Npc)
		DIRECTORYMGR->SetLoadMode(eLM_Npc);
	else if(EngineObjectType == eEngineObjectType_Effect)
		DIRECTORYMGR->SetLoadMode(eLM_Effect);
	else if(EngineObjectType == eEngineObjectType_Weapon)
		DIRECTORYMGR->SetLoadMode(eLM_Character);
	else if(EngineObjectType == eEngineObjectType_Pet)
		DIRECTORYMGR->SetLoadMode(eLM_Pet);
	else if(EngineObjectType == eEngineObjectType_GameSystemObject)
		DIRECTORYMGR->SetLoadMode(eLM_Effect);
	else if(EngineObjectType == eEngineObjectType_Shadow)
		DIRECTORYMGR->SetLoadMode(eLM_Effect);
	else if(EngineObjectType == eEngineObjectType_MapObject)
		DIRECTORYMGR->SetLoadMode(eLM_MapObject);
	else if(EngineObjectType == eEngineObjectType_Titan)
		DIRECTORYMGR->SetLoadMode(eLM_Titan);


#ifndef _RESOURCE_WORK_NOT_APPLY_	

	PreLoadObject(filename);

 	if( AttachObjectType == eAttachObjectType_TargetObject  )
		m_GXOHandle = g_pEngineObjectCache->CreateGXObjectThroughCache(filename, MHPlayerPROC, this, dwFlag);
	else if( AttachObjectType == eAttachObjectType_withAni  )
		m_GXOHandle = g_pEngineObjectCache->CreateGXObjectThroughCache(filename, MHPlayerPROC, this, dwFlag);
	else
		m_GXOHandle = g_pEngineObjectCache->CreateGXObjectThroughCache(filename, MHPlayerPROC, this, dwFlag);

#else
	
	//////////////////////////////////////////////////////////////////////////
	// 임시 설정
	PreLoadObject(filename);
	//////////////////////////////////////////////////////////////////////////
	
	if( AttachObjectType == eAttachObjectType_TargetObject  )
		m_GXOHandle = g_pExecutive->CreateGXObject(filename, NULL, NULL, dwFlag);
	else if( AttachObjectType == eAttachObjectType_withAni  )
		m_GXOHandle = g_pExecutive->CreateGXObject(filename, MHPlayerPROC, NULL, dwFlag);
	else
		m_GXOHandle = g_pExecutive->CreateGXObject(filename, MHPlayerPROC, this, dwFlag);

#endif
	
	if(m_GXOHandle == NULL)
	{
		FILE* fp = fopen("./Log/MissedObjectList.txt","a+");
		if(fp)
		{
			fprintf(fp,"%s\n",filename);
			fclose(fp);
		}
		DIRECTORYMGR->SetLoadMode(eLM_Root);
		return FALSE;
	}


	if(EngineObjectType == eEngineObjectType_Effect)
		EnableShadow(FALSE);

	//trustpak 2005/06/27
	if (EngineObjectType == eEngineObjectType_GameSystemObject)
		EnableShadow(FALSE);

	if (EngineObjectType == eEngineObjectType_Shadow)
		EnableShadow(FALSE);
	///
	
	//yh2do test
	//g_pExecutive->EnableUpdateShading(m_GXOHandle);
	
	m_AniInfo = m_staAniMgr.GetAniInfoObj(filename,m_GXOHandle);

	DIRECTORYMGR->SetLoadMode(eLM_Root);
	
	//FILE* fp = fopen("
	if(m_CurMotionNum == -1)
		ChangeMotion(1);
	SetEngineToCurmotion();
	
	return TRUE;
}

void CEngineObject::Release()
{
	if(m_pObjectLinkTo)
	{
		m_pObjectLinkTo->m_LinkedObject.Remove(this);
		m_pObjectLinkTo = NULL;
	}
	PTRLISTSEARCHSTART(m_LinkedObject,CEngineObject*,pLinkChild)
		pLinkChild->LinkTo(NULL);
	PTRLISTSEARCHEND

	EndIllusionEffect();
	m_bApplyHeight = FALSE;
	RemoveAllAttachedWeapon();
	RemoveAllAttachedDress();
	if(m_GXOHandle)
	{
		AddGarbageObject(m_GXOHandle);
		Hide();
		m_GXOHandle = NULL;
	}
	if(m_AniInfo)
	{
		m_AniInfo = NULL;
	}
}

void CEngineObject::Animate(int animatekey)
{
	if(animatekey == 0)
		return;
	if(m_bMotionPause)
		return;
	if(m_AniInfo->GetMaxAniNum() == 0)
		return;

	//////////////////////////////////////////////////////////////////////////
	// 게임이 느리면 한번 걸러 한번씩 에니메이션을 처리한다.
	if(IsGameSlow() == TRUE)
	{
		VECTOR3 thispos;
		int SkipAnimateCount;
		g_pExecutive->GXOGetPosition(m_GXOHandle,&thispos);
		float gx = thispos.x - gHeroPos.x;
		float gz = thispos.z - gHeroPos.z;
		float dist = gx*gx + gz*gz;
		if(dist < 490000)	// 700*700
			SkipAnimateCount = 0;
		else if(dist < 4000000)		// 2000*2000
			SkipAnimateCount = 3;
		else
			SkipAnimateCount = 7;
		if(m_NotProcessedAnimateKey < SkipAnimateCount)
		{
			m_NotProcessedAnimateKey += animatekey;
			return;
		}
	}
	if(m_NotProcessedAnimateKey != 0)
	{
		animatekey += m_NotProcessedAnimateKey;
		m_NotProcessedAnimateKey = 0;
	}
	//////////////////////////////////////////////////////////////////////////
		
	DWORD	dwMotionIndex = g_pExecutive->GXOGetCurrentMotionIndex(m_GXOHandle);
	DWORD	dwFrame = g_pExecutive->GXOGetCurrentFrame(m_GXOHandle);
	MOTION_DESC pMotionDesc;
	if(dwMotionIndex == 0)
		dwMotionIndex = 1;
	g_pExecutive->GXOGetMotionDesc(m_GXOHandle,&pMotionDesc,dwMotionIndex,0);

	DWORD CurFrame = dwFrame+animatekey;
	if(CurFrame > pMotionDesc.dwLastFrame || CurFrame > m_pCurAniInfo->EndFrame)
	{
		if(m_bAnimationRoop)
		{
			g_pExecutive->GXOSetCurrentFrame(m_GXOHandle, m_pCurAniInfo->StartFrame);
			m_bEndMotion = TRUE;
		}
		else
		{
			m_bEndMotion = TRUE;
			ChangeMotion(m_BaseMotionNum);
		}
	}
	else
	{
		g_pExecutive->GXOSetCurrentFrame(m_GXOHandle, CurFrame);
		m_bEndMotion = FALSE;
	}
	
}

void CEngineObject::SetEngineToCurmotion()
{
	if(m_GXOHandle == NULL)		return;

	m_pCurAniInfo = m_AniInfo->GetAniInfo(m_CurMotionNum);
	g_pExecutive->GXOSetCurrentMotionIndex(m_GXOHandle,m_pCurAniInfo->EngineMotionIndex);
	g_pExecutive->GXOSetCurrentFrame(m_GXOHandle, m_pCurAniInfo->StartFrame);
}

//////////////////////////////////////////////////////////////////////////
void CEngineObject::SetEngObjPosition(VECTOR3* pPos)
{
	if(m_GXOHandle == NULL)		return;
	
	
//	ASSERTMSG(pPos->x > -1000,"이 메세지 도 윤호한테 연락을... Set");
//	ASSERTMSG(pPos->z > -1000,"이 메세지 도 윤호한테 연락을... Set");

	VECTOR3 position;
	if(g_bFixHeight)
	{
		position.x = pPos->x;
		position.z = pPos->z;
		
		if(m_bApplyHeight)
			position.y = g_fFixHeight;
		else
			position.y = pPos->y;

		g_pExecutive->GXOSetPosition(m_GXOHandle,&position,FALSE);
	}
	else
		g_pExecutive->GXOSetPosition(m_GXOHandle,pPos,FALSE);

	if(m_LinkedObject.GetCount() != 0)
	{
		GetEngObjPosition(&position);
		PTRLISTSEARCHSTART(m_LinkedObject,CEngineObject*,pLinkChild)
			pLinkChild->SetEngObjPosition(&position);
		PTRLISTSEARCHEND
	}
}

void CEngineObject::SetEngObjAngle(float angleRad)
{
	if(m_GXOHandle == NULL)		return;

	static VECTOR3 axis = { 0,1,0};
	g_pExecutive->GXOSetDirection(m_GXOHandle,&axis,angleRad);
}

void CEngineObject::SetEngObjAngleX(float angleRad)
{
	if(m_GXOHandle == NULL)		return;

	static VECTOR3 axis = { 1,0,0};
	g_pExecutive->GXOSetDirection(m_GXOHandle,&axis,angleRad);
}
void CEngineObject::SetEngObjAngleZ(float angleRad)
{
	if(m_GXOHandle == NULL)		return;

	static VECTOR3 axis = { 0,0,1};
	g_pExecutive->GXOSetDirection(m_GXOHandle,&axis,angleRad);
}

void CEngineObject::RotateEngObj( float x, float y, float z )
{
	if(m_GXOHandle == NULL)		return;

	if( x != 0.0f ) SetEngObjAngleX( x );
	if( y != 0.0f ) SetEngObjAngle( y );
	if( z != 0.0f ) SetEngObjAngleZ( z );
}

void CEngineObject::RotateEngObj( VECTOR3 vRot )
{
	if(m_GXOHandle == NULL)
	{
		return;
	}

	if( vRot.x != 0.0f ) SetEngObjAngleX( vRot.x );
	if( vRot.y != 0.0f ) SetEngObjAngle( vRot.y );
	if( vRot.z != 0.0f ) SetEngObjAngleZ( vRot.z );
}

void CEngineObject::GetEngObjPosition(VECTOR3* pPos)
{
	if(m_GXOHandle == NULL)
	{
		pPos->x = pPos->y = pPos->z = 0;
		return;
	}

	g_pExecutive->GXOGetPosition(m_GXOHandle,pPos);
}

float CEngineObject::GetEngObjAngle()
{
	if(m_GXOHandle == NULL)		return 0;

	static VECTOR3 axis = { 0,1,0};
	float angle;
	g_pExecutive->GXOGetDirection(m_GXOHandle,&axis,&angle);
	return angle;
}

void CEngineObject::Show()
{
	if(m_GXOHandle == NULL)		return;

	if(m_CurMotionNum == -1)
	{
		ChangeMotion(1);
	}
	
	g_pExecutive->GXOSetCurrentFrame(m_GXOHandle, m_pCurAniInfo->StartFrame);
	m_bEndMotion = FALSE;
	g_pExecutive->EnableSchedule(m_GXOHandle);
	g_pExecutive->EnableRender(m_GXOHandle);
}

void CEngineObject::Hide()
{
	if(m_GXOHandle == NULL)		return;

	EndIllusionEffect();
	g_pExecutive->DisableSchedule(m_GXOHandle);
	g_pExecutive->DisableRender(m_GXOHandle);
}

void CEngineObject::HideWithScheduling()
{
	if(m_GXOHandle == NULL)		return;

	g_pExecutive->EnableSchedule(m_GXOHandle);
	g_pExecutive->DisableRender(m_GXOHandle);
}

void CEngineObject::ChangeBaseMotion(int BaseMotionNum)
{
	m_BaseMotionNum = BaseMotionNum;
}

void CEngineObject::ChangeMotion(int MotionNum,BOOL bRoop)
{
	if(m_GXOHandle == NULL)		return;

	if(m_CurMotionNum != MotionNum)
	{
		m_CurMotionNum = MotionNum;

		m_pCurAniInfo = m_AniInfo->GetAniInfo(m_CurMotionNum);
		g_pExecutive->GXOSetCurrentMotionIndex(m_GXOHandle,m_pCurAniInfo->EngineMotionIndex);
		g_pExecutive->GXOSetCurrentFrame(m_GXOHandle, m_pCurAniInfo->StartFrame);
		m_bMotionPause = FALSE;
	}
	m_bAnimationRoop = bRoop;
}

void CEngineObject::ChangeCustumMotion(int MotionNum,int StartFrame,int EndFrame,BOOL bRoop)
{
	if(m_GXOHandle == NULL)		return;

	m_CurMotionNum = MotionNum;

	m_CustumAniInfo.EngineMotionIndex = MotionNum;
	m_CustumAniInfo.StartFrame = StartFrame;
	m_CustumAniInfo.EndFrame = EndFrame;

	m_pCurAniInfo = &m_CustumAniInfo;
	
	g_pExecutive->GXOSetCurrentMotionIndex(m_GXOHandle,m_pCurAniInfo->EngineMotionIndex);
	g_pExecutive->GXOSetCurrentFrame(m_GXOHandle, m_pCurAniInfo->StartFrame);
	m_bMotionPause = FALSE;

	m_bAnimationRoop = bRoop;
}

BOOL CEngineObject::Attach(CEngineObject* pToObject,char* szObjectName)
{
	if(m_GXOHandle == NULL)		return FALSE;
	
#ifndef _RESOURCE_WORK_NOT_APPLY_

	if (NULL == pToObject->m_GXOHandle)
	{
		return FALSE;
	}

#endif

	return g_pExecutive->GXOAttach(m_GXOHandle,pToObject->m_GXOHandle,szObjectName);
}

void CEngineObject::Detach(CEngineObject* pChildObject)
{
	if(m_GXOHandle == NULL)		return;

	BOOL rt = g_pExecutive->GXODetach(m_GXOHandle,pChildObject->m_GXOHandle);
}

void CEngineObject::DrawFitInViewPort(int ViewPortNum,float fFOV)
{
	if(m_GXOHandle == NULL)		return;

	g_pExecutive->SetCameraFitGXObject(m_GXOHandle,1,1000,fFOV,ViewPortNum);
	g_pExecutive->GetGeometry()->SetAmbientColor(ViewPortNum,0xffffffff);
	g_pExecutive->GetGeometry()->BeginRender(ViewPortNum,0xffff0000,BEGIN_RENDER_FLAG_DONOT_CLEAR_FRAMEBUFFER);
	g_pExecutive->GXORender(m_GXOHandle);
	g_pExecutive->GetGeometry()->EndRender();
}

void CEngineObject::SetScale(VECTOR3* pScale)
{
	if(m_GXOHandle == NULL)		return;

	g_pExecutive->GXOSetScale(m_GXOHandle,pScale);
}

void CEngineObject::GetScale(VECTOR3* pScale)
{
	if(m_GXOHandle == NULL)		return;

	g_pExecutive->GXOGetScale(m_GXOHandle,pScale);
}

BOOL CEngineObject::ChangePart(int PartNum,char* pFileName)
{
	if(m_GXOHandle == NULL)		return FALSE;

	if(m_pObject->m_EngineObjectType == eEngineObjectType_Character)
		DIRECTORYMGR->SetLoadMode(eLM_Character);
	else if(m_pObject->m_EngineObjectType == eEngineObjectType_Monster)
		DIRECTORYMGR->SetLoadMode(eLM_Monster);
	else if(m_pObject->m_EngineObjectType == eEngineObjectType_Npc)
		DIRECTORYMGR->SetLoadMode(eLM_Npc);
	else if(m_pObject->m_EngineObjectType == eEngineObjectType_Effect)
		DIRECTORYMGR->SetLoadMode(eLM_Effect);
	else if(m_pObject->m_EngineObjectType == eEngineObjectType_Weapon)
		DIRECTORYMGR->SetLoadMode(eLM_Character);
	else if(m_pObject->m_EngineObjectType == eEngineObjectType_Pet)
		DIRECTORYMGR->SetLoadMode(eLM_Monster);
	else if(m_pObject->m_EngineObjectType == eEngineObjectType_MapObject )
		DIRECTORYMGR->SetLoadMode(eLM_MapObject);
	else if(m_pObject->m_EngineObjectType == eEngineObjectType_Titan )
		DIRECTORYMGR->SetLoadMode(eLM_Titan);

	BOOL rt = g_pExecutive->GXOReplaceModel(m_GXOHandle,PartNum,pFileName);

	
		DIRECTORYMGR->SetLoadMode(eLM_Root);
	return rt;
}

void CEngineObject::ApplyHeightField(BOOL bApply)
{
	if(m_GXOHandle == NULL)		return;

	m_bApplyHeight = bApply;
	if(g_bFixHeight)
		return;
	
	if(bApply)
		g_pExecutive->GXOEnableHFieldApply(m_GXOHandle);
	else
		g_pExecutive->GXODisableHFieldApply(m_GXOHandle);
}

void CEngineObject::EnableShadow(BOOL bShadow)
{
	if(m_GXOHandle == NULL)		return;

	if(bShadow)
		g_pExecutive->EnableSendShadow(m_GXOHandle);
	else
		g_pExecutive->DisableSendShadow(m_GXOHandle);
	
	PTRLISTSEARCHSTART(m_AttachedWeapon,CEngineObject*,pWeapon)
		pWeapon->EnableShadow(bShadow);
	PTRLISTSEARCHEND
	
	PTRLISTSEARCHSTART(m_AttachedDress,CEngineObject*,pDress)
		pDress->EnableShadow(bShadow);
	PTRLISTSEARCHEND
}

void CEngineObject::SetAlpha(float Alpha)
{
	if(m_GXOHandle == NULL)		return;

#ifndef _RESOURCE_WORK_NOT_APPLY_
	m_fAlpha = Alpha;

#endif

	DWORD dAlpha = (DWORD)(Alpha * 255);
	if(dAlpha == 0)
		dAlpha = 1;
	
	g_pExecutive->SetAlphaFlag(m_GXOHandle,dAlpha);
	
	
	PTRLISTSEARCHSTART(m_AttachedWeapon,CEngineObject*,pWeapon)
		pWeapon->SetAlpha(Alpha);
	PTRLISTSEARCHEND
	
	PTRLISTSEARCHSTART(m_AttachedDress,CEngineObject*,pDress)
		pDress->SetAlpha(Alpha);
	PTRLISTSEARCHEND
}

void CEngineObject::EnablePick()
{
	if(m_GXOHandle == NULL)		return;

	g_pExecutive->EnablePick(m_GXOHandle);
	
	PTRLISTSEARCHSTART(m_AttachedWeapon,CEngineObject*,pWeapon)
		pWeapon->EnablePick();
	PTRLISTSEARCHEND
	
	PTRLISTSEARCHSTART(m_AttachedDress,CEngineObject*,pDress)
		pDress->EnablePick();
	PTRLISTSEARCHEND
}

void CEngineObject::DisablePick()
{
	if(m_GXOHandle == NULL)		return;

	g_pExecutive->DisablePick(m_GXOHandle);
	
	PTRLISTSEARCHSTART(m_AttachedWeapon,CEngineObject*,pWeapon)
		pWeapon->DisablePick();
	PTRLISTSEARCHEND
	
	PTRLISTSEARCHSTART(m_AttachedDress,CEngineObject*,pDress)
		pDress->DisablePick();
	PTRLISTSEARCHEND
}

void CEngineObject::EnableSelfIllumin()
{
	if(m_GXOHandle == NULL)		return;

	g_pExecutive->GXOEnableSelfIllumin(m_GXOHandle);
	
	PTRLISTSEARCHSTART(m_AttachedWeapon,CEngineObject*,pWeapon)
		pWeapon->EnableSelfIllumin();
	PTRLISTSEARCHEND
	
	PTRLISTSEARCHSTART(m_AttachedDress,CEngineObject*,pDress)
		pDress->EnableSelfIllumin();
	PTRLISTSEARCHEND
}

void CEngineObject::DisableSelfIllumin()
{
	if(m_GXOHandle == NULL)		return;

	g_pExecutive->GXODisableSelfIllumin(m_GXOHandle);
	
	PTRLISTSEARCHSTART(m_AttachedWeapon,CEngineObject*,pWeapon)
		pWeapon->DisableSelfIllumin();
	PTRLISTSEARCHEND
	
	PTRLISTSEARCHSTART(m_AttachedDress,CEngineObject*,pDress)
		pDress->DisableSelfIllumin();
	PTRLISTSEARCHEND
}

BOOL CEngineObject::IsVisible()
{
	if(m_GXOHandle == NULL)		return FALSE;

	return g_pExecutive->IsInViewVolume(m_GXOHandle);
}

void CEngineObject::InitializeIllusionEffect(DWORD MaxIllusionFrame,void* pMtl)
{
	return;
	if(m_GXOHandle == NULL)		return;

	/*
	void* pMtlHandle =	NULL;
	MATERIAL	mtl;	
	memset(&mtl, 0, sizeof(mtl));
	mtl.dwDiffuse	=	0xffffffff;
	mtl.dwAmbient	=	0xffffffff;
	mtl.dwFlag = TRANSP_TYPE_ADDITIVE;
    lstrcpy(mtl.szDiffuseTexmapFileName,"d:\\work\\CWorking\\3ddata\\Effect\\mo2.tga");	// 이 매터리얼이 사용할 텍스쳐 파일
	
	// 매터리얼 생성. 필요없게되면 반드시 해제해야한다.
	pMtlHandle = g_pExecutive->GetRenderer()->CreateMaterial( &mtl,NULL,NULL, 0);
	*/
	BOOL rt = g_pExecutive->GXOInitializeIllusionEffect(m_GXOHandle,MaxIllusionFrame,"_ILLUSION_MESH",pMtl,0);
}
void CEngineObject::BeginIllusionEffect()
{
	return;
	if(m_GXOHandle == NULL)		return;

	g_pExecutive->GXOBeginIllusionEffect(m_GXOHandle);
}
void CEngineObject::EndIllusionEffect()
{
	return;
	if(m_GXOHandle == NULL)		return;

	g_pExecutive->GXOEndIllusionEffect(m_GXOHandle);
}


DWORD CEngineObject::GetAnimationTime(DWORD MotionNum)
{
	if(m_GXOHandle == NULL)		return 1;
	if(MotionNum == (DWORD)-1)	return 0;

	ANIINFO* AniInfo = m_AniInfo->GetAniInfo(MotionNum);
	MOTION_DESC pMotionDesc;
	g_pExecutive->GXOGetMotionDesc(m_GXOHandle,&pMotionDesc,AniInfo->EngineMotionIndex,0);

	DWORD lastFrame = pMotionDesc.dwLastFrame < AniInfo->EndFrame ?
							pMotionDesc.dwLastFrame : AniInfo->EndFrame;
	DWORD startFrame = AniInfo->StartFrame;

	if(startFrame > lastFrame)
	{
		MessageBox(NULL,"StartFrame is bigger than LastFrame. Check chi File",0,0);
		return 0;
	}
	DWORD frame = lastFrame - startFrame;
	
	return (DWORD)(frame * gTickPerFrame);
}

void CEngineObject::RandMotionFrame()
{
	if(m_GXOHandle == NULL)		return;

	DWORD MotionNum = m_CurMotionNum;
	MOTION_DESC pMotionDesc;
	g_pExecutive->GXOGetMotionDesc(m_GXOHandle,&pMotionDesc,MotionNum,0);

	ANIINFO* AniInfo = m_AniInfo->GetAniInfo(MotionNum);
	DWORD aniinfo = AniInfo->EndFrame - AniInfo->StartFrame;

	DWORD frame = pMotionDesc.dwLastFrame < aniinfo ? 
					pMotionDesc.dwLastFrame : aniinfo;
	
	if(pMotionDesc.dwLastFrame == 0)
	{
		if(AniInfo->EndFrame == (WORD)-1)
		{
			g_pExecutive->GXOGetMotionDesc(m_GXOHandle,&pMotionDesc,AniInfo->EngineMotionIndex,0);
			frame = pMotionDesc.dwLastFrame - AniInfo->StartFrame;
		}
		else
			frame = aniinfo;
	}

	DWORD rrr = 0;
	if(frame)
		rrr = rand() % frame;

	g_pExecutive->GXOSetCurrentFrame(m_GXOHandle,rrr);
}
void CEngineObject::SetDirection(VECTOR3* pDir)
{
	if(m_GXOHandle == NULL)		return;

	if(pDir->x == 0 && pDir->y == 0 && pDir->z == 0)
		return;

	VECTOR3 ypr;
	ypr.x = (float)(atan2(pDir->y, roughGetLength(pDir->x,pDir->z)));
	ypr.y = -(float)(atan2(pDir->z, pDir->x) + gPHI/2.f);
	ypr.z = 0;

	g_pExecutive->GXOSetDirectionFPSStyle(m_GXOHandle,&ypr);
}

void CEngineObject::AttachToCamera(float fDist)
{
	if(m_GXOHandle == NULL)		return;
	g_pExecutive->GXOAttachCameraFront(m_GXOHandle,fDist);
}
void CEngineObject::DetachFromCamera()
{
	if(m_GXOHandle == NULL)		return;
	g_pExecutive->GXODetachCameraFront(m_GXOHandle);
}

//////////////////////////////////////////////////////////////////////////

BOOL CEngineObject::AttachWeapon(CEngineObject* pWeaponObject,char* szObjectName)
{
	BOOL rt = pWeaponObject->Attach(this,szObjectName);
	if(rt)
		m_AttachedWeapon.AddHead(pWeaponObject);

	return rt;
}

BOOL CEngineObject::AttachDress(CEngineObject* pDressObject,char* szObjectName)
{
	BOOL rt = pDressObject->Attach(this,szObjectName);
	if(rt)
		m_AttachedDress.AddHead(pDressObject);

	return rt;
}

void CEngineObject::RemoveAllAttachedWeapon()
{
	CEngineObject* pAttachedWeapon;
	while(pAttachedWeapon = (CEngineObject*)m_AttachedWeapon.RemoveTail())
	{
		Detach(pAttachedWeapon);
		delete pAttachedWeapon;		
	}	
}

void CEngineObject::RemoveAllAttachedDress()
{
	CEngineObject* pDressObject;
	while(pDressObject = (CEngineObject*)m_AttachedDress.RemoveTail())
	{
		Detach(pDressObject);
		delete pDressObject;		
	}	
}

void CEngineObject::StartWeaponIllusion(DWORD MaxIllusionFrame,void* pMtl)
{
	PTRLISTSEARCHSTART(m_AttachedWeapon,CEngineObject*,pWeapon)
		pWeapon->InitializeIllusionEffect(MaxIllusionFrame,pMtl);
		pWeapon->BeginIllusionEffect();
	PTRLISTSEARCHEND
}
void CEngineObject::EndWeaponIllusion()
{
	PTRLISTSEARCHSTART(m_AttachedWeapon,CEngineObject*,pWeapon)
		pWeapon->EndIllusionEffect();
	PTRLISTSEARCHEND
}

void CEngineObject::LinkTo(CEngineObject* pParent)
{
	if(pParent != m_pObjectLinkTo)
	{
		if(m_pObjectLinkTo)
			m_pObjectLinkTo->m_LinkedObject.Remove(this);

		if(pParent)
			pParent->m_LinkedObject.AddTail(this);

		m_pObjectLinkTo = pParent;
	}
}


DWORD __stdcall MHPlayerPROC(I4DyuchiGXExecutive* pExecutive, GXMAP_OBJECT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData)
{
	CEngineObject* pObject = (CEngineObject*)pData;
	if(pObject == NULL)
		return 0;

	if(arg1)
		pObject->Animate(arg1);

	if(pObject->m_pObject)
		pObject->m_pObject->Process();

	return 0;
}


#ifndef _RESOURCE_WORK_NOT_APPLY_
void CEngineObject::SetCacheListData(void* pData)
{
	m_pCacheListData = pData;
}
void* CEngineObject::GetCacheListData(void)
{
	return m_pCacheListData;
}

#endif



//---material 
VOID CEngineObject::SetMaterialIndex(DWORD dwIndex)
{
	if (NULL == m_GXOHandle)
	{
		return;
	}

	// Index의 범위가 넘어가더라도 내부에서 예외처리를 하기 때문에 아무런 이상 없음. 
	// chr이나 chx에 명시된 MTL 파일의 순서가 Index가 된다. 
	g_pExecutive->GXOSetCurrentMaterialIndex(m_GXOHandle, dwIndex);
}