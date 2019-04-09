
#ifndef	TriangleLib_h_
#define	TriangleLib_h_

#include	"../4DyuchiGRX_common/typedef.h"

// 평면 p로 주어진 삼각형을 자르고, p 뒤에 위치한건 날린다.
// 리턴값은 결과물 삼각형의 갯수. 하나의 삼각형을 잘라 최대 두개의 삼각형이 나올 수 있으므로(잘렸을 경우 두개가 일반적)
// pOut에 제공할 버퍼도 최소한 두개의 삼각형이 들어갈 정도가 되야한다.
// 삼각형이 잘렸을 경우 1, 아니면 2,
// 삼각형이 안잘렸을경우 0 아니면 1이 나온다. (0은 평면 뒤에 존재할 경우.)
DWORD	ClipTriangle( VECTOR3* pOut, PLANE p, VECTOR3* pV0, VECTOR3* pV1, VECTOR3* pV2 );

#endif
