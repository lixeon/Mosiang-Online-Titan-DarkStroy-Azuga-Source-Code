//////////////////////////////////////////////////////////////////////////////
/// \file Noncopyable.h
/// \author excel96
/// \date 2003.7.23
//////////////////////////////////////////////////////////////////////////////

#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

//////////////////////////////////////////////////////////////////////////////
/// \class Noncopyable
/// \brief 복사 생성자나 대입 연산자가 사용되면 곤란한 클래스들은 이 클래스를 
/// private으로 상속하면, 컴파일 때 에러가 발생하면서 복사 생성자나 대입 
/// 연산자를 사용할 수 없다고 알려준다.
//////////////////////////////////////////////////////////////////////////////

class Noncopyable
{
protected:
	Noncopyable() {}
	~Noncopyable() {}

private:
	Noncopyable(const Noncopyable&);
	const Noncopyable& operator = (const Noncopyable&);
};

#ifdef _MSC_VER
// 복사 생성자를 생성하지 못했습니다.
#pragma warning(disable:4511)

// 컴파일러에서 주어진 클래스에 대한 대입 연산자를 생성할 수 없습니다.
#pragma warning(disable:4512)
#endif

/// 사정상 Noncopyable을 바로 상속받지 못하는 클래스에서, 
/// 복사 생성자 및 연산자를 막기 위한 매크로
#define DECLARE_NONCOPYABLE(NAME) private: \
	NAME(const NAME&) {} \
	NAME& operator = (const NAME&) { return *this; }

#endif //__NONCOPYABLE_H__