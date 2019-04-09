


#ifndef	_SampleHeightField_h_
#define	_SampleHeightField_h_

#include "../4DyuchiGRX_common/typedef.h"



//	IVERTEX 그룹에서 VECTOR3로 된 하이트필드를 샘플링 하는 함수.
//	pV는 x, z값용 헬퍼(?), pIV는 원본 ivertex, dwTriCount는 삼각형의 갯수. dwXComponentCount, dwYComponentCount 각각 샘플링할 x,y갯수.
//	리턴값은 x*y개의 버텍스값.
VECTOR3*	SampleHeightFieldFromIVertex( VECTOR3* pV, IVERTEX* pIV, DWORD dwTriCount, DWORD dwX, DWORD dwZ);




#endif