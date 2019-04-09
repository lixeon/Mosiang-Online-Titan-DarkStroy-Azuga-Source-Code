#include "SampleHeightField.h"
#include "myself97common.h"



VECTOR3*	SampleHeightFieldFromIVertex_old3( VECTOR3* pVInput, IVERTEX* pIV, DWORD dwTriCount, DWORD dwX, DWORD dwY)
{
	DWORD	x = dwX;
	DWORD	z = dwY;	// 좌표계간의 차이...
	// 충돌처리용.
	VECTOR3 tv1, tv2, tv3, lv1, lv2, rv1, rv2;
	float	t1, t2;



	DWORD	i = 0, j = 0, p = 0;		// 임시변수.

	// 바운딩 박스를 만든다.
	MAABB	box;
	memset( &box, 0, sizeof(box));
	for( i = 0; i < dwTriCount*3; i++)
	{
		// max
		if( pIV[i].x > box.Max.x)	box.Max.x = pIV[i].x;
		if( pIV[i].y > box.Max.y)	box.Max.y = pIV[i].y;
		if( pIV[i].z > box.Max.z)	box.Max.z = pIV[i].z;
		// min
		if( pIV[i].x < box.Min.x)	box.Min.x = pIV[i].x;
		if( pIV[i].y < box.Min.y)	box.Min.y = pIV[i].y;
		if( pIV[i].z < box.Min.z)	box.Min.z = pIV[i].z;
	}
	box.Max.y = 900000.0f;
	box.Min.y = -900000.0f;

	// x, z의 간격.
//	double	fXStep	=	(box.Max.x - box.Min.x) / (x-1);
//	double	fZStep	=	(box.Max.z - box.Min.z) / (z-1);


	VECTOR3*	pV = new VECTOR3[x*z];		// 결과물 버퍼.
//	memset( pV, 0, sizeof(VECTOR3)*x*z);
	memcpy( pV, pVInput, sizeof(VECTOR3)*x*z);

/*
	for( j = 0; j < z; j++)
	{
		for( i = 0; i < x; i++)
		{
					pV[i+j*x].x = box.Min.x + (float)fXStep*i;
					pV[i+j*x].y = box.Min.y;
					pV[i+j*x].z = box.Min.z + (float)fZStep*j;
		}
	}
	

	// 그냥 평면인가.?
	if( box.Max.y == box.Min.y)
	{
		for( j = 0; j < z; j++)
		{
			for( i = 0; i < x; i++)
			{
				pV[i+j*x].x = box.Min.x + (float)fXStep*i;
				pV[i+j*x].z = box.Min.z + (float)fZStep*j;
				pV[i+j*x].y = box.Min.y;
			}
		}
		goto Finish;
	}

	for( p = 0; p < dwTriCount; p++)
	{
		// 삼각형 하나를 가져와서,
		tv1.x = pIV[p*3].x;
		tv1.y = pIV[p*3].y;
		tv1.z = pIV[p*3].z;
		tv2.x = pIV[p*3+1].x;
		tv2.y = pIV[p*3+1].y;
		tv2.z = pIV[p*3+1].z;
		tv3.x = pIV[p*3+2].x;
		tv3.y = pIV[p*3+2].y;
		tv3.z = pIV[p*3+2].z;
		// 삼각형의 바운딩박스.
		MAABB tbox;
		tbox.Min.x = tbox.Max.x = tv1.x;
		tbox.Min.y = tbox.Max.y = tv1.y;
		tbox.Min.z = tbox.Max.z = tv1.z;
		for( i = 0; i < 3; i++)
		{
			if( tbox.Min.x > pIV[p*3+i].x) tbox.Min.x = pIV[p*3+i].x;
			if( tbox.Min.y > pIV[p*3+i].y) tbox.Min.y = pIV[p*3+i].y;
			if( tbox.Min.z > pIV[p*3+i].z) tbox.Min.z = pIV[p*3+i].z;

			if( tbox.Max.x < pIV[p*3+i].x) tbox.Max.x = pIV[p*3+i].x;
			if( tbox.Max.y < pIV[p*3+i].y) tbox.Max.y = pIV[p*3+i].y;
			if( tbox.Max.z < pIV[p*3+i].z) tbox.Max.z = pIV[p*3+i].z;
		}

		// 이게 걸치는 하이트필드의 영역 x, y, x갯수, y갯수 구한다.
		// x
		float	fXOffset = tbox.Min.x - box.Min.x;		
		DWORD	dwXStartIndex = int( fXOffset/fXStep);				// 시작점 인덱스. // int 타입캐스트는 버림.
		if( dwXStartIndex != 0) dwXStartIndex--;
		fXOffset	=	tbox.Max.x - box.Min.x;
		DWORD	dwXEndIndex = int( fXOffset/fXStep);			// 끝점 인덱스.
		if( dwXEndIndex != x-1)	dwXEndIndex++;

		// z
		float	fZOffset = tbox.Min.z - box.Min.z;
		DWORD	dwZStartIndex = int( fZOffset/fZStep);				// 시작점 인덱스, z
		fZOffset	=	tbox.Max.z - box.Min.z;
		DWORD	dwZEndIndex	=	int( fZOffset/fZStep);				// 끝점 인덱스. z
		if( dwZEndIndex != z-1) dwZEndIndex++;
		if( dwZStartIndex != 0) dwZStartIndex--;

		
		for( j = dwZStartIndex; j <= dwZEndIndex; j++)		// z팩트.
		{
		// for( x갯수.){
			for( i = dwXStartIndex; i <= dwXEndIndex; i++)
			{
		//	이번 직선이 삼각형이랑 만나는가.?
				lv1.x = lv2.x = pV[i+j*x].x;	// 충돌처리 할 직선들.
				lv1.z = lv2.z = pV[i+j*x].z;
				lv1.y = box.Min.y;
				lv2.y = box.Max.y;

				COLLISIONTYPE ct = TriangleMeetLineSegment( tv1, tv2, tv3, lv1, lv2, rv1, rv2, t1, t2);
				if( ct == COLLISIONTYPE_Vertex)		// 만났을 경우.
				{
					// 이게 정상.
					pV[i+j*x].x = box.Min.x + (float)fXStep*i;
					pV[i+j*x].y = rv1.y;
					pV[i+j*x].z = box.Min.z + (float)fZStep*j;
					break;
				}
				else if( ct == COLLISIONTYPE_None)
				{
					;
				}
				else if( ct == COLLISIONTYPE_Error)
				{
					_asm int 3;
				}
				else if( ct == COLLISIONTYPE_LineSegment)
				{
					_asm int 3;
				}
				else
				{
					// 뭔가 잘못된 케이스들.
					_asm int 3;
				}


			}	// i

		}	// j
	}

*/



	for( j = 0; j < z; j++)	// z요소.
	{
		for( i = 0; i < x; i++)	// x요소.
		{
//			pV[i+j*x].x = box.Min.x + (float)fXStep*i;
//			pV[i+j*x].z = box.Min.z + (float)fZStep*j;
			// 충돌처리용 라인.
			lv1.x = lv2.x = pV[i+j*x].x;
			lv1.z = lv2.z = pV[i+j*x].z;
			lv1.y = box.Min.y;
			lv2.y = box.Max.y;
			//y값이 높이값. 삼각형을 한바퀴 돌면서 충돌처리 검사.
			bool	bSet = false;
			for( p = 0; p < dwTriCount; p++)
			{
				tv1.x = pIV[p*3].x;
				tv1.y = pIV[p*3].y;
				tv1.z = pIV[p*3].z;
				tv2.x = pIV[p*3+1].x;
				tv2.y = pIV[p*3+1].y;
				tv2.z = pIV[p*3+1].z;
				tv3.x = pIV[p*3+2].x;
				tv3.y = pIV[p*3+2].y;
				tv3.z = pIV[p*3+2].z;

				COLLISIONTYPE ct = TriangleMeetLineSegment( tv1, tv2, tv3, lv1, lv2, rv1, rv2, t1, t2);
				if( ct == COLLISIONTYPE_Vertex)
				{
					// 이게 정상.
					pV[i+j*x].y = rv1.y;
					bSet = true;
					break;
				}
				else if( ct == COLLISIONTYPE_Error)
				{
					_asm int 3;
				}
				else if( ct == COLLISIONTYPE_LineSegment)
				{
					_asm int 3;
				}
				else if( ct == COLLISIONTYPE_None)
				{
					;
				}
				else
				{
					// 뭔가 잘못된 케이스들.
					_asm int 3;
				}
			}
			if( bSet == false)
			{
//				_asm int 3;
				// 이리로 빠지면...
				// 충돌 처리 함수를 float로 하니까 스케일이 커지거나, 삼각형의 밀도가 높으니까 잘 못잡아 내더라.
				// 아니면 메쉬에 구멍이 있는 경우도 이리 빠질 수 있다.
				pV[i+j*x].y = box.Min.y;
			}

		}
	}

Finish:
	// 버퍼를 하이트필드 좌표계에 맞게 재배열.
/*
	VECTOR3*	pResult = new VECTOR3[x*z];
	for( j = 0; j < z; j++)
	{
		for( i = 0; i < x; i++)
		{
//			pResult[i+j*x].x = pV[i+(z-j-1)*x].x;
//			pResult[i+j*x].y = pV[i+(z-j-1)*x].z * -1;
//			pResult[i+j*x].z = pV[i+(z-j-1)*x].y;
		}
	}

	delete[] pV;

	return	pResult;
*/

/*
	//위아래 바꾸자.
	VECTOR3*	pResult = new VECTOR3[x*z];
	for( j = 0; j < z; j++)
	{
		for( i = 0; i < x; i++)
		{
			pResult[i+j*x].x = pV[i+(z-j-1)*x].x;
			pResult[i+j*x].y = pV[i+(z-j-1)*x].y;
			pResult[i+j*x].z = pV[i+(z-j-1)*x].z;
		}
	}
	delete[] pV;
	return pResult;
*/
//	LogInit();
	for( j = 0; j < z; j++)
	{
		for( i = 0; i < x; i++)
		{
//			LogPrintf("%d, %d = %f, %f, %f", i, j, pV[i+j*x].x, pV[i+j*x].y, pV[i+j*x].z);	
		}
	}
	return pV;
}
/*
VECTOR3*	SampleHeightFieldFromIVertex_old2( VECTOR3* pVInput, IVERTEX* pIV, DWORD dwTriCount, DWORD dwX, DWORD dwZ)
{
//	LogInit();
	DWORD	i, j, p;
	DWORD	x = dwX, z = dwZ;
	VECTOR3 tv1, tv2, tv3, lv1, lv2, rv1, rv2;
	float	t1, t2, u1, v1;

	VECTOR3*	pV	=	new VECTOR3[x*z];
	memcpy( pV, pVInput, sizeof(VECTOR3) * x* z);
	// 전체 바운딩 박스.
	MAABB	WorldBox;
	WorldBox.Max.x = WorldBox.Min.x = pV[0].x;
	WorldBox.Max.y = WorldBox.Min.y = pV[0].y;
	WorldBox.Max.z = WorldBox.Min.z = pV[0].z;

//	for( i = 0; i < dwTriCount*3; i++)
	for( i = 0; i < x*z; i++)
	{
		// max
		if( pV[i].x > WorldBox.Max.x)	WorldBox.Max.x = pV[i].x;
		if( pV[i].y > WorldBox.Max.y)	WorldBox.Max.y = pV[i].y;
		if( pV[i].z > WorldBox.Max.z)	WorldBox.Max.z = pV[i].z;
		// min
		if( pV[i].x < WorldBox.Min.x)	WorldBox.Min.x = pV[i].x;
		if( pV[i].y < WorldBox.Min.y)	WorldBox.Min.y = pV[i].y;
		if( pV[i].z < WorldBox.Min.z)	WorldBox.Min.z = pV[i].z;
	}
	WorldBox.Max.y	=	900000.0f;
	WorldBox.Min.z	=	-900000.0f;

	for( j = 0; j < z; j++)	// z요소.
	{
		for( i = 0; i < x; i++)	// x요소.
		{
//			pV[i+j*x].x = WorldBox.Min.x + (float)fXStep*i;
//			pV[i+j*x].z = WorldBox.Min.z + (float)fZStep*j;
			// 충돌처리용 라인.
			lv1.x = lv2.x = pV[i+j*x].x;
			lv1.z = lv2.z = pV[i+j*x].z;
			lv1.y = WorldBox.Min.y;
			lv2.y = WorldBox.Max.y;
			//y값이 높이값. 삼각형을 한바퀴 돌면서 충돌처리 검사.
			bool	bSet = false;
			for( p = 0; p < dwTriCount; p++)
			{
				tv1.x = pIV[p*3].x;
				tv1.y = pIV[p*3].y;
				tv1.z = pIV[p*3].z;
				tv2.x = pIV[p*3+1].x;
				tv2.y = pIV[p*3+1].y;
				tv2.z = pIV[p*3+1].z;
				tv3.x = pIV[p*3+2].x;
				tv3.y = pIV[p*3+2].y;
				tv3.z = pIV[p*3+2].z;


				VECTOR3 pTri[3];
				pTri[0] = tv1;
				pTri[1] = tv2;
				pTri[2] = tv3;

				if( IsCollisionTriAndLine(pTri, &lv1, &lv2))
				{
						_asm nop;
						_asm int 3;
				}



				VECTOR3 dir;
				dir.x = lv2.x - lv1.x;
				dir.y = lv2.y - lv1.y;
				dir.z = lv2.z - lv1.z;


				if( IntersectTriangle( &lv1, &dir, &tv1, &tv2, &tv3, &t1, &u1, &v1))		// 충돌했을경우.
				{
					if( t1 <= 1.1 && t1 >= -0.1)
					{
						rv1.x = lv1.x + t1*dir.x;
						rv1.y = lv1.y + t1*dir.y;
						rv1.z = lv1.z + t1*dir.z;
						bSet = true;
						pV[i+j*x].y = rv1.y;
						break;
					}
				}


			}
			if( bSet == false)
			{
//				_asm int 3;
				// 이리로 빠지면...
				// 충돌 처리 함수를 float로 하니까 스케일이 커지거나, 삼각형의 밀도가 높으니까 잘 못잡아 내더라.
				// 아니면 메쉬에 구멍이 있는 경우도 이리 빠질 수 있다.
				pV[i+j*x].y = WorldBox.Min.y;
			}

		}
	}


	return	pV;
}
*/

VECTOR3*	SampleHeightFieldFromIVertex( VECTOR3* pVInput, IVERTEX* pIV, DWORD dwTriCount, DWORD x, DWORD z)
{
	/*
	DWORD	i, j, p;		// 임시 변수.
	VECTOR3 tv1, tv2, tv3, lv1, lv2, rv1, rv2;
	float	t1, t2, u1, v1;

	MOctree2		tree;
	DWORD*		pHandle	=	new DWORD[dwTriCount];

	VECTOR3*	pV	=	new VECTOR3[x*z];
	memcpy( pV, pVInput, sizeof(VECTOR3)*x*z);

	// 월드 바운딩 박스.
	MAABB wbox;
	wbox.Max.x = wbox.Min.x = pVInput[0].x;
//	wbox.Max.y = wbox.Min.y = pVInput[0].y;
	wbox.Max.z = wbox.Min.z = pVInput[0].z;
	for( i = 0; i < x*z; i++)
	{
		if( wbox.Max.x < pV[i].x) wbox.Max.x = pV[i].x;
		if( wbox.Max.z < pV[i].z) wbox.Max.z = pV[i].z;

		if( wbox.Min.x > pV[i].x)	wbox.Min.x = pV[i].x;
		if( wbox.Min.z > pV[i].z)	wbox.Min.z = pV[i].z;
	}
	wbox.Max.y = 900000.0f;
	wbox.Min.y = -900000.0f;

	// 아이버텍스로 트리 빌드.
	tree.Init();
	for( p = 0; p < dwTriCount; p++)
	{
		// 바운딩 박스 만들고,
		VECTOR3	objMax, objMin;
		objMax.x = objMin.x = pIV[p*3].x;
		objMax.y = objMin.y = pIV[p*3].y;
		objMax.z = objMin.z = pIV[p*3].z;
		for( i = 1; i < 3; i++)
		{
			if( objMax.x < pIV[p*3+i].x)	objMax.x = pIV[p*3+i].x;
			if( objMax.y < pIV[p*3+i].y)	objMax.y = pIV[p*3+i].y;
			if( objMax.z < pIV[p*3+i].z)	objMax.z = pIV[p*3+i].z;

			if( objMin.x > pIV[p*3+i].x)	objMin.x = pIV[p*3+i].x;
			if( objMin.y > pIV[p*3+i].y)	objMin.y = pIV[p*3+i].y;
			if( objMin.z > pIV[p*3+i].z)	objMin.z = pIV[p*3+i].z;
		}
		// 트리에 푸쉬, 핸들 얻는다.
		pHandle[p] = tree.AddStaticObject( objMax, objMin);
	}
	// 트리 빌드.
	tree.Build( wbox.Max, wbox.Min,  5, 5);

	for( j = 0; j < z; j++)
	{
		if( j == 200)
		{
			_asm nop;
		}
		for( i = 0; i < x; i++)
		{
			lv1.x = lv2.x = pV[i+j*x].x;
			lv1.z = lv2.z = pV[i+j*x].z;
			lv1.y = 900000.0f;
			lv2.y = -900000.0f;
			// 만나는 오브젝트 찝어서,
			DWORD	dwClock =  GetLowClock();
			DWORD	dwCount = tree.FindMeetObjectWithLineSegment( lv1, lv2);
			dwClock = GetLowClock() - dwClock;
			bool	bSet = false;
			// 충돌처리
			for( p = 0; p < dwCount; p++)
			{
				DWORD* pBuff = tree.GetCandidateBuffer();
				DWORD d = pBuff[p];

				tv1.x = pIV[d*3].x;
				tv1.y = pIV[d*3].y;
				tv1.z = pIV[d*3].z;
				tv2.x = pIV[d*3+1].x;
				tv2.y = pIV[d*3+1].y;
				tv2.z = pIV[d*3+1].z;
				tv3.x = pIV[d*3+2].x;
				tv3.y = pIV[d*3+2].y;
				tv3.z = pIV[d*3+2].z;

				COLLISIONTYPE ct = TriangleMeetLineSegment( tv1, tv2, tv3, lv1, lv2, rv1, rv2, t1, t2);
				if( ct == COLLISIONTYPE_Vertex)
				{
					// 이게 정상.
					pV[i+j*x].y = rv1.y;
					bSet = true;
					break;
				}
				else if( ct == COLLISIONTYPE_Error)
				{
					_asm int 3;
				}
				else if( ct == COLLISIONTYPE_LineSegment)
				{
					_asm int 3;
				}
				else if( ct == COLLISIONTYPE_None)
				{
					;
				}
				else
				{
					// 뭔가 잘못된 케이스들.
					_asm int 3;
				}

			}
			if( bSet == false)
			{
//				_asm int 3; // 하나도 못건졌군. -_-;
			}
		}
	}
	return pV;
*/
	return 0;
}