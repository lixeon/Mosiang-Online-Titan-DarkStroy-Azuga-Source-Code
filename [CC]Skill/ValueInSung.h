// ValueInSung.h: interface for the CValueInSung class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUEINSUNG_H__478AAD75_F29C_48B5_B30A_05AA421908CB__INCLUDED_)
#define AFX_VALUEINSUNG_H__478AAD75_F29C_48B5_B30A_05AA421908CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<class T>
class CValueInSung  
{
	T Values[MAX_SUNG];
public:
	CValueInSung()
	{
	}
	virtual ~CValueInSung()
	{
	}

	T& GetValue(int sung)	{
		ASSERT(sung>0 && sung<=MAX_SUNG);
		if(!(sung>0 && sung<=MAX_SUNG))
			return Values[0];
		return Values[sung-1];
	}
	T& operator [] (int sung)
	{
		return GetValue(sung);
	}
	void SetValue(int sung,T v)
	{
		ASSERT(sung>0 && sung<=MAX_SUNG);
		Values[sung] = v;
	}
	void Init(T* vals)
	{
		if(vals)
			memcpy(Values,vals,sizeof(T)*MAX_SUNG);
		else
			memset(Values,0,sizeof(T)*MAX_SUNG);
	}
};

class MINMAXWORD
{
	WORD Minv;
	WORD Maxv;
public:
	void SetValue(WORD miv,WORD mav)
	{
		Minv = miv;
		Maxv = mav;
	}
	inline WORD GetMin()	{	return Minv;	}
	inline WORD GetMax()	{	return Maxv;	}
};

#endif // !defined(AFX_VALUEINSUNG_H__478AAD75_F29C_48B5_B30A_05AA421908CB__INCLUDED_)
