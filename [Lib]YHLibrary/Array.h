// Array.h: interface for the CArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARRAY_H__4B90A0FC_7328_49B8_97A9_CB29434FF05A__INCLUDED_)
#define AFX_ARRAY_H__4B90A0FC_7328_49B8_97A9_CB29434FF05A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class T>
class CArray  
{
	int MaxDataCount;	// ¹è¿­Å©±â
	T TempData;			// Àß¸øµÈ ¹è¿­ÀÎÀÚ°¡ µé¾î¿ÔÀ»¶§ ³Ñ°ÜÁÙ °ª
	T* pData;			// ½ÇÁ¦ µ¥ÀÌÅ¸
public:
	CArray()
	{
		MaxDataCount = 0;
		pData = 0;
		memset(&TempData,0,sizeof(T));
		// memsetÀ» ÇÏ±â¶§¹®¿¡ ±¸Á¶Ã¼³ª ½ºÆ®·°ÃÄ°¡ Á¦ÇÑÀûÀÌ´Ù. ¹æ½ÄÀ» ¹Ù²Ù¸é µÉµíµµ ÇÑµ¥..
	}
	~CArray()
	{
		if(pData)
			delete [] pData;
	}
	void Allocate(int count)		// ÇÒ´çÇÑ´Ù.
	{
		if(pData)
		{
			delete [] pData;
		}
		MaxDataCount = count;
		pData = new T[count];
	}
	T& GetData(int n)
	{
		if(n<0 || n>=MaxDataCount)
		{
			// ¿¡·¯ Ã³¸®.... ·Î±×¸¦ ³²±â°Å³ª ÇÑ´Ù....
			return TempData;
		}
		return pData[n];
	}
	T& operator [] (int n)
	{
		return GetData(n);
	}
	void operator = (const CArray& arr)
	{
		MaxDataCount = arr.MaxDataCount;
		TempData = arr.TempData;
		pData = (T*)arr.pData;
	}
};

#endif // !defined(AFX_ARRAY_H__4B90A0FC_7328_49B8_97A9_CB29434FF05A__INCLUDED_)
