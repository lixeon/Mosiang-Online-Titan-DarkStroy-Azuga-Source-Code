

#include	"../../4DyuchiGXExecutive/GXObject.h"
#include	"../myself97common.h"
#include	"../CheckClock.h"
#include	"../../4DyuchiGXGFunc/global.h"
#include	"../../4DyuchiGXExecutive/GXEventTriggerObject.h"

#include	"CollisionTestMovingEllipsoidMeetTriangle.h"
 
//typedef DWORD (__stdcall *CollisionTestCallBackProcedure)(COLLISION_RESULT*	pResult);
//// 지오메트리에서 충돌처리때 쓰일 콜백.
DWORD	__stdcall CollisionCallBack(COLLISION_RESULT* pResult);


BOOL AAEllipsoidMeetAAEllipsoid( COLLISION_RESULT* pOut, AAELLIPSOID* pMoving, VECTOR3* pVelocity, AAELLIPSOID* pTarget);

BOOL AAEllipsoidMeetTriangle( COLLISION_RESULT* pOut, AAELLIPSOID* pMoving, VECTOR3* pVelocity, VECTOR3* pTri );

BOOL TestMovingSphereMeetPLANE2( float* pTOut, BOUNDING_SPHERE* pSphere, VECTOR3* pVelocity, VECTOR3* pTri);

// 타원체위의 주어진 한 점을 만나고 있는 평면을 만든다. return값이 false일 경우 타원체에서 너무 멀리 떯어져 있을 경우.
BOOL FindPlaneOnEllipsoidWithVertex( PLANE* pOut, AAELLIPSOID* pEllipsoid, VECTOR3* pVertex);


/*
	타원체기준으로 삼각형과 함께 스피어 공간으로 만든다.
	점, 선, 면 테스트를 거친 뒤,
	타원체 공간으로 삼각형과 타원체를 되돌리고, 구해진 t값으로 접평면을 구한 다음 리턴 한다.

	타원체를 스피어로 만들어서 계산 한 값중, 접점과 그때의 t값만 유효하고,
	접평면은 타원체 공간에서 직접 계산해야 맞을 듯 하다.
	
*/
BOOL AAEllipsoidMeetTriangle( COLLISION_RESULT* pOut, AAELLIPSOID* pMoving, VECTOR3* pVelocity, VECTOR3* pTri )
{
	MOVING_ELLIPSOID	MovingEllipsoid;
	MovingEllipsoid.From		=	pMoving->P;
	MovingEllipsoid.fHeight		=	pMoving->h;
	MovingEllipsoid.fWidth		=	pMoving->w;
	MovingEllipsoid.Velocity	=	*pVelocity;

	COLLISION_RESULT	CollisionInfo;
	BOOL	bResult	=	CollisionTestMovingEllipsoidMeetTriangle( &CollisionInfo, &MovingEllipsoid, pTri);


	// 만난게 있는가.?
	if( bResult == FALSE)
	{
		return	FALSE;
	}

	*pOut	=	CollisionInfo;

	return	TRUE;			// 끝.
}

void AAEllipsoidCollisionTest( CoExecutive* pExecutive, CoGXObject* pThis, COLLISION_TEST_RESULT* pCollTestResult, VECTOR3* pTo, DWORD dwBufferIndex)
{

	// 이 오브젝트의 위치.
	VECTOR3			GXObjectPosition;
	pThis->GetPosition( &GXObjectPosition);

	// pTo와 같은가.? 움직이지 않으면 리턴.
	VECTOR3			ObjVelocity;
	VECTOR3_SUB_VECTOR3( &ObjVelocity, pTo, &GXObjectPosition);
	float			fObjVelocity	=	VECTOR3Length( &ObjVelocity);
	if( fObjVelocity == 0.0f)
	{

		pCollTestResult->LastVelocity.x			=	0.0f;
		pCollTestResult->LastVelocity.y			=	0.0f;
		pCollTestResult->LastVelocity.z			=	0.0f;

		pCollTestResult->Candidate		=	*pTo;
		pCollTestResult->bLand			=	FALSE;
		return	;
	}


	// 프로시져.
	GXSchedulePROC	pProc	=	pThis->GetProcedure();


	// 이 오브젝트의 충돌 모델이 될 타원체 만든다.
	BOUNDING_VOLUME*	pObjBV;
	pObjBV		=	pThis->GetBoundingVolume();
	AAELLIPSOID	ObjEllip;
	ObjEllip.h		=	pObjBV->fHeight;
	ObjEllip.w		=	pObjBV->fRadius;
	ObjEllip.P		=	pObjBV->vPivot		+	GXObjectPosition;

	// pTo에 피봇 적용.
	VECTOR3			To	=	*pTo;
	VECTOR3_ADDEQU_VECTOR3( &To, &pObjBV->vPivot);

	// STM의 충돌메쉬에 충돌 시도.
	I3DStaticModel*			pSTM		=		pExecutive->GetGXMap()->GetBaseModel();
	VECTOR3			CollResult;
	VECTOR3			LastVelocity;
	BOOL			bLanding;

	pSTM->CollisionTestAAELLIPSOID( &CollResult, &LastVelocity, &bLanding, CollisionCallBack, &ObjEllip, &To, dwBufferIndex);
	
	pCollTestResult->LastVelocity	=	LastVelocity;
	float	fLastVelocity	=	VECTOR3Length( &LastVelocity);
	if( fLastVelocity == 0.0f)
		_asm nop;
	/*
	 *	여기서는 CollisionCallBack에서 ....
	 */

	// 충돌처리 마무리.
	
	// 충돌 했구나~
	// 피봇 교정하고,
//	Candidate		=	CollResult		-	pObjBV->vPivot;	// 중간 피봇만 참조.
	
	// 트리 타고 이벤트트리거 긁어온다.
	BOUNDING_SPHERE		TriggerBS;
	TriggerBS.fRs		=	1.0f;
	TriggerBS.v3Point	=	CollResult;
	MClipper*	pClipper	=	pExecutive->GetGXMap()->GetClipper();
	pClipper->FindNearObject( TriggerBS);
	CoGXEventTrigger**			ppTrigger;		// 시간이 지난 후 버퍼를 따로 만들어서 카피.
	DWORD	dwTrigger		=	pClipper->GetTargetBuffer( (CGXMapObject***)&ppTrigger, GX_MAP_OBJECT_TYPE_TRIGGER);
	if( dwTrigger)
	{
		// 중점만 테스트. 각 면 안에 중점이 들어가는가를 판단한다.
		// 트리거박스는 바깥쪽을 향하고 있으므로 -방향을 체크.
		DWORD	i;
		for( i = 0; i < dwTrigger; i++)
		{
			DWORD	j;
			PLANE*		pPlane	=	ppTrigger[i]->GetPlanes();
			for( j = 0; j < 6; j++)
			{
				float	fIn	=	DotProduct(&pPlane[j].v3Up, &CollResult)	+	pPlane[j].D;
				if( fIn >	0.0f)
					goto	lbBreakTrigger;
			}
			
			// 다 돌았으면 만났나.? 객체의 프로시저 한번 호출 해준다.
			if( pProc)
			{
				GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER_ARG		TriggerArg;
				TriggerArg.dwEventTriggerID		=	ppTrigger[i]->GetID();
				DWORD	dwEventTriggerReturn	=	pProc( pExecutive, GXOBJECT_HANDLE(pThis), GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER, DWORD( &TriggerArg), 0, pThis->GetData());
			}

lbBreakTrigger:
				_asm nop;

		}
	}


	// 위치를 다시 맞춰주고 리턴.
//	*pCandidate	=	Candidate	-	pObjBV->vPivot;
	pCollTestResult->Candidate		=	CollResult	-	pObjBV->vPivot;
	pCollTestResult->bLand			=	bLanding;

	
}

DWORD	__stdcall CollisionCallBack(COLLISION_RESULT* pResult)
{
	_asm nop;
	return	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_SLIDE;		// 일단. 원래는 GXOBJECT의 프로시저 호출해서 물어본다.
}
