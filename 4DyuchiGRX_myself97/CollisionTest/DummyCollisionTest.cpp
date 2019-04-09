
#include	"DummyCollisionTest.h"

void	DummyCollisionTest( CoExecutive* pExecutive, CoGXObject* pThis, COLLISION_TEST_RESULT* pCollTestResult, VECTOR3* pTo, DWORD dwBufferIndex)
{


//	*pCandidate	=	*pTo;

//	VECTOR3		Pos;
//	pThis->GetPosition( &Pos);
//	*pOutLastVelocity	=	*pTo	-	Pos;

	pCollTestResult->Candidate	=	*pTo;

	pCollTestResult->LastVelocity.x		=	0.0f;
	pCollTestResult->LastVelocity.y		=	0.0f;
	pCollTestResult->LastVelocity.z		=	0.0f;

	pCollTestResult->bLand	=	FALSE;

	return	;
}
