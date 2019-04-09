
#ifndef	_MyMouseLib_h_
#define	_MyMouseLib_h_

#include "../4dyuchigrx_common/typedef.h"


//	x, y, z축으로 각각 회전시켰을때의 카메라를 리턴.
void RotateCamera( CAMERA_DESC& cd, float x, float y, float z);
//	x, y, z로 이동했을때의 카메라 리턴.
CAMERA_DESC MoveCamera( const CAMERA_DESC& cd, float x, float y, float z);




#endif