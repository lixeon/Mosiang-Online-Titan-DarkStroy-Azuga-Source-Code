

#ifndef	_MouseLib_h_
#define	_MouseLib_h_

#include "../4DyuchiGRX_common/IGeometry.h"

// 스크린상의 x,y좌표를 실제 좌표의 x,0,z로 바꾼다. y는 언제나 0
//	주의할점은, 카메라 from의 y좌표가 0 이상일때 수평선보다 위를 보고 찍으면, 시점보다 뒤쪽에서 만나는 좌표가 나온다.
VECTOR3		GetXYZFromScreenXY( I4DyuchiGXGeometry* pGeometry, DWORD dwX, DWORD dwY, DWORD dwScreenX, DWORD dwScreenY);

#endif