/**********************************************************************
 *<
	FILE: anmexp.h

	DESCRIPTION:	Includes for Plugins

	CREATED BY:

	HISTORY:

 *>	Copyright (c) 2000, All Rights Reserved.
 **********************************************************************/

#ifndef __ANMEXP__H
#define __ANMEXP__H

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "motion.h"
#include "object_property.h"


#define CAnmExp_CLASS_ID	Class_ID(0x586d6c06, 0x3d00492f)


class CAnmExp : public SceneExport 
{
public:
	static HWND hParams;

	int				ExtCount();					// Number of extensions supported
	const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
	const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	const TCHAR *	AuthorName();				// ASCII Author name
	const TCHAR *	CopyrightMessage();			// ASCII Copyright message
	const TCHAR *	OtherMessage1();			// Other message #1
	const TCHAR *	OtherMessage2();			// Other message #2
	unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
	void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

	BOOL SupportsOptions(int ext, DWORD options);
	int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);
private:
	BOOL			m_bSuppressPrompts;
	DWORD			m_dwOption;
	CMotion*		m_pMotion;
	Interface*		m_pInterface;
	CObjectPropertyList	m_motionObjPropertyList;
	TCHAR			m_uszFileName[_MAX_PATH];
	void			EnumNode(INode* pNode);
	void			DisplayMotionObject(INode* node);

public:
	CObjectPropertyList*	GetMotionPropertyList()		{return &m_motionObjPropertyList;}
	
	void						DisplayMotionObjectsList();

	BOOL			WriteMotionFile();



	
	
	//Constructor/Destructor

	CAnmExp();
	~CAnmExp();		

};

extern TCHAR *GetString(int id);

class CAnmExpClassDesc:public ClassDesc2 
{
public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new CAnmExp(); }
	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
	SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID() { return CAnmExp_CLASS_ID; }
	const TCHAR* 	Category() { return GetString(IDS_CATEGORY); }

	const TCHAR*	InternalName() { return _T("CAnmExp"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }				// returns owning module handle
	char*			GetRsrcString(long hoho);
	

};



extern HINSTANCE hInstance;

#endif // __ANMEXP__H
