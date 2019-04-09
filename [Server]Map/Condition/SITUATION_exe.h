#ifndef _SITUATION_EXE_H_
#define _SITUATION_EXE_H_


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Don't Touch me
// Ask taiyo if you wanna change

typedef DWORD SITUATIONIDX;
typedef BOOL (*CONDITION_FUNC) ( CObject * pObj );
extern CONDITION_FUNC globalSITUATION[];

#ifdef ENCODESITUATION
#undef ENCODESITUATION
#endif
#define ENCODESITUATION(a)	BOOL a##(CObject * p);

#include "SITUATION_Incl.h"


#ifdef ENCODESITUATION
#undef ENCODESITUATION
#endif
#define ENCODESITUATION(a)	e##a,

enum eSITUATION
{
	#include "SITUATION_Incl.h"
};

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#endif