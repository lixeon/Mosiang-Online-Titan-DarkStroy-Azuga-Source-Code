// DamageNumber.h: interface for the CDamageNumber class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAMAGENUMBER_H__16CD2C8A_D6B3_4267_8435_F7B47E40EC64__INCLUDED_)
#define AFX_DAMAGENUMBER_H__16CD2C8A_D6B3_4267_8435_F7B47E40EC64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cImageSelf.h"

#define MAX_DAMAGE_JARISU	7

enum eDamageNumberKind
{
	eDNK_Yellow,
	eDNK_Green,
	eDNK_Red,

	eDNK_Max,
};

class CDamageNumber  
{

	struct NumberData
	{
		cImageSelf* pImage;
		VECTOR2 spos;
		void Draw(float fAlpha,VECTOR2* pos);
		void SetImage(cImageSelf* p,VECTOR2* pos);
	};

	static cImageSelf* m_pNumberImage[eDNK_Max];
//	static cImageSelf* m_pMissImage;
	static VECTOR2	   m_CriticalImgSize;
	static cImageSelf* m_pCriticalImage;
	static cImageSelf* m_pDodgeImage;

	DWORD m_CreatedTime;
	VECTOR3 m_Position;
	VECTOR3 m_PositionCritical;
	VECTOR3 m_Direction;
	float m_fVelocity;
	float m_fAlpha;
	
	BOOL m_bCritical;
	BOOL m_bDecisive;
	BOOL m_bDodge;
	
	BOOL m_bDraw;
	NumberData m_Numbers[MAX_DAMAGE_JARISU];		// 데미지는 최대 5자리까지
	DWORD m_Damage;
	DWORD m_Jarisu;	
public:
	static BOOL LoadImage();		// 맨처음 초기화
	static void DeleteImage();		// 맨나중 지울때...
	
	CDamageNumber();
	virtual ~CDamageNumber();

	void SetDodge(VECTOR3* pPos,VECTOR3* pVelocity);
	void SetDamage(DWORD Damage,VECTOR3* pPos,VECTOR3* pVelocity,BYTE nDamageNumberKind,BOOL bCritical,BOOL bDecisive);
	BOOL Render();		// fAlpha = [0,1]
	void SetAlpha(float fAlpha)		{	m_fAlpha = fAlpha;	}
	void Clear()			{	m_bDraw = FALSE;	}
};

#endif // !defined(AFX_DAMAGENUMBER_H__16CD2C8A_D6B3_4267_8435_F7B47E40EC64__INCLUDED_)
