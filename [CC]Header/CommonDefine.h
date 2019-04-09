#ifndef __COMMONDEFINE_H__
#define __COMMONDEFINE_H__




//////////////////////////////////////////////////////////////////////////
#define USINGTON(className)			(&g_##className)
#define EXTERNGLOBALTON(className)	extern className g_##className;
#define GLOBALTON(className)		className g_##className;


#define MAKESINGLETON(classname)	static classname* GetInstance()	{	static classname gInstance;		return &gInstance;	}
#define GETINSTANCE(ClassName) static ClassName * GetInstance(){ static ClassName g_Instance; return &g_Instance; }

//////////////////////////////////////////////////////////////////////////
#define SAFE_DELETE(a)			if((a))	{ delete (a); (a)=NULL; }
#define SAFE_DELETE_ARRAY(a)	if((a))	{ delete [] (a); (a)=NULL; }
#define SAFE_RELEASE(a)			if((a))	{ (a)->Release(); (a)=NULL; }


//////////////////////////////////////////////////////////////////////////
#define START_STRING_COMPARE(src)	{	char* _compare_src_ = src;	if(0)	{
#define COMPARE(des)				}	else if(strcmp(_compare_src_,des)==0)	{	
#define END_COMPARE					}	}
//////////////////////////////////////////////////////////////////////////
#define ySWITCH(a)			{	\
								switch((a))	{		\
									case -1:	{	// COMMAND 가 -1이 될리가 없으므로...//왜없을가..
#define yCASE(a)									}	\
									break;		\
									case (a):	{
#define yDEFAULT									}	\
									break;		\
									default:	{
#define yENDSWITCH				}	break;		}	}

#define EVENT_TYPE(a)			switch(a)	{	case -1:	{
#define EVENT_END				}	return;		}
#define EVENTOF(a)				}	return;		case (a):	{


//////////////////////////////////////////////////////////////////////////
extern char _g_AssertErrorMessage[255];		// exist in ErrorMsg.cpp
//#define ASSERT(a)				{a;}


#ifdef _REGENTOOL
void WriteAssertMsg(char* str,int line,void* p);
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
// DEBUG

#ifndef _RMTOOL_
#ifdef _DEBUG
////////////////////////////////////////////////////////////////////////////////////////////////////////
#undef ASSERT
#define ASSERT(a)				{ if(0 == (a)) { WriteAssertMsg(__FILE__,__LINE__,NULL); } }
#define ASSERTMSG(a,b)			{ if(0 == (a)) { WriteAssertMsg(__FILE__,__LINE__,b); } }
#define CRITICALASSERT(a)		{ if(0 == (a)) { CriticalAssertMsg(__FILE__,__LINE__,NULL); } }


#define DEBUGMSG(a, b)			{ if(0 == (a)) OutputDebugString(b); }
#define DEBUG(a)				a
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#else	// RELEASE
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ErrorMsg.h"
#define ASSERT(a)				{ if(0 == (a)) { WriteAssertMsg(__FILE__,__LINE__,NULL); } }
#define ASSERTMSG(a,b)			{ if(0 == (a)) { WriteAssertMsg(__FILE__,__LINE__,b); } }
#define CRITICALASSERT(a)		void(0)

#define DEBUGMSG(a, b)			{ if(0 == (a)) OutputDebugString(b); }
#define DEBUG(a)				a

#endif //_DEBUG
#endif //_RMTOOL_

#ifdef _MHCLIENT_
#ifndef _ASSERTENABLE_

#undef ASSERT
#undef ASSERTMSG
#undef DEBUGMSG
#undef DEBUG

#define ASSERT(a)			void(0)
#define ASSERTMSG(a,b)		void(0)
#define DEBUGMSG(a, b)		void(0)
#define DEBUG(a)			void(0)

#endif	//_ASSERTENABLE_
#endif	//_MHCLIENT_


#ifdef _IGNORE_ASSERT_
#undef ASSERT
#undef ASSERTMSG
#undef DEBUGMSG
#undef DEBUG

#define ASSERT(a)			void(0);
#define ASSERTMSG(a,b)		void(0);
#define DEBUGMSG(a, b)		void(0);
#define DEBUG(a)			void(0);
#endif


#define POW(a, b)				pow(a, b)
#define SWAPVALUE( a, b )		{ a ^= b ^= a ^= b; }

//////////////////////////////////////////////////////////////////////////
#define gPHI					3.141592f
#define DEGTORAD(deg)			( (deg) / 180.f * gPHI )
#define RADTODEG(rad)			( (rad) / gPHI * 180.f )
#define LargeRandom(min, max)	(DWORD)((((rand()<<15)|rand())%(int)(((max)+1)-(min)))+(min))
#define random(min, max)                    ((rand()%(int)(((max) + 1)-(min)))+ (min)) 
#define LargeRandom(min, max)	(DWORD)((((rand()<<15)|rand())%(int)(((max)+1)-(min)))+(min))
//////////////////////////////////////////////////////////////////////////
#ifdef RGBA_MAKE
#undef RGBA_MAKE
#endif
#define RGBA_MAKE(r, g, b, a)   ((DWORD) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))
#define RGB_HALF(r, g, b)		((DWORD) (((r) << 16) | ((g) << 8) | (b)))
#define RGBA_MERGE(rgb, a)		((DWORD) (((a) << 24) | (rgb)))

//////////////////////////////////////////////////////////////////////////
#define CMD_ST(a)	char buffer[255]; strcpy(buffer, a); if(0){
#define CMD_CS(b)		}else if(strcmp(buffer, b) == 0){
#define CMD_EN			}


#define TEXT_DELIMITER			'^'
#define TEXT_NEWLINECHAR		'n'
#define TEXT_TABCHAR			't'
#define TEXT_SPACECHAR			's'
#define TEXT_EMPTYCHAR			'e'

#define VECTORTORAD(vec)		( (float)(atan2(vec.z, vec.x) + gPHI/2.f) )

typedef unsigned int	UINT;

//taiyo
#define max( x, y) (((x) > (y)) ? (x) : (y))
#define abs( x ) (((x) < 0) ? -(x) : (x))
#define sign( x ) ((x) > 0 ? 1 : ((x) == 0 ? 0: (-1)))


#define ABSD(n)			(((n)<0) ? (n)*=-1 : (n))

#define MAX_POINT_NUM	300

#define MAKEDWORD(a, b)		((DWORD)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))

#define WRITEDEBUGFILE(str)	{WriteDebugFile(__FILE__,__LINE__,str);}

#endif //__COMMONDEFINE_H__
