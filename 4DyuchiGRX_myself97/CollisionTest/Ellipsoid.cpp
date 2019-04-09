
#include "Ellipsoid.h"

#include	"../myself97common.h"


void ELLIPSOID::Create( const VECTOR3& Pivot, float fWidth, float fHeight)
{
	// 높이보다 넓이가 크면 좆된다.
	BreakIfTrue( w < 1.0f || h < 1.0f || h < w);

	// 일단 받은것을 그대로...
	w	=	fWidth;
	h	=	fHeight;

	if( w == h)
	{
		k	=	0.0f;
		hperw	=	1.0f;
	}
	else	// w > h
	{
		// 피봇으로부터 J, K까지의 거리.
		k		=	(float)sqrt( h*h - w*w);
		// h/w ?
		hperw	=	h / w;
	}

	SetPosition( Pivot);

}

// 옮기기. U와 L도 갱신된다.
void ELLIPSOID::SetPosition( const VECTOR3& Pivot)
{
	this->P	=	Pivot;
	
	// U	윗쪽 축.
	U		=	Pivot;
	U.y		+=	k;

	// L	아랫쪽축.
	L		=	Pivot;
	L.y		-=	k;

}
