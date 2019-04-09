#pragma once
#include "../max_common/stdafx.h"
#include "chunk_id.h"
#include "scene.h"
#include "object_property.h"

#define MAXEXP_CLASS_ID		Class_ID(0x4474094c, 0x4b120f81)
extern HINSTANCE hInstance;

class CMaxExp : public SceneExport
 {
public:
	CMaxExp();
	~CMaxExp();
	int				ExtCount();					// Number of extemsions supported
	const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
	const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	const TCHAR *	AuthorName();				// ASCII Author name
	const TCHAR *	CopyrightMessage();			// ASCII Copyright message
	const TCHAR *	OtherMessage1();			// Other message #1
	const TCHAR *	OtherMessage2();			// Other message #2
	unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
	void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box
	int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);	// Export file
	BOOL			SupportsOptions(int ext, DWORD options);
	CScene*			m_pScene;
	
	Interface*		m_pInterface;
	ExpInterface*	m_pExpInterface;
	BOOL			m_bSuppressPrompts;
	DWORD			m_dwOption;
	DWORD			m_dwTotalNodeNum;
	DWORD			m_dwCurExportedNodeNum;
	TCHAR			m_uszFileName[_MAX_PATH];
	CObjectPropertyList*	m_pMeshPropertyList;

	void			EnumNode(INode* pNode);
		
public:
	CObjectPropertyList*	GetMeshPropertyList() {return m_pMeshPropertyList;}

	BOOL			Convert();
	void			DisplayObjectsList();
	void			DisplayObject(INode* node);
	BOOL			WriteSceneFile();
	BOOL			WriteMotionFile();
};

TCHAR *GetString(int id);
ClassDesc* GetMaxExpDesc();
