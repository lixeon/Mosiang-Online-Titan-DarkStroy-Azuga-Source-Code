/*
	GXSCHEDULE_PROC_MSG.h
	
	typedef.h 에 정의된 GXSchedulePROC 함수의 메시지를 정의한 파일.

	2002/05/30	처음 만듦.

	typedef DWORD (*GXSchedulePROC)(I4DyuchiGXExecutive* pExecutive,GXOBJECT_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData);
*/
#pragma once

#ifndef		_GXSCHEDULE_PROC_MSG_h_
#define		_GXSCHEDULE_PROC_MSG_h_

// 충돌이 일어났을때.
#define		GXSCHEDULE_PROC_MSG_COLLISION				10

struct	GXSCHEDULE_PROC_MSG_COLLISION_ARG
{
	DWORD				dwCollisionType;	// 충돌된 오브젝트의 타입.		GX_MAP_OBJECT_TYPE 중의 하나다.
	GXOBJECT_HANDLE		gxHandle;			// 충돌된 오브젝트의 핸들,		dwCollisionType에 따라 다르게 해석을... 밖에서 볼때 '오브젝트'외엔 의미가 없을지도.
	VECTOR3				vMeetPos;			// 충돌된때의 피봇.
	VECTOR3				vWhereMeet;			// 실제 충돌 모델이 충돌된 지점.
//	VECTOR3				vToCandidate;		// 충돌된 것을 기준으로 주어진 새 To 후보, 이때 새 From 후보는 vFromCandidate 이 된다.
	PLANE				MeetPlane;
};

enum	GXSCHEDULE_PROC_MSG_COLLISION_RETURN
{
	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_STOP,				// 정지하라.
	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_SLIDE				// 미끄러져라.
//	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_REFLECT			// 반사하라.
};

// 개체가 이동중 이벤트 박스와 만났을때. arg1아래의 인자가 들어온다.
#define		GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER		11

struct	GXSCHEDULE_PROC_MSG_MEET_EVENT_TRIGGER_ARG
{
	DWORD				dwEventTriggerID;			// 만난 이벤트트리거의 ID
};

#endif
