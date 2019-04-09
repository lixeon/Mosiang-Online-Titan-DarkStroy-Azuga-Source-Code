//////////////////////////////////////////////////////////////////////////////
/// \file MiniDumper.h
/// \author excel96
/// \date 2003.11.18
//////////////////////////////////////////////////////////////////////////////

#ifndef __MINIDUMPER_H__
#define __MINIDUMPER_H__

#ifndef __NONCOPYABLE_H__
#include "Noncopyable.h"
#endif

//////////////////////////////////////////////////////////////////////////////
/// \class MiniDumper
/// \brief 미니 덤프를 실행하기 위한 클래스. 
///
/// 특별히 하는 일은 없고, 정적 변수와 함수를 가지고 있는 네임 스페이스 역할 
/// 정도 밖에는 하는 일이 없다. 실제로 미니 덤프가 이루어지는 것은 
/// TopLevelFilter 함수다.
//////////////////////////////////////////////////////////////////////////////

class MiniDumper : private Noncopyable
{
public:
	/// \brief 덤프할 데이터의 수준
	enum DumpLevel
	{
		DUMP_LEVEL_0, ///< MiniDumpNormal을 사용
		DUMP_LEVEL_1, ///< MiniDumpWithDataSegs를 사용
		DUMP_LEVEL_2  ///< MiniDumpWithFullMemory를 사용
	};


private:
	/// 덤프 레벨.
	static DumpLevel s_DumpLevel;            

	/// 덤프 파일 이름에다가 덤프 파일이 생성된 날짜를 집어넣는가의 여부.
	static bool s_bAddTimeStamp;

	/// 덤프 파일 이름 문자열.
	static TCHAR s_szAppName[_MAX_PATH];

	/// 덤프가 발생한 이유
	static TCHAR s_szFaultReason[2048];


public:
	/// \brief 생성자
	MiniDumper(DumpLevel DL, bool bAddTimestamp=true);

	/// \brief 소멸자
	~MiniDumper();


private:
	/// \brief 예외에 대한 정보를 받아서, 미니 덤프 파일을 생성한다. 
	static LONG WINAPI TopLevelFilter(struct _EXCEPTION_POINTERS* pExPtrs);

	/// \brief 예외의 원인에 대한 문자열을 반환한다.
	static LPCTSTR GetFaultReason(struct _EXCEPTION_POINTERS* pExPtrs);
};

#endif //__MINIDUMPER_H__