
#ifndef		_SphereCollisionTest_h_
#define		_SphereCollisionTest_h_

#include	"../../4DyuchiGXExecutive/GXObject.h"

//void	SphereCollisionTest( CoExecutive* pExecutive, CoGXObject* pThis, VECTOR3* pCandidate, VECTOR3* pOutLastVelocity, VECTOR3* pTo);
void	SphereCollisionTest( CoExecutive* pExecutive, CoGXObject* pThis, COLLISION_TEST_RESULT* pCollTestResult, VECTOR3* pTo, DWORD dwBufferIndex);

#endif
