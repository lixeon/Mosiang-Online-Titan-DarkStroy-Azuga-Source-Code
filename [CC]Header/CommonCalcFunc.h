#ifndef _COMMONCALCFUNC_H_
#define _COMMONCALCFUNC_H_

DWORD CalcRangeAttackPower(WORD Minchub,WORD Weapon);
DWORD CalcMeleeAttackPower(WORD Gengol,WORD Weapon);

BOOL CheckRandom(float SeedVal,LEVELTYPE OperatorLevel,LEVELTYPE TargetLevel);
WORD GetPercent(float SeedVal,LEVELTYPE OperatorLevel,LEVELTYPE TargetLevel);

//SW060906 Ω≈±‘∆Í(Ω∫≈»π›ø√∏≤)
//int	GetIntegerRound(int IntVal, int Cipher);
WORD GetStatRound(WORD StatVal);
WORD GetAddStatFromRound(WORD StatVal);

#ifdef _MAPSERVER_
BOOL CheckSpecialAttack( float SeedVal, CObject* pOperator, CObject* pTarget );
#endif

#endif
