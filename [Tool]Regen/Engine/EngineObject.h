// EngineObject.h: interface for the CEngineObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINEOBJECT_H__50E9AA05_A5A2_4CF4_BA4A_4FE8CED446F3__INCLUDED_)
#define AFX_ENGINEOBJECT_H__50E9AA05_A5A2_4CF4_BA4A_4FE8CED446F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GraphicEngine.h"

enum EEngineObjectType
{
	eEngineObjectType_Character,
	eEngineObjectType_Monster,
	eEngineObjectType_Npc,
	eEngineObjectType_Weapon,
	eEngineObjectType_Effect,
};

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
};

class CEngineObject  
{
	struct ANIINFO
	{
		WORD EngineMotionIndex;
		WORD StartFrame;
		WORD EndFrame;
	};

protected:
	BOOL m_bApplyHeight;

	CObjectBase* m_pObject;
	
	GXOBJECT_HANDLE m_GXOHandle;

	BOOL m_bEndMotion;
	BOOL m_bAnimationRoop;
	void Animate(int animatekey);


	int m_BaseMotionNum;
	int m_CurMotionNum;
	bool m_bMotionPause;


	ANIINFO* m_AniInfo;
	void LoadInfoFile(char* filename);

	
	cPtrList m_AttachedWeapon;

public:
	CEngineObject();
	virtual ~CEngineObject();

	static void PreLoadObject(char* filename);
	
	BOOL IsInited()		{	return m_GXOHandle ? TRUE : FALSE;	}

	BOOL Init(char* filename,CObjectBase* pObject,BYTE EngineObjectType);
	
	virtual void Release();

	BOOL IsEndMotion()		{	return m_bEndMotion;	}
	
	void SetEngineToCurmotion();

	//////////////////////////////////////////////////////////////////////////
	void SetEngObjPosition(VECTOR3* pPos);
	void GetEngObjPosition(VECTOR3* pPos);
	void SetEngObjAngle(float angleRad);
	float GetEngObjAngle();
	void Show();
	void Hide();
	void HideWithScheduling();
	void ChangeBaseMotion(int BaseMotionNum);
	void ChangeMotion(int MotionNum,BOOL bRoop = TRUE);
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
	void EnablePick();
	void DisablePick();
	void EnableSelfIllumin();
	void DisableSelfIllumin();
	
	int GetCurMotion()	{	return m_CurMotionNum;	}

	//////////////////////////////////////////////////////////////////////////
	BOOL AttachWeapon(CEngineObject* pWeaponObject,char* szObjectName);
	void RemoveAllAttachedWeapon();
	//////////////////////////////////////////////////////////////////////////
	
	
	DWORD GetAnimationTime(DWORD MotionNum);


	friend class CEngineLight;
	friend class CEngineCamera;
	friend DWORD __stdcall MHPlayerPROC(I4DyuchiGXExecutive* pExecutive, GXOBJECT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData);
	friend CObjectBase* GetSelectedObject(int MouseX,int MouseY);
};

#endif // !defined(AFX_ENGINEOBJECT_H__50E9AA05_A5A2_4CF4_BA4A_4FE8CED446F3__INCLUDED_)
