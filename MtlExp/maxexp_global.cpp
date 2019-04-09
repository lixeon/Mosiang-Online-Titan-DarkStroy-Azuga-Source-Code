#include "../4DyuchiGXGFunc/global.h"
#include <math.h>
#include <stdio.h>



float CalcTriAreaSize(VECTOR3* pv3Tri)
{
	
	VECTOR3 v0v2,v1v2;
	float	dist_v0v2;
	float	dist_v1v2;
	float	ang;
	float	result;

	VECTOR3_SUB_VECTOR3(&v0v2,&pv3Tri[0],&pv3Tri[2]);
	VECTOR3_SUB_VECTOR3(&v1v2,&pv3Tri[1],&pv3Tri[2]);

//	v0v2 = pv3Tri[0] - pv3Tri[2];
//	v1v2 = pv3Tri[1] - pv3Tri[2];

	ang = CalcAngle(&v0v2,&v1v2);
	
	dist_v0v2 = CalcDistance(pv3Tri+0,pv3Tri+2);
	dist_v1v2 = CalcDistance(pv3Tri+1,pv3Tri+2);

	if (ang == PI/2.0)
	{
		result = (dist_v0v2*dist_v1v2) / 2.0f;
		goto lb_return;
	}

	if (ang > PI/2.0)
	{
		result = (sin(PI-ang)*dist_v1v2*dist_v0v2) / 2.0f;
		goto lb_return;
	}
	result = (sin(ang)*dist_v1v2*dist_v0v2) / 2.0f;

lb_return:
	return result;
}
