#include <math.H>
#include "../4DYuchiGXGFunc/global.h"
#include "myself97Common.h"
#include "logger.h"
#include "CheckClock.h"

// a와 b가 안만나면 false;
// a와 b가 만나면 true;
bool		IsAABBMeetAABB( MAABB& a, MAABB& b)
{
	if( a.Min.x <= b.Max.x && a.Max.x >= b.Min.x)
	{
		if( a.Min.y <= b.Max.y && a.Max.y >= b.Min.y)
		{
			if( a.Min.z <= b.Max.z && a.Max.z >= b.Min.z)
			{
				// 만나는 경우.
				return true;
			}
		}
	}
	return false;
}

//	스피어와 스피어가 만나는가.?
bool		IsSphereMeetSphere( BOUNDING_SPHERE& bs1, BOUNDING_SPHERE& bs2)
{
	VECTOR3		Length;
	VECTOR3_SUB_VECTOR3( &Length, &(bs1.v3Point), &(bs2.v3Point));
	float		fLength	=	VECTOR3Length( &Length);
	if( fLength <= bs1.fRs + bs2.fRs)
		return	true;

	return	false;
}

// 스피어와 박스가 만나는가?
bool		IsSphereMeetAABB( BOUNDING_SPHERE& bs, MAABB& Box)
{
	if( Box.Max.x >= bs.v3Point.x-bs.fRs)
		if( Box.Max.y >= bs.v3Point.y-bs.fRs)
			if( Box.Max.z >= bs.v3Point.z-bs.fRs)
				if( Box.Min.x <= bs.v3Point.x+bs.fRs)
					if( Box.Min.y <= bs.v3Point.y+bs.fRs)
						if( Box.Min.z <= bs.v3Point.z+bs.fRs)
							return	true;
	return		false;
}

// 0: 걸친다. 1: 볼륨의 안에 완전히 들어간다. -1: 볼륨에서 완전히 벗어난다.
int			WhereIsAABBFromViewVolume( MAABB& b, VIEW_VOLUME& vv)
{
/*	// 바운딩 볼륨으로 임시로 ...
	VECTOR3		Mid;
	Mid.x = (b.Max.x + b.Min.x)/2;
	Mid.y = (b.Max.y + b.Min.y)/2;
	Mid.z = (b.Max.z + b.Min.z)/2;
	float		fRadius = sqrt( (Mid.x-b.Max.x)*(Mid.x-b.Max.x) + (Mid.x-b.Max.x)*(Mid.x-b.Max.x) + (Mid.x-b.Max.x)*(Mid.x-b.Max.x));
	int k = 0;
	int iD = 0;
	for( k = 0; k < 4; k++)
	{
		float fDetermin = vv.Plane[k].v3Up.x*Mid.x + vv.Plane[k].v3Up.y*Mid.y + vv.Plane[k].v3Up.z*Mid.z + vv.Plane[k].D;
		if( fDetermin > fRadius)	return -1;
		if( fDetermin < -fRadius)	iD++;
	}
	if( iD >= 4)	return 1;
	return 0;
*/
//////////////////////////////////////////////////////////////////////////

	int i = 0, j = 0, D = 0, D2 = 0;		// D는 디터민 값.
	float fResult = 0;
	// 점을 여덟개 만든다.
	VECTOR3 v[8];
	v[0].x = v[2].x = v[4].x = v[6].x = b.Min.x;
	v[1].x = v[3].x = v[5].x = v[7].x = b.Max.x;

	v[0].y = v[1].y = v[2].y = v[3].y = b.Max.y;
	v[4].y = v[5].y = v[6].y = v[7].y = b.Min.y;

	v[0].z = v[1].z = v[4].z = v[5].z = b.Max.z;
	v[2].z = v[3].z = v[6].z = v[7].z = b.Min.z;

	D2 = 0;

	


	for( j = 0; j < 4; j++)
	{
		D = 0;
		for( i = 0; i < 8; i++)
		{
			fResult =  vv.Plane[j].v3Up.x*v[i].x + vv.Plane[j].v3Up.y*v[i].y + vv.Plane[j].v3Up.z*v[i].z + vv.Plane[j].D;
			if( fResult > 0) D = D + 1;
			else if( fResult < 0) D = D - 1;
		}
		if( D >= 8)		// 점들이 완전히 평면 앞에 있으면,
		{
			return -1;	// 한 평면에서 완전히 벗어나면 -1 리턴.
		}
		if( D <= -8)	// 점들이 완전히 평면 뒤에 있으면,
		{
			D2++;
		}
	}

	// 뷰볼륨의 From이 바운딩박스 안에 있는경우 return 0;
	if( vv.From.x > b.Min.x && vv.From.x < b.Max.x)
		if( vv.From.y > b.Min.y && vv.From.y < b.Max.y)
			if( vv.From.z > b.Min.z && vv.From.z < b.Max.z)
				return 0;

	// 거리.
	D = 0;
		VECTOR3		TempVec3;
	for( i = 0; i < 8; i++)
	{
		// 시점에서 여덟 점간의 거리가 vv.fFar보다 멀면 -1, 가까우면 +1을 한다.
		if( vv.From.x > v[i].x)	TempVec3.x = vv.From.x-v[i].x;
		else					TempVec3.x = v[i].x - vv.From.x;
		if( vv.From.y > v[i].y)	TempVec3.y = vv.From.y-v[i].y;
		else					TempVec3.y = v[i].y - vv.From.y;
		if( vv.From.z > v[i].z)	TempVec3.z = vv.From.z-v[i].z;
		else					TempVec3.z = v[i].z - vv.From.z;

		fResult	=	TempVec3.x*TempVec3.x + TempVec3.y*TempVec3.y + TempVec3.z*TempVec3.z;
		float fMaxLength = vv.fFar * vv.fFar;
		if( fResult < fMaxLength)	D++;
		if( fResult > fMaxLength)	D--;
	}

	if( D <= -8)	return -1;		// 뷰볼륨에서 완전히 벗어난다.

	if( D >= 8 && D2 >= 4)
	{
		return 1;		// 뷰볼륨에 완전히 들어간다.
	}
	return 0;						// 어중간하게 걸친다.

}
int WhereISBOUNDING_BOXFromViewVolume( BOUNDING_BOX& bb, VIEW_VOLUME& vv)
{
	VECTOR3*	v	=	bb.v3Oct;
	int			iDet = 0, iDet2 = 0;
	float		fResult;
	int	i, j;
	for( j = 0; j < 4; j++)
	{
		for( i = 0; i < 8; i++)
		{
			fResult =  vv.Plane[j].v3Up.x*v[i].x + vv.Plane[j].v3Up.y*v[i].y + vv.Plane[j].v3Up.z*v[i].z + vv.Plane[j].D;
			if( fResult > 0) iDet++;
			else if( fResult < 0) iDet--;
		}
		if( iDet >= 8)		// 점들이 완전히 평면 앞에 있으면,
		{
			return -1;	// 한 평면에서 완전히 벗어나면 -1 리턴.
		}
		if( iDet <= -8)	// 점들이 완전히 평면 뒤에 있으면,
		{
			iDet2++;
		}
	}

	// 임시구현.
	if( iDet2 == 4)	return	1;
	return	0;
}

int		WhereIsAABBFromBoundingSphere( MAABB& b, BOUNDING_SPHERE& bs)
{
	// aabb의 바운딩 스피어를 먼저 구해서 대충 계산.
	VECTOR3		BoxMid;
	BoxMid.x	=	(b.Max.x + b.Min.x) / 2;
	BoxMid.y	=	(b.Max.y + b.Min.y) / 2;
	BoxMid.z	=	(b.Max.z + b.Min.z) / 2;
	float	fBoxRad	=	(float)sqrt( (BoxMid.x-b.Max.x)*(BoxMid.x-b.Max.x) + (BoxMid.y-b.Max.y)*(BoxMid.y-b.Max.y) + (BoxMid.z-b.Max.z)*(BoxMid.z-b.Max.z));

	float	fDist	=	(float)sqrt((BoxMid.x-bs.v3Point.x)*(BoxMid.x-bs.v3Point.x) + (BoxMid.y-bs.v3Point.y)*(BoxMid.y-bs.v3Point.y) + (BoxMid.z-bs.v3Point.z)*(BoxMid.z-bs.v3Point.z));
	if( fDist > bs.fRs + fBoxRad)	// 바운딩 스피어간에 안만나면..
	{
		return -1;		// 완전히 안만난다. 리턴.
	}
	// 이밑으론 만날 가능성이 있다.....
	if( fDist + fBoxRad < bs.fRs)	// 바운딩 스피어간에.. fRadius에 완전히 포함되면....
	{
		return 1;		// 완전히 포함된다.
	}

	// 일단 어중간하게 포함된다로... 나중에 구현.
	return 0;
}


BOUNDING_SPHERE		GetSphereFromAABB( MAABB& Box)
{
	BOUNDING_SPHERE		Sphere;
	VECTOR3	Mid;
	Mid.x	=	(Box.Max.x+Box.Min.x)/2;
	Mid.y	=	(Box.Max.y+Box.Min.y)/2;
	Mid.z	=	(Box.Max.z+Box.Min.z)/2;
	float fRadius	=	(float)sqrt( (Mid.x-Box.Max.x)*(Mid.x-Box.Max.x) + (Mid.y-Box.Max.y)*(Mid.y-Box.Max.y) + (Mid.z-Box.Max.z)*(Mid.z-Box.Max.z));

	Sphere.fRs			=	fRadius;
	Sphere.v3Point		=	Mid;

	return Sphere;
}


// 0: 걸친다. 1: 볼륨 안에 완전히 들어간다. -1: 볼륨에서 완전히 벗어난다.
int			WhereIsSphereFromViewVolume( BOUNDING_SPHERE& Sphere, VIEW_VOLUME& vv)
{
	int i = 0, D = 0;
	for( i = 0; i < 4; i++)
	{
		float	fDetermin	=	vv.Plane[i].v3Up.x*Sphere.v3Point.x  +  vv.Plane[i].v3Up.y*Sphere.v3Point.y + vv.Plane[i].v3Up.z*Sphere.v3Point.z + vv.Plane[i].D;
		if( fDetermin > Sphere.fRs)		return -1;
		if( fDetermin < -1*Sphere.fRs)	D++;
	}
	// 거리 클리핑.

	if( D >=4)	return 1;
	return 0;
}
/*
BOOL ClipSphereWithViewVolume(VIEW_VOLUME* pVolume,BOUNDING_SPHERE* pSphere)
{
	BOOL		bResult = FALSE;
	float		D = 0,fDist;
	VECTOR3		v3Dist;
	
	DWORD	j;
	// 뷰볼륨 클리핑.
	for( j = 0; j < 4; j++)
	{
		// 바운딩 스피어.
		D = DotProduct(&pVolume->Plane[j].v3Up,&pSphere->v3Point) + pVolume->Plane[j].D;

		if( D > pSphere->fRs)	
			goto lb_return;
	}

	// 뷰볼륨 거리 클리핑.
	VECTOR3_SUB_VECTOR3(&v3Dist,&pVolume->From,&pSphere->v3Point);

	fDist = VECTOR3Length(&v3Dist);
	if( pSphere->fRs + pVolume->fFar < fDist )	
		goto lb_return;


	bResult = TRUE;

lb_return:
	return bResult;
}
*/

/*

  2004년 7월 10일 GFunc의 함수로 대체함에 따라 주석처리
void	MakePLANE( PLANE* pP, VECTOR3* pv1, VECTOR3* pv2, VECTOR3* pv3)
{
	VECTOR3	vec1;
	VECTOR3	vec2;
//	vec1	=	v2	-	v1;
//	vec2	=	v3	-	v1;
	VECTOR3_SUB_VECTOR3( &vec1, pv2, pv1);
	VECTOR3_SUB_VECTOR3( &vec2, pv3, pv1);

	CrossProduct( &(pP->v3Up), &vec1, &vec2);
	Normalize( &(pP->v3Up), &(pP->v3Up));
	pP->D = -1 * ( DotProduct( &(pP->v3Up), pv1));

}

//	라인 From-To 위에서 점 v와 가장 가까운 점을 찾는다.
//	LineFrom과 LineTo가 같지 않다고 가정한다.


void			FindNearestVertexOnLine( VECTOR3* pOut, float* pT, VECTOR3& From, VECTOR3& To, VECTOR3& v)
{
	//	직선 From-To위의 한 점을 점 Result라고 할때, 직선 From-To와  직선 Result-v의 내적은 0이다.
	//	직선은 벡터Result = 벡터From + t*벡터FromTo
	//	t를 먼저 구하고, 그 t를 직선방정식에 대입.
	//	t = ((T-F)*(V-F)) / ((T-F)*(T-F))

//	*pT			=	((To.x-From.x)*(v.x-From.x) + (To.y-From.y)*(v.y-From.y) + (To.z-From.z)*(v.z-From.z)) / ( (To.x-From.x)*(To.x-From.x) + (To.y-From.y)*(To.y-From.y) + (To.z-From.z)*(To.z-From.z) );


//	(*pOut).x	=	From.x + (*pT) * (To.x - From.x);
//	(*pOut).y	=	From.y + (*pT) * (To.y - From.y);
//	(*pOut).z	=	From.z + (*pT) * (To.z - From.z);
//	VECTOR3_SUB_VECTOR3( pOut, &To, &From);
//	VECTOR3_MULEQU_FLOAT( pOut, *pT);
//	VECTOR3_ADDEQU_VECTOR3( pOut, &From);

	VECTOR3		Velocity;
	VECTOR3_SUB_VECTOR3( &Velocity, &To, &From);
	VECTOR3		FV;
	VECTOR3_SUB_VECTOR3( &FV, &v, &From);

	*pT		=	DotProduct( &Velocity, &FV) / DotProduct( &Velocity, &Velocity);

	*pOut	=	Velocity;
	VECTOR3_MULEQU_FLOAT( pOut, *pT);
	VECTOR3_ADDEQU_VECTOR3( pOut, &From);
}

// 스피어와 직선이 어디서 만나는가.? 리턴0이면 안만나고, 1이면 한점에서, 2면 두점에서 만난다.

  */
// 이놈은 trymoveto 제대로 되면 없어질 놈이다.
DWORD	WhereDidSphereMeetLine( VECTOR3* OUT pCandidate1, float* OUT pT1, VECTOR3* OUT pCandidate2, float* OUT pT2, VECTOR3* IN pLFrom, VECTOR3* IN pLTo, VECTOR3* IN pSPivot, float fSRadius)
{
	// 일단 라인이 아닌 점, 같은 점이 두개 들어왔는가.?
	if( IsVECTOR3Same( pLFrom, pLTo))
	{
		// 그냥 안만난걸로 하고 리턴.
		return 0;
	}

	// 근의 공식 -_-;   (-b (+-) 루트(b*b-4ac) )/(2a)
	VECTOR3		V;
	float		tV;
	FindNearestVertexOnLine( &V, &tV, pLFrom, pLTo, pSPivot);
	float		fPV	=	CalcDistance( &V, pSPivot);

	if( (fPV - fSRadius) >= 0.5 )			//	아무것도 만나지 않는다.
	{
		return	0;		// 만난 점 없다 리턴.
	}

	// 하나, 또는 두개의 점에서 만난다. 판별은 b제곱-4ac로.
	float	t1;		// 첫번째 점.
	float	t2;		// 두번째 점.
	float	a, b, c;


	a	=	((*pLTo).x-(*pLFrom).x)*((*pLTo).x-(*pLFrom).x)	+	((*pLTo).y-(*pLFrom).y)*((*pLTo).y-(*pLFrom).y)	+	((*pLTo).z-(*pLFrom).z)*((*pLTo).z-(*pLFrom).z);
	b	=	2*( ((*pLTo).x-(*pLFrom).x)*((*pLFrom).x-(*pSPivot).x) + ((*pLTo).y-(*pLFrom).y)*((*pLFrom).y-(*pSPivot).y) + ((*pLTo).z-(*pLFrom).z)*((*pLFrom).z-(*pSPivot).z) );
	c	=	((*pLFrom).x-(*pSPivot).x)*((*pLFrom).x-(*pSPivot).x)	+	((*pLFrom).y-(*pSPivot).y)*((*pLFrom).y-(*pSPivot).y)	+	((*pLFrom).z-(*pSPivot).z)*((*pLFrom).z-(*pSPivot).z) - fSRadius*fSRadius;

	if( ( b*b - 4*a*c)	<=	0.1)		// 0.1은 0의 근사치... 
	{
		if( (b*b - 4*a*c) <	-0.1)
		{
			_asm int 3;
		}
	}
	float	fTemp	=	(float)sqrt(b*b-4*a*c);
	t1	=	(-1*b - fTemp)	/	(2*a);
	t2	=	(-1*b + fTemp)	/	(2*a);

	if( t1 < 0)
	{
		if( t1 >= -0.1)	t1	=	0.0f;		// t1이 허용치 이내에서 0보다 작은가.?
//		else	_asm int 3;
	}

	*pT1	=	t1;
	(*pCandidate1).x	=	(*pLFrom).x + t1*((*pLTo).x - (*pLFrom).x);
	(*pCandidate1).y	=	(*pLFrom).y + t1*((*pLTo).y - (*pLFrom).y);
	(*pCandidate1).z	=	(*pLFrom).z + t1*((*pLTo).z - (*pLFrom).z);

	*pT2	=	t2;
	(*pCandidate2).x	=	(*pLFrom).x + t2*((*pLTo).x - (*pLFrom).x);
	(*pCandidate2).y	=	(*pLFrom).y + t2*((*pLTo).y - (*pLFrom).y);
	(*pCandidate2).z	=	(*pLFrom).z + t2*((*pLTo).z - (*pLFrom).z);


	if( (fPV - fSRadius) < 0.5 && (fPV-fSRadius) >= -0.5)
	{
		// 한점에서 만난다고 볼 수 있다. 
		return 1;
	}
	return 2;
}


BOOL	IsVECTOR3Same( VECTOR3* pV1, VECTOR3* pV2)
{
	float	x	=	(*pV1).x - (*pV2).x;
	float	y	=	(*pV1).y - (*pV2).y;
	float	z	=	(*pV1).z - (*pV2).z;
	if( x >= (fFloatMin/2) && x < (fFloatMin/2) && y >= (fFloatMin/2) && y < (fFloatMin/2) && z >= (fFloatMin/2) && z < (fFloatMin/2) )	return TRUE;
	return FALSE;
}


void		GetTriMiddle( VECTOR3* pMid, VECTOR3* pTri)
{
	(*pMid).x		=	(pTri[0].x + pTri[1].x + pTri[2].x) / 3;
	(*pMid).y		=	(pTri[0].y + pTri[1].y + pTri[2].y) / 3;
	(*pMid).z		=	(pTri[0].z + pTri[1].z + pTri[2].z) / 3;
}

// 바운딩 스피어와 바운딩 캡슐이 만나는가.?
// bc의 시작점과 끝점을 F, T, 직선상의 한 점을 X, bs의 중점을 P라 하면,
// X = F + t(T-F); (T-F)(X-P) = 0;
int		BOUNDING_CAPSULE_Meet_BOUNDING_SPHERE( BOUNDING_CAPSULE& bc, BOUNDING_SPHERE& bs)
{
	VECTOR3		X;

	VECTOR3		TF	=	bc.v3To		-	bc.v3From;		// T-F
	VECTOR3		FP	=	bc.v3From	-	bs.v3Point;		//	F-P

	float		t	=	-1.0f * DotProduct(&TF, &FP) / DotProduct( &TF, &TF);

	// X == ??
	float		fLength;
	if( t > 1)		// to를 벗어났으므로 to와 스피어의 중점간 거리 계산.
	{
		X		=	bc.v3To;
	}
	else if( t < 0)		// from을 벗어났으므로 from과 스피어의 중점간 거리 계산.
	{
		X		=	bc.v3From;
	}
	else //  0 <= t <= 1
	{
//		X		=	bc.v3From	+	TF * t;
		VECTOR3_MUL_FLOAT( &X, &TF, t);
		VECTOR3_ADDEQU_VECTOR3( &X, &(bc.v3From));

	}
	VECTOR3_SUB_VECTOR3( &X, &X, &(bs.v3Point));
	fLength		=	VECTOR3Length( &X);


	if( fLength > bs.fRs + bc.fRadius )		// 완전히 안만나는 경우.
		return	-1;

	if( fLength + bs.fRs > bc.fRadius)		// 걸치면서 만나는 경우.
		return	0;

	// 완전히 만나는 경우.
	return	1;
}

// 삼각형과, 그 평면위에 있을거라고 가정하는 버텍스가 만나는가.
// 리턴값은 만나면 TRUE이고, WhereMeet는 벡터01*x + 벡터02*y로 나타내는 텍스쳐좌표같은거.
//BOOL	IsTriMeetVertex( VECTOR2* pWhereMeet, VECTOR3* pTri, VECTOR3* pV)
BOOL	IsTriMeetVertex( VECTOR3* pTri, VECTOR3* pV)
{

	VECTOR3		OnLine;
	float		fWhere;
	PLANE		p;
	float		fTemp;
	//	0,1
	FindNearestVertexOnLine( &OnLine, &fWhere, &(pTri[0]), &(pTri[1]), &(pTri[2]));
	p.v3Up	=	pTri[2]	-	OnLine;
	p.D		=	-1.0f * DotProduct( &(p.v3Up), &OnLine);

	fTemp	=	DotProduct( &(p.v3Up), pV)	+	p.D;
	if( fTemp < 0.0f)
		return	FALSE;

	//	1,2
	FindNearestVertexOnLine( &OnLine, &fWhere, &(pTri[1]), &(pTri[2]), &(pTri[0]));
	p.v3Up	=	pTri[0]	-	OnLine;
	p.D		=	-1.0f * DotProduct( &(p.v3Up), &OnLine);

	fTemp	=	DotProduct( &(p.v3Up), pV)	+	p.D;
	if( fTemp < 0.0f)
		return	FALSE;


	//	2,0
	FindNearestVertexOnLine( &OnLine, &fWhere, &(pTri[2]), &(pTri[0]), &(pTri[1]));
	p.v3Up	=	pTri[1]	-	OnLine;
	p.D		=	-1.0f * DotProduct( &(p.v3Up), &OnLine);

	fTemp	=	DotProduct( &(p.v3Up), pV)	+	p.D;
	if( fTemp < 0.0f)
		return	FALSE;


	return	TRUE;

/*
	float&	s	=	pWhereMeet->x;
	float&	t	=	pWhereMeet->y;

	PLANE		p;		// 거리를 구할때 쓸 평면.

	VECTOR3		OnLine;	// 삼각형 엣지 위의 점.
	float		f1;		// 삼각형의 엣지에서 맞은편 꼭지점까지의 거리.
	float		f2;		// 삼각형의 엣지에서 점 v까지의 거리.
	
	float		fTemp;

	// s	(0,1)
	// 0,2상의 평면 p구한다.
	FindNearestVertexOnLine2( &OnLine, &fTemp, pTri, &(pTri[2]), &(pTri[1]));
	VECTOR3_SUB_VECTOR3( &p.v3Up, &(pTri[1]), &OnLine);
	Normalize( &p.v3Up, &p.v3Up);
	p.D	=	-1.0f * DotProduct( &p.v3Up, &OnLine);
	
	f1	=	DotProduct( &p.v3Up, &pTri[1])	+	p.D;
	f2	=	DotProduct( &p.v3Up, pV)	+	p.D;

	s	=	f2 / f1;
	
	// t	(0,2)
	// 0,1상의 평면 p 구한다.
	FindNearestVertexOnLine2( &OnLine, &fTemp, pTri, &(pTri[1]), &(pTri[2]));
	VECTOR3_SUB_VECTOR3( &p.v3Up, &pTri[2], &OnLine);
	Normalize( &p.v3Up, &p.v3Up);
	p.D	=	-1.0f * DotProduct( &p.v3Up, &OnLine);

	f1	=	DotProduct( &p.v3Up, &pTri[2])	+	p.D;
	f2	=	DotProduct( &p.v3Up, pV)	+	p.D;

	t	=	f2 / f1;

	// s와 t가 유효한가?( 삼각형 안에 들어가는가) 판별.
	if( s > 1.0f || s < 0.0f || t > 1.0f || t < 0.0f || s+t > 1.0f)
		return	FALSE;
	
	return	TRUE;
*/
}



//	 주어진 삼각형이 평면의 어디에 있는가.?
//	명백히 앞에 있으면 2, 명백히 뒤에 있으면 -2, 평면이 겹쳐있는데 노말이 같은경우 1, 노말이 반대인 경우 -1, 
//	평면과 삼각형이 교차하고 있으면 0 리턴.
int		WhereIsTriangleFromPLANE(VECTOR3 *pTri, PLANE *pPlane)
{
	DWORD	i;
	VECTOR3		Normal, vec1, vec2;
	
	DWORD	dwFront = 0;		// 평면의 앞에 잇는 점 갯수.
	DWORD	dwBack = 0;			// 뒤에 있는 점 갯수.
	DWORD	dwOn = 0;			// 평면 위에 있는 점 갯수.
	
	float	d;
	for( i = 0; i < 3; i++)
	{
//		d	=	pPlane->v3Up.x*pTri[i].x + pPlane->v3Up.y*pTri[i].y + pPlane->v3Up.z*pTri[i].z + pPlane->D;
		d	=	DotProduct( &(pPlane->v3Up), &pTri[i]) + pPlane->D;
		if( d > 0.01f )			dwFront++;
		else if( d < -0.01f )	dwBack++;
		else					dwOn++;
	}
	
	if( dwOn == 3)	// 점 세개가 다 평면위에 있는경우, 노말을 판별해서 -1, 1을 리턴.
	{
		VECTOR3_SUB_VECTOR3( &vec1, &(pTri[1]), &(pTri[0]));
		VECTOR3_SUB_VECTOR3( &vec2, &(pTri[2]), &(pTri[0]));
		CrossProduct(	&Normal, &vec1, &vec2);
		// 평면 벡터와 삼각형 노말의 곱.
		float	dot	=	DotProduct( &Normal, &(pPlane->v3Up));
		if( dot > 0)	return 1;
		else if( dot < 0)	return -1;
		else	_asm int 3;
	}
	
	if( dwFront > 0)	// 점이 최소 하나가 앞에 있을경우.
	{
		if( dwBack == 0)	// 점들이 뒤에 하나도 없는경우. 
		{
			return 2;
		}
		// 점이 하나라도 뒤에 있으면, cross.
		return 0;
	}
	
	// 이까지 오는건 점들이 모두 뒤에 있다는 뜻.
	return	-2;
}

// pTri로 주어진 삼각형의 AABB를 구한다.
// 혹시 부피가 없다면 FALSE를 리턴.
BOOL	MakeAABBFromTri( MAABB* pOutBox, VECTOR3* pTri)
{
	pOutBox->Max	=	pOutBox->Min	=	pTri[0];

	DWORD	i;
	for( i = 1; i < 3; i++)
	{
		VECTOR3*	pV	=	&(pTri[i]);
		if( pOutBox->Max.x < pV->x)
				pOutBox->Max.x	=	pV->x;
		if( pOutBox->Max.y < pV->y)
				pOutBox->Max.y	=	pV->y;
		if( pOutBox->Max.z < pV->z)
				pOutBox->Max.z	=	pV->z;

		if( pOutBox->Min.x > pV->x)
				pOutBox->Min.x	=	pV->x;
		if( pOutBox->Min.y > pV->y)
				pOutBox->Min.y	=	pV->y;
		if( pOutBox->Min.z > pV->z)
				pOutBox->Min.z	=	pV->z;
	}

	if( pOutBox->Max == pOutBox->Min)
	{
		return	FALSE;
	}

	return	TRUE;
}

int	BoundingCapsuleMeetPlane( BOUNDING_CAPSULE* pBC, PLANE* pPlane)
{
#ifdef	_DEBUG
	// 반지름이 0?
	if( pBC->fRadius < 0.1f)
		_asm int 3;
	// 바운딩 캡슐이 아니라 그냥 원이면.?
	if( pBC->v3From == pBC->v3To)
		_asm int 3;
#endif

	// 일단 평면의 노말 벡터와, 바운딩캡슐의 방향벡터(?)가 수직일 경우, 반지름을 가지고 판별한다.
	VECTOR3		vBCVec;
	VECTOR3_SUB_VECTOR3( &vBCVec, &pBC->v3To, &pBC->v3From);
	VECTOR3		vBCDir;
	Normalize( &vBCDir, &vBCVec);
	float	fDotPlaneNormalBCDir	=	DotProduct( &vBCDir, &pPlane->v3Up);
	if( fDotPlaneNormalBCDir <= 0.0001f && fDotPlaneNormalBCDir >= -0.0001f)		// == 0.0f
	{
		float	fDistFromPlane	=	DotProduct( &pBC->v3From, &pPlane->v3Up)	+	pPlane->D;
		if( fDistFromPlane < 0.0f)
			fDistFromPlane *= -1.0f;
		// 반지름을 벗어나면,
		if( fDistFromPlane > pBC->fRadius)
		{
			return	-1;				// 아예 만나지 않는다.
		}
		// 반지름 안으로 들어간다면,
		if( fDistFromPlane < pBC->fRadius - 0.1f)				// 약간의 임계치.
		{
			return	1;				// 완전히 만난다.
		}

		// 딱 접하는 경우라면,?
		return	0;

	}
	
	// 일반적인 경우들.
	// bc의 vector위의 한점 X = F + (T-F)*t 와 X*N + d = r 에서,
	// t = (r - d - F*N) / ((T-F)*N)
	float	fFN	=	DotProduct(&pBC->v3From, &pPlane->v3Up);

	VECTOR3		TF	=	pBC->v3To;
	VECTOR3_SUBEQU_VECTOR3( &TF, &pBC->v3From);
	float	fTFN	=	DotProduct( &TF, &pPlane->v3Up);
	
	// 자, t값이 나왔다.
	float	t	=	(pBC->fRadius - pPlane->D - fFN) / ( fTFN);

	// 안쪽으로 확실히 들어왔으면,?
//	if( t > 0.0f && t < 1.0f)
	if( t < 1.0f)
	{
		return	1;
	}

	// 만약 BC의 Vector에서 범위가 벗어났으면,?
	if( t > 1.0f || t < 0.0f)
	{
		return	-1;
	}

	return	0;			// t가 0인거나 t가 1이면 딱 만나는것이므로,
	
}
