// TaticAbilityInfo.h: interface for the CTaticAbilityInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TATICABILITYINFO_H__18C96934_5351_40A5_9BB6_5CB31FAECA87__INCLUDED_)
#define AFX_TATICABILITYINFO_H__18C96934_5351_40A5_9BB6_5CB31FAECA87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaticAbilityInfo  
{
	TATIC_ABILITY_INFO m_Info;
public:
	CTaticAbilityInfo(TATIC_ABILITY_INFO* pInfo);
	virtual ~CTaticAbilityInfo();

	WORD	GetTacticAttackAbilityValue( WORD wLevel );
	WORD	GetTacticRecoverAbilityValue( WORD wLevel );
	float	GetTacticBuffRateValue( WORD wLevel );
	WORD	GetTacticBuffValue( WORD wLevel );
};

#endif // !defined(AFX_TATICABILITYINFO_H__18C96934_5351_40A5_9BB6_5CB31FAECA87__INCLUDED_)
