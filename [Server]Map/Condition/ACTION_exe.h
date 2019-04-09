// ACTION_exe.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTION_EXE_H__38646EFE_AACD_4856_BCBA_D4F24A21AFD6__INCLUDED_)
#define AFX_ACTION_EXE_H__38646EFE_AACD_4856_BCBA_D4F24A21AFD6__INCLUDED_


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Don't Touch me
// Ask taiyo if you wanna change
// = Read only

typedef DWORD ACTIONIDX;
typedef void (*ACTION_FUNC) ( CObject * pObj );
extern ACTION_FUNC globalACTION[];

#ifdef ENCODEACTION
#undef ENCODEACTION
#endif
#define ENCODEACTION(a)	void a##(CObject * p);

#include "ACTION_Incl.h"


#ifdef ENCODEACTION
#undef ENCODEACTION
#endif
#define ENCODEACTION(a)	e##a,

enum eACTION
{
	#include "ACTION_Incl.h"
};

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!






#endif // !defined(AFX_ACTION_EXE_H__38646EFE_AACD_4856_BCBA_D4F24A21AFD6__INCLUDED_)
