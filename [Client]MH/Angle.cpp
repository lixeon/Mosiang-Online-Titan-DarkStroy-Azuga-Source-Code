// Angle.cpp: implementation of the CAngle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Angle.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CAngle::AdjustAngleIn_0to360()
{
	while(m_fAngle >= 360)
		m_fAngle -= 360;

	while(m_fAngle < 0)
		m_fAngle += 360;
}

void CAngle::SetAngleDeg(float fDeg)
{
	m_fAngle = fDeg;
	AdjustAngleIn_0to360();
}

void CAngle::SetAngleRad(float fRad)
{
	SetAngleDeg(RADTODEG(fRad));
}

void CAngle::SetTargetDeg(float fDeg)
{	
	m_fStartAngle = m_fAngle;
	m_fTargetAngle = fDeg;
	m_fChangeAmount = m_fTargetAngle - m_fStartAngle;

	if(m_fChangeAmount > 180)
		m_fChangeAmount -= 360;

	if(m_fChangeAmount <= -180)
		m_fChangeAmount += 360;

#ifdef _ANGLE_DEBUG_
	FILE* fp = fopen("./Log/debug.txt","a+");
	fprintf( fp,"%4.2f     %4.2f     %4.2f \n\r", m_fStartAngle,m_fTargetAngle,m_fChangeAmount);
	fclose(fp);
#endif
}

void CAngle::SetTargetRad(float fRad)
{
	SetTargetDeg(RADTODEG(fRad));
}

void CAngle::SetRotateDeg(float fRotateDeg)
{
	m_fStartAngle = m_fAngle;
	m_fTargetAngle = m_fAngle + fRotateDeg;
	m_fChangeAmount = fRotateDeg;
}
void CAngle::SetRotateRad(float fRotateRad)
{
	SetRotateDeg(RADTODEG(fRotateRad));
}

void CAngle::Process(float fRate)
{
	ASSERT(fRate >= 0);
	ASSERT(fRate <= 1);

	m_fAngle = m_fStartAngle + m_fChangeAmount * fRate;

	AdjustAngleIn_0to360();
}