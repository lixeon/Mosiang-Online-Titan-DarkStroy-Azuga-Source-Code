// EngineObject.cpp: implementation of the CEngineObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EngineObject.h"
#include "GraphicEngine.h"

#include "../MHFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEngineObject::CEngineObject()
{
	m_GXOHandle = NULL;
	m_CurMotionNum = -1;
	m_bMotionPause = FALSE;
	m_BaseMotionNum = 1;
	m_AniInfo = NULL;
	m_bApplyHeight = FALSE;
}

CEngineObject::~CEngineObject()
{
	Release();
}

void CEngineObject::PreLoadObject(char* filename)
{
	g_pExecutive->PreLoadGXObject(filename);
}


BOOL CEngineObject::Init(char* filename,CObjectBase* pObject,BYTE EngineObjectType)
{
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
	if( EngineObjectType == eEngineObjectType_Effect ||
		EngineObjectType == eEngineObjectType_Weapon)
		dwFlag = GXOBJECT_CREATE_TYPE_EFFECT;

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
	
	//////////////////////////////////////////////////////////////////////////
	// 임시 설정
	PreLoadObject(filename);
	//////////////////////////////////////////////////////////////////////////
	
	m_GXOHandle = g_pExecutive->CreateGXObject(filename,MHPlayerPROC,this,dwFlag);
	if(m_GXOHandle == NULL)
	{
		char temp[256];
		sprintf(temp,"오브젝트 생성 실패 : %s",filename);
		MessageBox(NULL,temp,NULL,NULL);
		return FALSE;
	}
	
	if(EngineObjectType == eEngineObjectType_Effect)
		EnableShadow(FALSE);
	
	//yh2do test
	//g_pExecutive->EnableUpdateShading(m_GXOHandle);
	
	LoadInfoFile(filename);


	DIRECTORYMGR->SetLoadMode(eLM_Root);
	
	//FILE* fp = fopen("
	SetEngineToCurmotion();
	
	return TRUE;
}

void CEngineObject::LoadInfoFile(char* filename)
{
	DWORD MaxAniNum = g_pExecutive->GXOGetMotionNum(m_GXOHandle,0);	

	m_AniInfo = new ANIINFO[MaxAniNum+1];

	for(WORD n=0;n<MaxAniNum+1;++n)
	{
		m_AniInfo[n].EngineMotionIndex = n;
		m_AniInfo[n].StartFrame = 0;
		m_AniInfo[n].EndFrame = -1;
	}

	char ChiFileName[256];
	strcpy(ChiFileName,filename);
	sprintf(&ChiFileName[strlen(filename)-3],"CHI");
	CMHFile file;
	if(file.Init(ChiFileName,"r",
		MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR|
		MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT) == FALSE)
		return;
	
	WORD MotionNum;
	while(MotionNum = file.GetWord())
	{
		ASSERT(MotionNum <= MaxAniNum);
		m_AniInfo[MotionNum].EngineMotionIndex = file.GetWord();
		m_AniInfo[MotionNum].StartFrame = file.GetWord();
		m_AniInfo[MotionNum].EndFrame = file.GetWord();
	}
}

void CEngineObject::Release()
{
	m_bApplyHeight = FALSE;
	RemoveAllAttachedWeapon();
	if(m_GXOHandle)
	{
		AddGarbageObject(m_GXOHandle);
		m_GXOHandle = NULL;
	}
	if(m_AniInfo)
	{
		delete [] m_AniInfo;
		m_AniInfo = NULL;
	}
}

void CEngineObject::Animate(int animatekey)
{
	if(animatekey == 0)
		return;
	if(m_bMotionPause)
		return;

	DWORD	dwMotionIndex = g_pExecutive->GXOGetCurrentMotionIndex(m_GXOHandle);
	DWORD	dwFrame = g_pExecutive->GXOGetCurrentFrame(m_GXOHandle);
	MOTION_DESC pMotionDesc;
	if(dwMotionIndex == 0)
		dwMotionIndex = 1;
	g_pExecutive->GXOGetMotionDesc(m_GXOHandle,&pMotionDesc,dwMotionIndex,0);

	DWORD CurFrame = dwFrame+animatekey;
	if(CurFrame > pMotionDesc.dwLastFrame || CurFrame > m_AniInfo[m_CurMotionNum].EndFrame)
	{
		if(m_bAnimationRoop)
		{
			g_pExecutive->GXOSetCurrentFrame(m_GXOHandle, m_AniInfo[m_CurMotionNum].StartFrame);
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
	g_pExecutive->GXOSetCurrentMotionIndex(m_GXOHandle,m_AniInfo[m_CurMotionNum].EngineMotionIndex);
	g_pExecutive->GXOSetCurrentFrame(m_GXOHandle, m_AniInfo[m_CurMotionNum].StartFrame);
}

//////////////////////////////////////////////////////////////////////////
void CEngineObject::SetEngObjPosition(VECTOR3* pPos)
{
	if(g_bFixHeight)
	{			
		VECTOR3 pos;
		pos.x = pPos->x;
		pos.z = pPos->z;
		
		if(m_bApplyHeight)
			pos.y = g_fFixHeight;
		else
			pos.y = pPos->y;

		g_pExecutive->GXOSetPosition(m_GXOHandle,&pos, FALSE);
	}
	else
		g_pExecutive->GXOSetPosition(m_GXOHandle,pPos, FALSE);
}
void CEngineObject::SetEngObjAngle(float angleRad)
{
	static VECTOR3 axis = { 0,1,0};
	g_pExecutive->GXOSetDirection(m_GXOHandle,&axis,angleRad);
}
void CEngineObject::GetEngObjPosition(VECTOR3* pPos)
{
	g_pExecutive->GXOGetPosition(m_GXOHandle,pPos);
}
float CEngineObject::GetEngObjAngle()
{
	static VECTOR3 axis = { 0,1,0};
	float angle;
	g_pExecutive->GXOGetDirection(m_GXOHandle,&axis,&angle);
	return angle;
}
void CEngineObject::Show()
{
	if(m_CurMotionNum == -1)
	{
		ChangeMotion(1);
	}
	g_pExecutive->GXOSetCurrentFrame(m_GXOHandle, m_AniInfo[m_CurMotionNum].StartFrame);
	m_bEndMotion = FALSE;
	g_pExecutive->EnableSchedule(m_GXOHandle);
	g_pExecutive->EnableRender(m_GXOHandle);
}
void CEngineObject::Hide()
{
	g_pExecutive->DisableSchedule(m_GXOHandle);
	g_pExecutive->DisableRender(m_GXOHandle);
}
void CEngineObject::HideWithScheduling()
{
	g_pExecutive->EnableSchedule(m_GXOHandle);
	g_pExecutive->DisableRender(m_GXOHandle);
}
void CEngineObject::ChangeBaseMotion(int BaseMotionNum)
{
	m_BaseMotionNum = BaseMotionNum;
}
void CEngineObject::ChangeMotion(int MotionNum,BOOL bRoop)
{
	if(m_CurMotionNum != MotionNum)
	{
		m_CurMotionNum = MotionNum;
		g_pExecutive->GXOSetCurrentMotionIndex(m_GXOHandle,m_AniInfo[m_CurMotionNum].EngineMotionIndex);
		g_pExecutive->GXOSetCurrentFrame(m_GXOHandle, m_AniInfo[m_CurMotionNum].StartFrame);
		m_bMotionPause = FALSE;
	}
	m_bAnimationRoop = bRoop;

}
BOOL CEngineObject::Attach(CEngineObject* pToObject,char* szObjectName)
{
	return g_pExecutive->GXOAttach(m_GXOHandle,pToObject->m_GXOHandle,szObjectName);
}
void CEngineObject::Detach(CEngineObject* pChildObject)
{
	g_pExecutive->GXODetach(m_GXOHandle,pChildObject->m_GXOHandle);
}
void CEngineObject::DrawFitInViewPort(int ViewPortNum,float fFOV)
{
	g_pExecutive->SetCameraFitGXObject(m_GXOHandle,1,1000,fFOV,ViewPortNum);
	g_pExecutive->GetGeometry()->SetAmbientColor(ViewPortNum,0xffffffff);
	g_pExecutive->GetGeometry()->BeginRender(ViewPortNum,0xffff0000,BEGIN_RENDER_FLAG_DONOT_CLEAR_FRAMEBUFFER);
	g_pExecutive->GXORender(m_GXOHandle);
	g_pExecutive->GetGeometry()->EndRender();
}
void CEngineObject::SetScale(VECTOR3* pScale)
{
	g_pExecutive->GXOSetScale(m_GXOHandle,pScale);
}
void CEngineObject::GetScale(VECTOR3* pScale)
{
	g_pExecutive->GXOGetScale(m_GXOHandle,pScale);
}
BOOL CEngineObject::ChangePart(int PartNum,char* pFileName)
{
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

	BOOL rt = g_pExecutive->GXOReplaceModel(m_GXOHandle,PartNum,pFileName);

	
		DIRECTORYMGR->SetLoadMode(eLM_Root);
	return rt;
}
void CEngineObject::ApplyHeightField(BOOL bApply)
{
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
	if(bShadow)
		g_pExecutive->EnableSendShadow(m_GXOHandle);
	else
		g_pExecutive->DisableSendShadow(m_GXOHandle);
}
void CEngineObject::SetAlpha(float Alpha)
{
	DWORD dAlpha = (DWORD)(Alpha * 255);
	if(dAlpha == 0)
		dAlpha = 1;
	g_pExecutive->SetAlphaFlag(m_GXOHandle,dAlpha);
}
void CEngineObject::EnablePick()
{
	g_pExecutive->EnablePick(m_GXOHandle);
}
void CEngineObject::DisablePick()
{
	g_pExecutive->DisablePick(m_GXOHandle);
}
void CEngineObject::EnableSelfIllumin()
{
	g_pExecutive->GXOEnableSelfIllumin(m_GXOHandle);
}
void CEngineObject::DisableSelfIllumin()
{
	g_pExecutive->GXODisableSelfIllumin(m_GXOHandle);
}

DWORD CEngineObject::GetAnimationTime(DWORD MotionNum)
{
	MOTION_DESC pMotionDesc;
	g_pExecutive->GXOGetMotionDesc(m_GXOHandle,&pMotionDesc,MotionNum,0);

	DWORD aniinfo = m_AniInfo[MotionNum].EndFrame - m_AniInfo[MotionNum].StartFrame;

	DWORD frame = pMotionDesc.dwLastFrame < aniinfo ? 
					pMotionDesc.dwLastFrame : aniinfo;
	++frame;
	
	if(pMotionDesc.dwLastFrame == 0)
	{
		if(m_AniInfo[MotionNum].EndFrame == (WORD)-1)
		{
			g_pExecutive->GXOGetMotionDesc(m_GXOHandle,&pMotionDesc,m_AniInfo[MotionNum].EngineMotionIndex,0);
			frame = pMotionDesc.dwLastFrame - m_AniInfo[MotionNum].StartFrame;
		}
		else
			frame = aniinfo;
	}

	return (DWORD)(frame * gTickPerFrame);
}

void CEngineObject::RandMotionFrame()
{
	DWORD MotionNum = m_CurMotionNum;
	MOTION_DESC pMotionDesc;
	g_pExecutive->GXOGetMotionDesc(m_GXOHandle,&pMotionDesc,MotionNum,0);

	DWORD aniinfo = m_AniInfo[MotionNum].EndFrame - m_AniInfo[MotionNum].StartFrame;

	DWORD frame = pMotionDesc.dwLastFrame < aniinfo ? 
					pMotionDesc.dwLastFrame : aniinfo;
	
	if(pMotionDesc.dwLastFrame == 0)
	{
		if(m_AniInfo[MotionNum].EndFrame == (WORD)-1)
		{
			g_pExecutive->GXOGetMotionDesc(m_GXOHandle,&pMotionDesc,m_AniInfo[MotionNum].EngineMotionIndex,0);
			frame = pMotionDesc.dwLastFrame - m_AniInfo[MotionNum].StartFrame;
		}
		else
			frame = aniinfo;
	}

	DWORD rrr = rand() % frame;
	g_pExecutive->GXOSetCurrentFrame(m_GXOHandle,rrr);
}
//////////////////////////////////////////////////////////////////////////

BOOL CEngineObject::AttachWeapon(CEngineObject* pWeaponObject,char* szObjectName)
{
	BOOL rt = pWeaponObject->Attach(this,szObjectName);
	if(rt)
		m_AttachedWeapon.AddHead(pWeaponObject);

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



DWORD __stdcall MHPlayerPROC(I4DyuchiGXExecutive* pExecutive, GXMAP_OBJECT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData)
{
	CEngineObject* pObject = (CEngineObject*)pData;
	if(pObject == NULL)
		return 0;
	
	pObject->Animate(arg1);			
	if(pObject->m_pObject)
		pObject->m_pObject->Process();

	return 0;
}
