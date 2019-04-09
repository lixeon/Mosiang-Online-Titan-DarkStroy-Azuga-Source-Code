// MOTIONDESC.h: interface for the CMOTIONDESC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOTIONDESC_H__DC08BA1F_FE73_41CD_AAC2_C89F11BCD089__INCLUDED_)
#define AFX_MOTIONDESC_H__DC08BA1F_FE73_41CD_AAC2_C89F11BCD089__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////////////////////////////////////////
// taiyo 

enum eMONSTERMOTIONDAMAGE
{
	eMonsterMotionDamage_Front		= 0,
	eMonsterMotionDamage_Left		,	
	eMonsterMotionDamage_Right		,	
	eMonsterMotionDamage_Max		,
};

enum eMONSTERMOTIONDIE
{
	eMonsterMotionDie_Die			= 0,	// Á¦ÀÚ¸®¿¡¼­ Á×±â
	eMonsterMotionDie_Died			,		// Á×¾î¼­ ¸ØÃçÀÖ±â
	eMonsterMotionDie_FlyDie		,		// ³¯¶ó°¡¼­ Á×±â
	eMonsterMotionDie_FlyDied		,		// ³¯¶ó°¡¼­ Á×¾î¼­ ¸ØÃçÀÖ±â
	eMonsterMotionDie_Max			,
};


enum eMONSTERMOTIONREST
{
	eMonsterMotionRest_Down			= 0,
	eMonsterMotionRest_On			,
	eMonsterMotionRest_Up			,
	eMonsterMotionRest_Max			,
};


enum eMONSTERMOTIONN
{
	eMonsterMotionN_Stand = 0,
	eMonsterMotionN_Walk,
	eMonsterMotionN_Run,
	eMonsterMotionN_Attack,
	eMonsterMotionN_Die,
	eMonsterMotionN_Damage,
	eMonsterMotionN_Rest,

	eMonsterMotionN_Max,
};



class CMOTIONBASE
{
	friend class CMOTIONDESC;
public:
	CMOTIONBASE()
	{ num = 0; mo = NULL; }

	virtual ~CMOTIONBASE()
	{
		if(mo)
		{
			delete [] mo;
			mo = NULL;
		}
	}
	BYTE GetNum() { return num;	}
	void InitMotion(BYTE n)
	{
		num = n;
		mo = new int[n];
	}
	void AddMotion(BYTE idx, int value)
	{
		ASSERT(mo);
		ASSERT(idx < num);
		mo[idx] = value;
	}
private:
	BYTE num;
	int * mo;
};




class CMOTIONDESC  
{
	friend class CMotionManager;
public:
	CMOTIONDESC();
	virtual ~CMOTIONDESC();
	DWORD GetID()
	{
		return motionID;
	}
	void SetID(DWORD ID)
	{
		motionID = ID;
	}
	int GetMotionIDX(int MainMotion, int SubMotion = 0)
	{
		ASSERT(MainMotion<eMonsterMotionN_Max);
		ASSERT(SubMotion<motionBase[MainMotion].GetNum());
		return motionBase[MainMotion].mo[SubMotion];
	}
private:
	DWORD		motionID;
	CMOTIONBASE motionBase[eMonsterMotionN_Max];
};


#endif // !defined(AFX_MOTIONDESC_H__DC08BA1F_FE73_41CD_AAC2_C89F11BCD089__INCLUDED_)
