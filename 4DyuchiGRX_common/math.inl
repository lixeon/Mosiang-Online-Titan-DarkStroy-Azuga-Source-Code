#pragma once

#include <math.h>

struct VECTOR2
{
	float		x;
	float		y;
};
struct MATRIX4;

struct VECTOR3
{
	float	x;
	float	y;
	float	z;

	inline	VECTOR3		operator +(const VECTOR3 &v);
	inline	VECTOR3		operator -(const VECTOR3 &v);
	inline	VECTOR3		operator *(const float	&f);
	inline	VECTOR3		operator /(const float	&f);
	inline	float		operator *(const VECTOR3 &v);			// dot.
	inline	BOOL		operator==( const VECTOR3& v);

	/*
	VECTOR3		operator +(const VECTOR3 &v3);
	VECTOR3		operator -(const VECTOR3 &v3);
	VECTOR3		operator *(const VECTOR3 &v3);
	VECTOR3		operator /(const VECTOR3 &v3);

	VECTOR3		operator +(const float a);
	VECTOR3		operator -(const float a);
	VECTOR3		operator *(const float a);
	VECTOR3		operator /(const float a);

	void			operator +=(const VECTOR3 &v3);
	void			operator -=(const VECTOR3 &v3);
	void			operator *=(const VECTOR3 &v3);
	void			operator *=(const float a);
	void			operator /=(const float a);
	void			operator /=(const VECTOR3 &v3);
	*/
};

inline VECTOR3		VECTOR3::operator +(const VECTOR3 &v3)
{
	VECTOR3	result;
	result.x	=	this->x + v3.x;
	result.y	=	this->y + v3.y;
	result.z	=	this->z + v3.z;
	return	result;
}

inline VECTOR3		VECTOR3::operator -(const VECTOR3 &v3)
{
	VECTOR3		result;
	result.x	=	this->x -	v3.x;
	result.y	=	this->y	-	v3.y;
	result.z	=	this->z	-	v3.z;
	return	result;
}

inline VECTOR3		VECTOR3::operator *(const float	&f)
{
	VECTOR3		r;
	r.x	=	this->x	*	f;
	r.y	=	this->y *	f;
	r.z	=	this->z	*	f;
	return	r;
}

inline VECTOR3		VECTOR3::operator /(const float	&f)
{
	VECTOR3		r;
	r.x	=	this->x / f;
	r.y	=	this->y / f;
	r.z	=	this->z / f;
	return	r;
}
// dot.
inline float		VECTOR3::operator *(const VECTOR3 &v)
{
	float		r;
	r	=	this->x * v.x;
	r	+=	this->y * v.y;
	r	+=	this->z * v.z;
	return		r;
}

inline	BOOL		VECTOR3::operator==( const VECTOR3& v)
{
	BOOL	bResult;
	if( this->x == v.x && this->y == v.y && this->z == v.z)	bResult	=	TRUE;
	else	bResult	=	FALSE;
	return	bResult;
	//	if( (*this-v)*(*this-v) >0.25)	return FALSE;
	//	return	TRUE;
	// 거리 구해서 
	/*
	VECTOR3		a;
	if( float(int(this->x + 0.5)) == float(int(v.x	+	0.5)))
	{
	if( float(int(this->y + 0.5)) == float(int(v.y	+	0.5)))
	{
	if( float(int(this->z + 0.5))	==	float(int(v.z	+	0.5)))
	{
	return	TRUE;
	}
	}
	}
	return	FALSE;
	*/
}

struct VECTOR4
{
	float x;
	float y;
	float z;
	float w;
	/*
	void		Clear();
	VECTOR4		operator +(const VECTOR4 &v4);
	VECTOR4		operator -(const VECTOR4 &v4);
	VECTOR4		operator *(const VECTOR4 &v4);
	VECTOR4		operator /(const VECTOR4 &v4);

	void			operator =(const VECTOR4 &v4);
	void			operator +=(const VECTOR4 &v4);
	void			operator -=(const VECTOR4 &v4);
	void			operator *=(const VECTOR4 &v4);
	void			operator *=(const float a);
	void			operator /=(const float a);
	void			operator /=(const VECTOR4 &v4);
	VECTOR4()		{x = 0; y = 0; z = 0; w = 1;}*/

};
struct QUARTERNION
{
	float x;
	float y;
	float z;
	float w;
	QUARTERNION()	{x = 0; y = 0; z = 0; w = 1;}

};
struct MATRIX3
{
	float	_11;
	float	_12;
	float	_13;
	float	_21;
	float	_22;
	float	_23;
	float	_31;
	float	_32;
	float	_33;
};
struct MATRIX4
{
	float	_11;
	float	_12;
	float	_13;
	float	_14;

	float	_21;
	float	_22;
	float	_23;
	float	_24;

	float	_31;
	float	_32;
	float	_33;
	float	_34;

	float	_41;
	float	_42;
	float	_43;
	float	_44;

};

