// Angle.h: interface for the CAngle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANGLE_H__C300D251_9B7A_4545_B079_73F320D9873C__INCLUDED_)
#define AFX_ANGLE_H__C300D251_9B7A_4545_B079_73F320D9873C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAngle
{
	float m_fAngle;

	float m_fTargetAngle;
	float m_fStartAngle;
	
	float m_fChangeAmount;

	void AdjustAngleIn_0to360();
	
public:
	void SetAngleDeg(float fDeg);
	void SetAngleRad(float fRad);
	
	inline float ToRad()
	{
		return DEGTORAD(m_fAngle);
	}
	inline float ToDeg()
	{
		return m_fAngle;
	}
	
	void SetTargetDeg(float fDeg);
	void SetTargetRad(float fRad);

	void SetRotateDeg(float fRotateDeg);
	void SetRotateRad(float fRotateRad);
	
	float GetTargetAngleDeg()	{	return m_fTargetAngle;	}
	float GetTargetAngleRad()	{	return DEGTORAD(m_fTargetAngle);	}
	float GetChangeAmountDeg()	{	return m_fChangeAmount;	}
	float GetChangeAmountRad()	{	return DEGTORAD(m_fChangeAmount);	}

	void Process(float fRate);	// 시작부터 끝나는 때까지의 비율을 0~1까지의 범위로 지정
};

#endif // !defined(AFX_ANGLE_H__C300D251_9B7A_4545_B079_73F320D9873C__INCLUDED_)
