#include "stdafx.h"
#include "ACTION_exe.h"

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Don't Touch me
// Ask taiyo if you wanna change
#ifdef ENCODEACTION
#undef ENCODEACTION
#endif
#define ENCODEACTION(a)	a,


ACTION_FUNC globalACTION[]=
{
	#include "ACTION_Incl.h"
};

#ifdef ENCODEACTION
#undef ENCODEACTION
#endif
#define ENCODEACTION(a)	void a##(CObject * p)

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!




// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// > Custom code list - 2003. 10. 30. taiyo
// Add user callback function (for SITUATION)
// 
// ex)	
// 1. Add ENCODESITUATION( yourCallBackFunctionName ) code in File"SITUATION_incl.h"
// 2. Your ENCODESITUATION( yourCallBackFunctionName ){ } function is implemented
// 3. Write your situation code!!
// --------------------------------------------------------------------------------------------

#include "StateMachinen.h"
#include "Object.h"
#include "BossMonsterManager.h"
#include "ConditionManager.h"

ENCODEACTION(DoACTION)
{
//	GSTATEMACHINE.SetState((CObject *)BOSSMONSTERMNGR->GetBoss(), eMA_REST);
//	CONDITIONMGR->RemoveInList(1);
}