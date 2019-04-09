#include "stdafx.h"
#include "CommonCalcFunc.h"

#ifdef _MAPSERVER_
#include "Object.h"
#endif

DWORD CalcRangeAttackPower(WORD MinChub,WORD Weapon)
{
#ifdef _JAPAN_LOCAL_

	return Weapon + MinChub + MinChub/3;

#else

	double dW = Weapon;
	double dS = MinChub;

	return (DWORD)((dW * ((dS+200)/200)*((dS+1000)/500)+dS)*0.74 + min(dS-12,25));

#endif	
}


DWORD CalcMeleeAttackPower(WORD GenGol,WORD Weapon)
{
#ifdef _JAPAN_LOCAL_

	return Weapon + GenGol + GenGol/3;

#else

	double dW = Weapon;
	double dS = GenGol;

	return (DWORD)((dW * ((dS+200)/200)*((dS+1000)/500)+dS)*0.74 + min(dS-12,25));

#endif
}

WORD GetPercent(float SeedVal,LEVELTYPE OperatorLevel,LEVELTYPE TargetLevel)
{	
	//SeedVal+{(ÀÚ½ÅÀÇ ·¹º§ - »ó´ë ·¹º§)/2.5*0.01}
	int LevelGap = OperatorLevel - TargetLevel;
	float fRate = SeedVal + LevelGap*0.025f;
	if(fRate <= 0)
		return 0;

	return (WORD)(fRate * 100);
}

BOOL CheckRandom(float SeedVal,LEVELTYPE OperatorLevel,LEVELTYPE TargetLevel)
{
	WORD Percent = GetPercent(SeedVal,OperatorLevel,TargetLevel);
	WORD randVal = rand()%100;
	if(randVal <= Percent)
		return TRUE;
	else
		return FALSE;
}

/*
//임의의 정수와 반올림 자리수를 받아 반올림 값 리턴.
//ex> 234를 둘째 자리에서 반올림 => GetIntegerRound( 234, 1 );
//ex> -12를 첫째 자리에서 반올림 => GetIntegerRound( -12, 0 );
int	GetIntegerRound(int IntVal, int Cipher)
{
	//소수점이하는 반올림 하지 않는다.
	if(Cipher<0)	return 0;

	double RoundCipher = pow(10, Cipher);

	double RoundVal = IntVal;

	RoundVal += RoundCipher * 5;

	RoundVal /= RoundCipher * 10;

	RoundVal *= RoundCipher * 10;

	return int(RoundVal);
}
*/

//정수를 받아 일의 자리 반올림 값을 리턴. 단 반올림 되지 않을 시에는 원래 값을 리턴.
WORD GetStatRound( WORD StatVal )
{
	WORD RoundVal = StatVal;

	RoundVal += 5;

	RoundVal /= 10;

	RoundVal *= 10;

	if( RoundVal > StatVal )
		return RoundVal;
	else
		return StatVal;
}

WORD GetAddStatFromRound( WORD StatVal )
{
	/*
	WORD RoundVal = StatVal;

	RoundVal += 5;

	RoundVal /= 10;

	RoundVal *= 10;

	if( RoundVal > StatVal )
		return (RoundVal - StatVal);
	else
		return 0;
*/
	StatVal %= 10;

	if( StatVal < 5 )
		return 0;
	else
		return 10 - StatVal;
}

#ifdef _MAPSERVER_
BOOL CheckSpecialAttack( float SeedVal, CObject* pOperator, CObject* pTarget )
{
	if( SeedVal > 0.f )
	{
		LEVELTYPE OperLevel = pOperator->GetLevel();
		LEVELTYPE TargetLevel = pTarget->GetLevel();
		float fSAR = 0.f;
		if( OperLevel < TargetLevel )
		{
			if( TargetLevel - OperLevel < 10  )
				fSAR = SeedVal * ( 1.f - (float)( TargetLevel - OperLevel ) / 10.f );
			else
				fSAR = 0.f;
		}
		else
		{
			fSAR = SeedVal * ( 1.f + (float)( OperLevel - TargetLevel ) / 5.f );
		}
		
		if( pTarget->GetObjectKind() & eObjectKind_Monster )
		{
			fSAR *= 3.f;
		}
		
		float fRand = (float)(rand()%100) / 100.f;
		
		if( fSAR < fRand )
			return FALSE;
		
	}
	else
	{
		return FALSE;
	}
	
	return TRUE;
}
#endif
