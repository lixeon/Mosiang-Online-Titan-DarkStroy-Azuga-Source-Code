// EngineObject.h: interface for the CEngineObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINEOBJECT_H__50E9AA05_A5A2_4CF4_BA4A_4FE8CED446F3__INCLUDED_)
#define AFX_ENGINEOBJECT_H__50E9AA05_A5A2_4CF4_BA4A_4FE8CED446F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GraphicEngine.h"
#include "../AniInfoManager.h"


//trustpak 2005/06/22
/*
enum EEngineObjectType
{
	eEngineObjectType_Character,
	eEngineObjectType_Monster,
	eEngineObjectType_Npc,
	eEngineObjectType_Weapon,
	eEngineObjectType_Effect,
	eEngineObjectType_SkillObject, 
	eEngineObjectType_Shadow,
	eEngineObjectType_Pet,	
};
*/

enum EEngineObjectType
{
	eEngineObjectType_Character,
	eEngineObjectType_Monster,
	eEngineObjectType_Npc,
	eEngineObjectType_Weapon,
	eEngineObjectType_Effect,
	eEngineObjectType_SkillObject, 
	eEngineObjectType_Shadow,
	eEngineObjectType_Pet,	
	eEngineObjectType_GameSystemObject,
	eEngineObjectType_MapObject,
	eEngineObjectType_Titan,
};

enum EAttachObjectType
{
    eAttachObjectType_None=0,
	eAttachObjectType_TargetObject,
	eAttachObjectType_withAni,
};


///
 
class CObjectBase
{
	virtual void Process() = 0;

	friend DWORD __stdcall MHPlayerPROC(I4DyuchiGXExecutive* pExecutive, GXMAP_OBJECT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData);
	friend DWORD __stdcall MHEffectPROC(I4DyuchiGXExecutive* pExecutive, GXMAP_OBJECT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData);

public:
	inline int GetEngineObjectType()	{	return m_EngineObjectType;	}
protected:
	int m_EngineObjectType;

	friend class CEngineObject;
	friend class CEngineEffect;
};

class CEngineObject  
{
#ifndef _RESOURCE_WORK_NOT_APPLY_
private:
	void*		m_pCacheListData;	
	float		m_fAlpha;

public:
	void		SetCacheListData(void* pData);
	void*		GetCacheListData(void);
#endif

protected:
	static CAniInfoManager m_staAniMgr;

	BOOL m_bApplyHeight;

	CObjectBase* m_pObject;
	
	GXOBJECT_HANDLE m_GXOHandle;

	BOOL m_bEndMotion;
	BOOL m_bAnimationRoop;
	void Animate(int animatekey);


	int m_BaseMotionNum;
	int m_CurMotionNum;
	bool m_bMotionPause;
	
	int m_NotProcessedAnimateKey;

	ANIINFO	m_CustumAniInfo;
	ANIINFO* m_pCurAniInfo;
	CAniInfo* m_AniInfo;
	
	cPtrList m_AttachedWeapon;
	cPtrList m_AttachedDress;

	cPtrList m_LinkedObject;
	CEngineObject* m_pObjectLinkTo;

public:
	CEngineObject();
	virtual ~CEngineObject();

	static void PreLoadObject(char* filename);
	
	BOOL IsInited()		{	return m_GXOHandle ? TRUE : FALSE;	}

	BOOL Init(char* filename, CObjectBase* pObject, BYTE EngineObjectType, BYTE AttachObjectType=eAttachObjectType_None);
	
	virtual void Release();

	BOOL IsEndMotion()		{	return m_bEndMotion;	}
	
	void SetEngineToCurmotion();
	
	BOOL IsVisible();

	//////////////////////////////////////////////////////////////////////////
	void SetEngObjPosition(VECTOR3* pPos);
	void GetEngObjPosition(VECTOR3* pPos);
	void SetEngObjAngle(float angleRad); // y축 기준
 	void SetEngObjAngleX(float angleRad); // x축 기준
	void SetEngObjAngleZ(float angleRad); // z축 기준
	void SetDirection(VECTOR3* pDir);
	void RotateEngObj( float x, float y, float z );
	void RotateEngObj( VECTOR3 vRot );
	float GetEngObjAngle();
	void Show();
	void Hide();
	void HideWithScheduling();
	void ChangeBaseMotion(int BaseMotionNum);
	void ChangeMotion(int MotionNum,BOOL bRoop = TRUE);
	void ChangeCustumMotion(int MotionNum,int StartFrame,int EndFrame,BOOL bRoop = TRUE);
	void RandMotionFrame();
	BOOL Attach(CEngineObject* pToObject,char* szObjectName);
	void Detach(CEngineObject* pChildObject);
	void DrawFitInViewPort(int ViewPortNum,float fFOV = gPHI/2);
	BOOL ChangePart(int PartNum,char* pFileName);
	void ApplyHeightField(BOOL bApply);
	void EnableShadow(BOOL bShadow);
	void SetScale(VECTOR3* pScale);
	void GetScale(VECTOR3* pScale);
	void SetAlpha(float Alpha);		// Alpha = [0,1]
#ifndef _RESOURCE_WORK_NOT_APPLY_

	float GetAlpha(void)		{ return m_fAlpha; }

#endif

	void EnablePick();
	void DisablePick();
	void EnableSelfIllumin();
	void DisableSelfIllumin();
	void AttachToCamera(float fDist);
	void DetachFromCamera();
	
	void InitializeIllusionEffect(DWORD MaxIllusionFrame,void* pMtl);
	void BeginIllusionEffect();
	void EndIllusionEffect();
	
	int GetCurMotion()	{	return m_CurMotionNum;	}

	//////////////////////////////////////////////////////////////////////////
	BOOL AttachWeapon(CEngineObject* pWeaponObject,char* szObjectName);
	void RemoveAllAttachedWeapon();
	BOOL AttachDress(CEngineObject* pDressObject,char* szObjectName);
	void RemoveAllAttachedDress();
	void StartWeaponIllusion(DWORD MaxIllusionFrame,void* pMtl);
	void EndWeaponIllusion();
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	// link
	void LinkTo(CEngineObject* pParent);
	//////////////////////////////////////////////////////////////////////////
	
//---material

	VOID SetMaterialIndex(DWORD dwIndex);

//	
	
	DWORD GetAnimationTime(DWORD MotionNum);
	GXOBJECT_HANDLE GetGXOHandle(VOID)	{ return m_GXOHandle; }	

	friend class CEngineLight;
	friend class CEngineCamera;
	friend DWORD __stdcall MHPlayerPROC(I4DyuchiGXExecutive* pExecutive, GXOBJECT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData);
	friend CObjectBase* GetSelectedObject(int MouseX,int MouseY);
	friend CObjectBase* GetSelectedObjectBoneCheck(int MouseX, int MouseY);
};

#endif // !defined(AFX_ENGINEOBJECT_H__50E9AA05_A5A2_4CF4_BA4A_4FE8CED446F3__INCLUDED_)
