// ToolMouseStatus.cpp: implementation of the CToolMouseStatus class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "4DyuchiGXMapEditor.h"
#include "MToolMouseStatus.h"
#include "../4dyuchigrx_common/typedef.h"
#include "../4DyuchiGXGFunc/global.h"

//#include "tool.h"

//extern CTool* g_pTool;


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//	마우스 이벤트.
#define		MOUSE_NONE					0
#define		MOUSE_ROTATE				1
#define		MOUSE_MOVEHORIZON			2
#define		MOUSE_MOVEVERTICAL			3


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MToolMouseStatus::MToolMouseStatus()
{
	memset((char*)this+4, 0, sizeof( MToolMouseStatus)-4);
}

MToolMouseStatus::~MToolMouseStatus()
{

}

BOOL MToolMouseStatus::Initialize( I4DyuchiGXGeometry* pGeometry)
{
	fMoveHorizonStep		=	1.0f;
	fMoveVerticalStep		=	10.0f;
	fRotateStep		=	0.2f;	// Shift에 의해 상태가 달라지는 회전, 이동 속도값.
	m_pGeometry = pGeometry;
	bDoNotRotate		=	FALSE;
	return TRUE;
}
bool MToolMouseStatus::MoveMouse(DWORD dwFlag, int iMouseX, int iMouseY)
{
	if( this->m_pGeometry == 0)
	{
		__asm int 3
		// 랜더러가 아직 설정 안됬다.
	}
	this->iMouseX	=	iMouseX;
	this->iMouseY	=	iMouseY;

	//	컨트롤키.
	this->bCtrlDown	=	false;
	if( dwFlag & MK_CONTROL)
	{
		this->bCtrlDown	=	true;
	}
	//	시프트키.
	this->bShiftDown	=	false;
	if( dwFlag & MK_SHIFT)
	{
		this->bShiftDown	=	true;
//		fMoveHorizonStep		=	5.0f;
//		fMoveVerticalStep		=	50.0f;
//		fRotateStep		=	0.5f;	// Shift에 의해 상태가 달라지는 회전, 이동 속도값.
	}
	else
	{
//		fMoveHorizonStep		=	1.0f;
//		fMoveVerticalStep		=	10.0f;
//		fRotateStep		=	0.2f;	// Shift에 의해 상태가 달라지는 회전, 이동 속도값.
	}
	//	마우스 왼쪽 버튼.
	this->bLButtonDown	=	false;
	if( dwFlag & MK_LBUTTON)
	{
		this->bLButtonDown	=	true;
	}
	//	마우스 중간 버튼.
	this->bMButtonDown	=	false;
	if( dwFlag & MK_MBUTTON)
	{
		this->bMButtonDown	=	true;
	}
	//	마우스 오른쪽 버튼.
	this->bRButtonDown	=	false;
	if( dwFlag & MK_RBUTTON)
	{
		this->bRButtonDown	=	true;
	}


	// 모드에 따른 스위치, 함수 콜.
	switch( this->dwMouseMode)
	{
		case	MOUSE_NONE:
				iOldMouseX = iMouseX;
				iOldMouseY = iMouseY;
			// 마우스 오른쪽 버튼만 눌린 경우.
			// 회전 모드로.
			if( !bLButtonDown && !bMButtonDown && bRButtonDown && !bCtrlDown && !bShiftDown)	
			{
				dwMouseMode = MOUSE_ROTATE;	
			}
			// 마우스 오른쪽 버튼과 컨트롤이 눌려졌을 경우.
			//	수평 이동 모드로.
			else if( !bLButtonDown && !bMButtonDown && bRButtonDown && bCtrlDown && !bShiftDown)	
			{
				dwMouseMode	=	MOUSE_MOVEHORIZON;
			}
			// 마우스 중간 버튼만 눌린경우.
			//	수직(높이) 이동 모드로.
			else if( !bLButtonDown && !bMButtonDown && bRButtonDown && !bCtrlDown && bShiftDown)	
			{
				dwMouseMode	=	MOUSE_MOVEVERTICAL;
			}

			return this->MouseNone();
			break;
		case	MOUSE_ROTATE:
			if( bLButtonDown || bMButtonDown || !bRButtonDown || bCtrlDown || bShiftDown)	// 어떻게든 상태가 달라지면 모드 해제.
			{
				dwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseRotate();
			}

			break;
		case	MOUSE_MOVEHORIZON:
			if( bLButtonDown || bMButtonDown || !bRButtonDown || !bCtrlDown || bShiftDown)	// 어떻게든 상태가 달라지면 모드 해제.
			{
				dwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseMoveHorizon();
			}
			break;
		case	MOUSE_MOVEVERTICAL:
			if( bLButtonDown || bMButtonDown || !bRButtonDown || bCtrlDown || !bShiftDown)	// 어떻게든 상태가 달라지면 모드 해제.
			{
				dwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseMoveVertical();
			}
			break;
		default:
			break;
	}

return false;
}

bool MToolMouseStatus::MouseNone()
{
	// 아무것도 안한다. -_-;
	return false;
}

bool MToolMouseStatus::MouseRotate()
{
	if( bDoNotRotate == TRUE)	return true;		// 마우스 로테이트 금지 모드일경우...

	float	x = float(iMouseX - iOldMouseX) * fRotateStep;
	float	y = float(iMouseY - iOldMouseY) * fRotateStep;
	VECTOR3	Rotate;
	Rotate.x = -y;
	Rotate.y = x;
	Rotate.z = 0;


	m_pGeometry->RotateCamera(&Rotate);

	iOldMouseX	=	iMouseX;
	iOldMouseY	=	iMouseY;

	return true;
}

bool MToolMouseStatus::MouseMoveHorizon()
{
	float	x	=	float(iMouseX - iOldMouseX) * fMoveHorizonStep * -1;
	float	y	=	float(iMouseY - iOldMouseY) * fMoveHorizonStep * -1;
	
	VECTOR3	Rot	=	this->m_pGeometry->GetCameraAngle();

	VECTOR3	To;
	To.x	=	(-1 * x * (float)cos(Rot.y * 3.1415926f / 180.0f) *10 + y * (float)sin( Rot.y * 3.1415926f / 180.0f)*10);
	To.y	=	0;
	To.z	=	x * (float)sin(Rot.y * 3.1415926f / 180.0f) *10 + y * (float)cos( Rot.y * 3.1415926f / 180.0f)*10;

	m_pGeometry->MoveCamera( &To);

	iOldMouseX	=	iMouseX;
	iOldMouseY	=	iMouseY;
						
	return true;
}

bool MToolMouseStatus::MouseMoveVertical()
{
	VECTOR3		To;
	To.x	=	0;
	To.y	=	float(iMouseY - iOldMouseY) * fMoveVerticalStep * -1;
	To.z	=	0;
	m_pGeometry->MoveCamera(&To);

	iOldMouseX	=	iMouseX;
	iOldMouseY	=	iMouseY;

	return true;
}


void	MToolMouseStatus::DoNotRotate(void)
{
	this->bDoNotRotate	=	TRUE;
}


void	MToolMouseStatus::AllowRotate(void)
{
	this->bDoNotRotate	=	FALSE;
}

void	MToolMouseStatus::PushCameraAngleAndPos(DWORD dwStackNum)
{
	CAMERA_DESC		Desc;
	m_pGeometry->GetCameraDesc( &Desc);
	this->pCameraStackPos[dwStackNum]		=	Desc.v3From;
	this->pfCameraStackFromTo[dwStackNum]	=	CalcDistance( &(Desc.v3From), &(Desc.v3To));
	this->pCameraStackAngle[dwStackNum]		=	m_pGeometry->GetCameraAngle();
}

void	MToolMouseStatus::PopCameraAngleAndPos(DWORD dwStackNum)
{
	VECTOR3	To	=	this->pCameraStackPos[dwStackNum];
	To.z	=	To.z + this->pfCameraStackFromTo[dwStackNum];
	VECTOR3 Up;
	Up.x = Up.z = 0;
	Up.y = 1;
	m_pGeometry->SetCamera( &(this->pCameraStackPos[dwStackNum]), &To, &Up,100.0f,80000.0f);
	m_pGeometry->SetCameraAngle( &(this->pCameraStackAngle[dwStackNum]) );
}
