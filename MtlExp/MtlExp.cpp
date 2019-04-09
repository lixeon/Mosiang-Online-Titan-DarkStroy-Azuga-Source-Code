/**********************************************************************
 *<
	FILE: MtlExp.cpp

	DESCRIPTION:	Appwizard generated plugin

	CREATED BY: 

	HISTORY: 

 *>	Copyright (c) 2000, All Rights Reserved.
 **********************************************************************/

#include "MtlExp.h"
#include "material.h"
#include <stdmat.h>
#include "Dialog.h"
#include "../4DyuchiGRX_Common/IGeometry.h"

#define MTLEXP_CLASS_ID	Class_ID(0x62d9fab0, 0x40cc324e)




class MtlKeeper {
public:
	BOOL	AddMtl(Mtl* mtl);
	int		GetMtlID(Mtl* mtl);
	int		Count();
	Mtl*	GetMtl(int id);

	Tab<Mtl*> mtlTab;
};

BOOL MtlKeeper::AddMtl(Mtl* mtl)
{
	if (!mtl) {
		return FALSE;
	}

	int numMtls = mtlTab.Count();
	for (int i=0; i<numMtls; i++) {
		if (mtlTab[i] == mtl) {
			return FALSE;
		}
	}
	mtlTab.Append(1, &mtl, 25);

	return TRUE;
}

int MtlKeeper::GetMtlID(Mtl* mtl)
{
	int numMtls = mtlTab.Count();
	for (int i=0; i<numMtls; i++) {
		if (mtlTab[i] == mtl) {
			return i;
		}
	}
	return -1;
}

int MtlKeeper::Count()
{
	return mtlTab.Count();
}

Mtl* MtlKeeper::GetMtl(int id)
{
	return mtlTab[id];
}





class MtlExp : public SceneExport {
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
		void			PreProcess(INode* node, int& nodeCount);
		CMaterial*		AllocMaterial();
		DWORD			AddMaterial(CMaterial** ppMaterial,Mtl* pMtl,DWORD dwSubMtlIndex);
		void			ReleaseMaterials();
		BOOL			WriteFile(char* szFileName);
		
		//Constructor/Destructor

		MtlExp();
		~MtlExp();		

		MtlKeeper	mtlList;
		CMaterial*	m_pBaseMaterial;
		DWORD		m_dwMaterialNum;
		DWORD		m_dwMaxMtlIndex;

};


class MtlExpClassDesc:public ClassDesc2 {
	public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new MtlExp(); }
	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
	SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID() { return MTLEXP_CLASS_ID; }
	const TCHAR* 	Category() { return GetString(IDS_CATEGORY); }

	const TCHAR*	InternalName() { return _T("MtlExp"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }				// returns owning module handle



};



static MtlExpClassDesc MtlExpDesc;
ClassDesc2* GetMtlExpDesc() { return &MtlExpDesc; }


BOOL CALLBACK MtlExpOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static MtlExp *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (MtlExp *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;
	}
	return FALSE;
}


//--- MtlExp -------------------------------------------------------
MtlExp::MtlExp()
{
	m_pBaseMaterial = NULL;
	m_dwMaterialNum = 0;
	m_dwMaxMtlIndex = 0;
}

MtlExp::~MtlExp() 
{

}

int MtlExp::ExtCount()
{
	//TODO: Returns the number of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR *MtlExp::Ext(int n)
{		
	//TODO: Return the 'i-th' file name extension (i.e. "3DS").
	return _T("MTL");
}

const TCHAR *MtlExp::LongDesc()
{
	//TODO: Return long ASCII description (i.e. "Targa 2.0 Image File")
	return _T("Material Status File");
}
	
const TCHAR *MtlExp::ShortDesc() 
{			
	//TODO: Return short ASCII description (i.e. "Targa")
	return _T("Material Status");
}

const TCHAR *MtlExp::AuthorName()
{			
	//TODO: Return ASCII Author name
	return _T("yuchi");
}

const TCHAR *MtlExp::CopyrightMessage() 
{	
	// Return ASCII Copyright message
	return _T("");
}

const TCHAR *MtlExp::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *MtlExp::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int MtlExp::Version()
{				
	//TODO: Return Version number * 100 (i.e. v3.01 = 301)
	return 100;
}

void MtlExp::ShowAbout(HWND hWnd)
{			
	// Optional
}

BOOL MtlExp::SupportsOptions(int ext, DWORD options)
{
	// TODO Decide which options to support.  Simply return
	// true for each option supported by each Extension 
	// the exporter supports.

	return TRUE;
}


int	MtlExp::DoExport(const TCHAR *name,ExpInterface *ei,Interface *ip, BOOL suppressPrompts, DWORD options)
{
	char	filename[_MAX_PATH];
	_tcscpy(filename,name);

	DWORD	dwResult = 0;

	DWORD result = (int)CreatePanel(hInstance,ip->GetMAXHWnd());
	if (!result)
		return 0;

	int	nTotalNodeCount = 0;
	PreProcess(ip->GetRootNode(), nTotalNodeCount);
		

	int numMtls = mtlList.Count();
	CMaterial* pMaterial;
	for (int i=0; i<numMtls; i++) 
	{
		if (!mtlList.GetMtl(i)->NumSubMtls())
			AddMaterial(&pMaterial,mtlList.GetMtl(i),0);

		for (int j=0; j<mtlList.GetMtl(i)->NumSubMtls(); j++)
		{
			AddMaterial(&pMaterial,mtlList.GetMtl(i),j);
		}
	}
	if (!m_dwMaterialNum)
		goto lb_return;

	WriteFile(filename);
	ReleaseMaterials();
	dwResult = 1;

lb_return:

	ReleaseModel();
	return dwResult;
}

void MtlExp::PreProcess(INode* node, int& nodeCount)
{
	nodeCount++;
	
	// Add the nodes material to out material list
	// Null entries are ignored when added...
	mtlList.AddMtl(node->GetMtl());

	// For each child of this node, we recurse into ourselves 
	// and increment the counter until no more children are found.
	for (int c = 0; c < node->NumberOfChildren(); c++) {
		PreProcess(node->GetChildNode(c), nodeCount);
	}
}
CMaterial* MtlExp::AllocMaterial()
{
	CMaterial* pMtl = new CMaterial;
	pMtl->m_pNextMaterial = m_pBaseMaterial;
	m_pBaseMaterial = pMtl;
	m_dwMaterialNum++;
	
	return pMtl;

}
DWORD MtlExp::AddMaterial(CMaterial** ppMaterial,Mtl* pMtl,DWORD dwSubMtlIndex)
{
	I3DModel*	pModel = GetRootModel();
	DWORD dwResult = 0xffffffff;
	CMaterial	Material;
	CMaterial*	pCur = m_pBaseMaterial;

	StdMat*		pStdMtl;
	Texmap*		pTex;
	DWORD		i;
	DWORD		dwRootIndex;

	if (!pMtl)
		goto lb_return;

	if (!pMtl->NumSubMtls())
		goto lb_mtl_set;

	pMtl = pMtl->GetSubMtl(dwSubMtlIndex);

lb_mtl_set:
	dwRootIndex = pModel->GetMaterialIndex(pMtl->GetName());
	if (0xffffffff == dwRootIndex)
		goto lb_return;

	
	if (pMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
	{
		pStdMtl = (StdMat*)pMtl;
		Material.SetAmbient(pStdMtl->GetAmbient(0).r,pStdMtl->GetAmbient(0).g,pStdMtl->GetAmbient(0).b,0.0f);
		Material.SetDiffuse(pStdMtl->GetDiffuse(0).r,pStdMtl->GetDiffuse(0).g,pStdMtl->GetDiffuse(0).b,0.0f);
		Material.SetSpecular(pStdMtl->GetSpecular(0).r,pStdMtl->GetSpecular(0).g,pStdMtl->GetSpecular(0).b,0.0f);
		Material.SetShine(pStdMtl->GetShininess(0));
		Material.SetShineStrength(pStdMtl->GetShinStr(0));
		Material.SetTransparency(pStdMtl->GetXParency(0));

		switch (pStdMtl->GetTransparencyType()) 
		{
		case TRANSP_FILTER:
			Material.SetTranspFilter();
			break;
		case TRANSP_SUBTRACTIVE:
			Material.SetTranspSubtractive();
			break;
		case TRANSP_ADDITIVE:		
			Material.SetTranspAdditive();
			break;
		}

		if (pStdMtl->GetTwoSided()) 
			Material.Set2Side();

		if (pStdMtl->GetSelfIllumColorOn())
			Material.SetSelfIllunum();
		
	}
	else 
	{	
		Material.SetAmbient(pMtl->GetAmbient(0).r,pMtl->GetAmbient(0).g,pMtl->GetAmbient(0).b,0.0f);
		Material.SetDiffuse(pMtl->GetDiffuse(0).r,pMtl->GetDiffuse(0).g,pMtl->GetDiffuse(0).b,0.0f);
		Material.SetSpecular(pMtl->GetSpecular(0).r,pMtl->GetSpecular(0).g,pMtl->GetSpecular(0).b,0.0f);
		Material.SetShine(pMtl->GetShininess(0));
		Material.SetShineStrength(pMtl->GetShinStr(0));
		Material.SetTransparency(pMtl->GetXParency(0));
		if (pMtl->GetSelfIllumColorOn())
			Material.SetSelfIllunum();
	}
	for (i=0; i<pMtl->NumSubTexmaps(); i++)
	{
		pTex = pMtl->GetSubTexmap(i);
		if (!pTex)
			continue;

		if (pTex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00) )
			Material.SetTexture((TEXTURE_MAP_TYPE)i,((BitmapTex*)pTex)->GetMapName());
		
	}
lb_insert:
	// 매터리얼이 중복되지 않도록 추가하고 그 인덱스를 리턴
	if (!m_pBaseMaterial)
	{
		pCur = AllocMaterial();
		goto lb_set_mtl;
	}
	
	while (pCur)
	{
		if (!memcmp(pCur,&Material,sizeof(CMaterial)-8))
		{
			dwResult = pCur->GetIndex();
			goto lb_return;
		}
		pCur = pCur->m_pNextMaterial;

	}
	pCur = AllocMaterial();

lb_set_mtl:
	
	Material.SetIndex(dwRootIndex);
	Material.SetName(pMtl->GetName());
	memcpy(pCur,&Material,sizeof(CMaterial)-4);
	dwResult = Material.GetIndex();

	if (m_dwMaxMtlIndex < dwResult)
		m_dwMaxMtlIndex = dwResult;

	*ppMaterial = pCur;
lb_return:
	return dwResult;
}
struct MTL_HEADER
{
	DWORD	dwVersion;
	DWORD	dwMtlNum;
	DWORD	dwMaxMtlIndex;

};
BOOL MtlExp::WriteFile(char* szFileName)
{
	// 파일버젼
	// 매터리얼 갯수
	// 매터리얼 최대인덱스 
	// 매터리얼...
	// ..
	// ..
	// 매터리얼...

	FILE* fp = fopen(szFileName,"wb");
	if (!fp)
		return FALSE;

	MTL_HEADER	header;
	header.dwVersion = 0x00000001;
	header.dwMtlNum = m_dwMaterialNum;
	header.dwMaxMtlIndex = m_dwMaxMtlIndex;
	
	fwrite(&header,sizeof(header),1,fp);

	CMaterial*	pCur = m_pBaseMaterial;
	while (pCur)
	{
		pCur->WriteFile(fp);
		pCur = pCur->m_pNextMaterial;
	}

	fclose(fp);
	return TRUE;
}
void MtlExp::ReleaseMaterials()
{
	CMaterial*	pCur = m_pBaseMaterial;
	CMaterial*	pNext;

	while (pCur)
	{
		pNext = pCur->m_pNextMaterial;
		delete pCur;
		pCur = pNext;
	}
	m_pBaseMaterial = NULL;
	m_dwMaterialNum = 0;

}