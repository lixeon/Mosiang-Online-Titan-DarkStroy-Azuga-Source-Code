//
//	Tool Information Class
//
//			2002.12.12
//			Trust Pak


#include "StdAfx.h"
#include <stdio.h>
#include <assert.h>
#include "EngineRelation.h"
#include "ToolInfo.h"
#include <process.h>
#include <Mmsystem.h>
#include "DlgEffectPalette.h"
#include "DialogBar.h"

#pragma comment(lib, "winmm.lib")




/// Global variables /////////////////////////////////////////////////////////

CModelViewToolInfo g_ToolInfo;


//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::CreateChFile
// Description :  
//----------------------------------------------------------------------------
BOOL GetFileExtension(char* pResult, const char* pSource, INT iResultSize)
{
	if (NULL == pResult || NULL == pSource)
	{
		return false;
	}

	memset(pResult, 0, iResultSize);
	
	CString strFileName = pSource;
	strFileName.MakeReverse();

	int i;
	int iCount = 0;

	for (i = 0; i < strFileName.GetLength(); i++)
	{
		if ('.' == strFileName[i])			
		{
			break;
		}

		pResult[iCount] = strFileName[i];
		
		++iCount;
	}

	strFileName = pResult;
	strFileName.MakeReverse();

	memcpy(pResult, (LPSTR)(LPCTSTR)strFileName, strFileName.GetLength());

	return TRUE;
}


//----------------------------------------------------------------------------
// Name : QueryChFileType
// Description : 
//----------------------------------------------------------------------------
ENUM_CH_FILE_TYPE QueryChFileType(const char* szFileName)
{
	// Get extension name. ( last 3 character )

	CString strFileName;
	CString strExtension;

	strFileName = szFileName;
	strFileName.MakeReverse();

	int i = 0;
	int iCount = 0;
	char szExtension[512];
	memset(szExtension, 0, 512);

	for (i = 0; i < strFileName.GetLength(); i++)
	{
		if ('.' == strFileName[i])
		{
			break;
		}		

		szExtension[iCount++] = strFileName[i];
	}


	// Check file type.

	strExtension = szExtension;
	
	strExtension.MakeReverse();



	ENUM_CH_FILE_TYPE result;

	if ("CHR" == strExtension)
	{
		result = CHR_FILE;
	}
	else if ("CHX" == strExtension)
	{
		result = CHX_FILE;
	}
	else
	{
		result = NOT_CH_FILE;
	}

	return result;
	
}

//----------------------------------------------------------------------------
// Name : QueryDirectoryString
// Description : 
//----------------------------------------------------------------------------
BOOL QueryDirectoryString(char* pResult, char* pSource)
{
	// Checking argument.

	if (NULL == pResult)
	{
		return FALSE;
	}

	if (NULL == pSource)
	{
		return FALSE;
	}

	
	CString strFileName;

	char szDirectory[2048];
	BOOL bDirectory = FALSE;
	int iCount = 0;

	strFileName = pSource;	
	strFileName.MakeReverse();

	memset(szDirectory, 0, 2048);

	int i;
	for (i = 0; i < strFileName.GetLength(); i++)
	{
		if ('\\' == strFileName[i] && FALSE == bDirectory)
		{
			bDirectory = TRUE;			
		}

		if (TRUE == bDirectory)
		{
			szDirectory[iCount++] = strFileName[i];
		}
	}

	CString strDirectory = szDirectory;
	strDirectory.GetLength();

	strDirectory.MakeReverse();	
	memcpy(pResult, (LPSTR)(LPCTSTR)strDirectory, strDirectory.GetLength());

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : GetPureFileName
// Description :
//----------------------------------------------------------------------------
BOOL GetPureFileName(char* pResult, const char* pSource, INT iResultSize)
{

	if (NULL == pSource)
	{
		return FALSE;
	}

	if (0 > iResultSize)
	{
		return FALSE;
	}

	memset(pResult, 0, iResultSize);


	CString strSource;
	CString strResult;

	strSource = pSource;
	strSource.MakeReverse();

	int i = 0;

	for (i = 0; i < strSource.GetLength(); i++)
	{
		if ('\\' == strSource[i])
		{
			break;
		}

		pResult[i] = strSource[i];
	}

	
	strResult = pResult;
	strResult.MakeReverse();

	memcpy(pResult, (LPSTR)(LPCTSTR)strResult, strResult.GetLength());	

	return TRUE;
}






//////////////////////////////////////////////////////////////////////////////
// Class Name : CToolInfo
// Description :
//
//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::CModelViewToolInfo
// Description : Contructor.
//----------------------------------------------------------------------------
CModelViewToolInfo::CModelViewToolInfo() : 
	m_pFont(NULL)
{
	g_pDlgEffectPalette = NULL;
	m_bViewEffectPalette = FALSE;	
	RemoveFrontOfCameraObject();
	ClearInformation();	
	GetCurrentDirectory(1024, m_szProgramPath);		
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::~CModelViewToolInfo
// Description :
//----------------------------------------------------------------------------
CModelViewToolInfo::~CModelViewToolInfo()
{	
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::~CModelViewToolInfo
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::ClearInformation(VOID)
{
	// Restore used resource.

	// 잔상 오브젝트 지운다.
	DeleteIllusionMesh();	
	ClearGXObjects();
	
	// Set requirement information.

	m_bWireMode = FALSE; 
	m_iScrollMin = 0;
	m_iScrollMax = 0;
	m_iScrollPos = 0;
	m_iCurrentFrame = 0;
	
	m_iTotalModNo = 0;
	m_iTotalAnmNo = 0;
	m_iTotalMtlNo = 0;

	m_iCurrentModNo = -1;
	m_iCurrentAnmNo = -1;
	m_iCurrentMtlNo = -1;	
	m_iCurrentWavNo = -1;

	m_dwFps = 33;

	/*
	m_abyAmbient[0] = 100; 
	m_abyAmbient[1] = 100;
	m_abyAmbient[2] = 100;

	m_abyDiffuse[0] = 255;
	m_abyDiffuse[1] = 255;
	m_abyDiffuse[2] = 255;
	*/

	m_abyAmbient[0] = 180; 
	m_abyAmbient[1] = 180;
	m_abyAmbient[2] = 180;

	m_abyDiffuse[0] = 200;
	m_abyDiffuse[1] = 200;
	m_abyDiffuse[2] = 200;	

	m_abySpecular[0] = 0;
	m_abySpecular[1] = 0;
	m_abySpecular[2] = 0;
	
	m_afDirection[0] =  1.0f;
	m_afDirection[1] = -1.0f;
	m_afDirection[2] =  1.0f;

	m_bSpecularEnable		= FALSE;
	m_fSpecularIntensity	= 0.0f;

	m_bOnLight		= TRUE;
	m_bPlaySound	= FALSE;
	m_bNowPlayed	= FALSE;
	m_bIsChFile		= FALSE;
	m_gxLastSoundHandle = NULL;
	m_bDisplayBoundingBox = FALSE;

	m_lstModFileName.RemoveAll();
	m_lstAnmFileName.RemoveAll();
	m_lstMtlFileName.RemoveAll();
	m_lstGXObject.RemoveAll();	

	m_iCurrentGxObject = -1;
	m_iTotalGxObject = 0;
	m_gxhCurrentRendered = NULL;
	m_bIsAllSelect = FALSE;	

	m_Camera.InitCamera(g_pExecutive, 0);
	m_bRenderCameraInfo = FALSE;

	// 위치를 옮겨야 하나.?
	memset( &m_Illusion, 0, sizeof(m_Illusion));

	// 배경색
	m_dwBackGroundColorR	=	DEFAULT_BACKGROUND_COLOR_R;
	m_dwBackGroundColorG	=	DEFAULT_BACKGROUND_COLOR_G;
	m_dwBackGroundColorB	=	DEFAULT_BACKGROUND_COLOR_B;	
	
	
	return TRUE;
}
void CModelViewToolInfo::UpdateEffectIndex()
{
	DWORD	dwEffectIndex;
	dwEffectIndex = g_pDlgEffectPalette->GetCurrentEffectIndex();
	if (m_bViewEffectPalette)
	{

		int i = 0;
		for (i = 0; i < m_lstGXObject.GetCount(); i++)
		{
			POSITION pos = m_lstGXObject.FindIndex(i);
			GXOBJECT_HANDLE gxh = m_lstGXObject.GetAt(pos);
			g_pExecutive->GXOSetEffectIndex(gxh,dwEffectIndex);
		}
	}
}


BOOL CModelViewToolInfo::ViewEffectPalette()
{
	if (!m_bViewEffectPalette)
	{
		m_bViewEffectPalette = TRUE;
		g_pDlgEffectPalette->EnableView();
	}
	else
	{
		m_bViewEffectPalette = FALSE;
		g_pDlgEffectPalette->DisableView();
	}
	return m_bViewEffectPalette;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::RemoveModFile
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::RemoveModFile(int iCount)
{
	// Stop Animation.

	StopAnimation();

	if (iCount >= m_lstModFileName.GetCount())
	{
		return FALSE;
	}

	if (0 > iCount)
	{
		return FALSE;
	}

	
	POSITION GxPos = m_lstGXObject.FindIndex(iCount);
	POSITION ModFilePos = m_lstModFileName.FindIndex(iCount);

	GXOBJECT_HANDLE gxh = m_lstGXObject.GetAt(GxPos);

	g_pExecutive->DeleteGXObject(gxh);
	m_lstModFileName.RemoveAt(ModFilePos);
	m_lstGXObject.RemoveAt(GxPos);

	--m_iTotalGxObject;
	--m_iTotalModNo;
	--m_iCurrentModNo;	

	if (0 > m_iCurrentModNo && 0 < m_iTotalModNo)
	{
		m_iCurrentModNo = 0;
	}

	SetCurrentModFile(m_iCurrentModNo);

	return TRUE;
}


//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::RemoveAnmFile
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::RemoveAnmFile(int iCount)
{	
	// Stop Animation.

	StopAnimation();

	if (iCount >= m_lstAnmFileName.GetCount())
	{
		return FALSE;
	}

	if (0 > iCount)
	{
		return FALSE;
	}


	POSITION pos = m_lstAnmFileName.FindIndex(iCount);

	m_lstAnmFileName.RemoveAt(pos);


	int i = 0;
	for (i = 0; i < m_lstGXObject.GetCount(); i++)
	{
		I3DModel* pModel = NULL;
		DWORD dwRefIndex = 0;

		pos = m_lstGXObject.FindIndex(i);

		GXOBJECT_HANDLE gxh = m_lstGXObject.GetAt(pos);

		g_pExecutive->GXOGetModel(gxh,		
							      &pModel,
							      &dwRefIndex,
							      0);

		pModel->DeleteAllMotionPerReference(dwRefIndex);		

		pModel->Release();
		pModel = NULL;
	}

	
	int j = 0;

	for (i = 0; i < m_lstGXObject.GetCount(); i++)
	{
		I3DModel* pModel = NULL;
		DWORD dwRefIndex = 0;

		pos = m_lstGXObject.FindIndex(i);

		GXOBJECT_HANDLE gxh = m_lstGXObject.GetAt(pos);
	
		g_pExecutive->GXOGetModel(gxh,
							  &pModel,
							  &dwRefIndex,
							  0);

		pModel->CreateMotionList(MAX_MOTION_NUM, dwRefIndex);
		


		for (j = 0; j < m_lstAnmFileName.GetCount(); j++)
		{
			POSITION posAnm = m_lstAnmFileName.FindIndex(j);

			CString strAnmFileName = m_lstAnmFileName.GetAt(posAnm);

			pModel->AddMotion((LPSTR)(LPCTSTR)strAnmFileName,
							  dwRefIndex,
							  0);	
		}

		pModel->Release();
		pModel = NULL;
	}

	--m_iTotalAnmNo;
	--m_iCurrentAnmNo;

	if (0 > m_iCurrentAnmNo && 0 < m_iTotalAnmNo)
	{
		m_iCurrentAnmNo = 0;
	}

	SetCurrentAnmFile(m_iCurrentAnmNo);
	SetCurrentModFile(m_iCurrentModNo);

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::AddModFile
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::AddModFile(char* szFileName)
{

	// Stop animation

	StopAnimation();

	// Create GX Object.

	GXOBJECT_HANDLE hGxObject = g_pExecutive->CreateGXObject(szFileName,
															 ModelProcMod,
															 NULL,
															 GXOBJECT_CREATE_TYPE_NOT_OPTIMIZE);

#ifdef _DEBUG

	if (NULL == hGxObject)
	{		
		assert(!"Failed Create g_hGxObject");
		return FALSE;
	}	

#endif	

	m_lstGXObject.AddTail(hGxObject);

	// Create motion list for model file.

	I3DModel* pModel = NULL;
	DWORD dwRefIndex = 0;

	g_pExecutive->GXOGetModel(hGxObject,
							  &pModel,
							  &dwRefIndex,
							  0);
		
	pModel->CreateMotionList(MAX_MOTION_NUM, dwRefIndex);
	pModel->CreateMaterialList(MAX_MATERIAL_NUM, dwRefIndex);

	// And add file name to the Model file list.

	CString strAddFileName = szFileName;

	m_lstModFileName.AddTail(strAddFileName);

	// Update user interface

	++m_iTotalModNo;
	++m_iCurrentModNo;
	++m_iTotalGxObject;

	SetCurrentModFile(m_iCurrentModNo);
	SetCurrentRenderedGxObject(hGxObject);

	// Bind exsiting anm file to the New Model file.

	int i = 0;

	for (i = 0; i < m_lstAnmFileName.GetCount(); i++)
	{
		POSITION pos = m_lstAnmFileName.FindIndex(i);
		CString strAnmFileName = m_lstAnmFileName.GetAt(pos);

		DWORD dwReturn = pModel->AddMotion((LPSTR)(LPCTSTR)strAnmFileName,
									       dwRefIndex,
										   0);
	}

	// Bind existing mtl file to the new model file.

	for (i = 0; i < m_lstMtlFileName.GetCount(); i++)
	{
		POSITION pos = m_lstMtlFileName.FindIndex(i);
		CString strMtlFileName = m_lstMtlFileName.GetAt(pos);

		DWORD dwReturn = pModel->AddMaterial((LPSTR)(LPCTSTR)strMtlFileName,
											 dwRefIndex,
											 0);
	}	

	// Restore used resource.

	pModel->Release();

	
	SetCurrentMtlFile(g_ToolInfo.m_iCurrentMtlNo);
	SetCurrentAnmFile(g_ToolInfo.m_iCurrentAnmNo);	
	SetCurrentModFile(m_iCurrentModNo);

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::AddAnmFile
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::AddAnmFile(char* szFileName)
{
	// Stop Animation

	StopAnimation();

	// Bind Animation information.

	int i = 0;

	for (i = 0; i < m_lstGXObject.GetCount(); i++)
	{
		I3DModel* pModel = NULL;
		DWORD dwRefIndex = 0;

		POSITION pos = m_lstGXObject.FindIndex(i);
		GXOBJECT_HANDLE gxh = m_lstGXObject.GetAt(pos);


		g_pExecutive->GXOGetModel(gxh,
								  &pModel,
								  &dwRefIndex,
								  0);
		
		pModel->AddMotion(szFileName, dwRefIndex, 0);

		// Restore used resource.

		pModel->Release();
	}	

	// Update user interface

	++m_iCurrentAnmNo;
	++m_iTotalAnmNo;
	

	// To add Animation file nate to the animation file list.

	CString strFileName = szFileName;
	m_lstAnmFileName.AddTail(strFileName);

	SetCurrentMtlFile(m_iCurrentMtlNo);
	SetCurrentAnmFile(m_iCurrentAnmNo);

	// Above code includes Updating user interface.

	SetCurrentModFile(m_iCurrentModNo);

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::AddChrFile
// Description :
//   Add character file.
//   Character file contains one of model file and lots of animation files.
//   clear model file list and animatino list.
//   Next, contain Chr file's information to the lists.
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::AddChrFile(char* szFileName)
{	
	// Clear legacy resource.	 (It includes to stop animation )

	// Sound objects are independent from Model, animation and material files...
	// So back-up its data before clear informations of ToolInfo Object.

	int iLastWavNo = -1;

	if (0 < m_iTotalWavNo)
	{
		iLastWavNo = m_iCurrentWavNo;
	}	

	ClearInformation();

	SetCurrentWavFile(iLastWavNo);


	// Create GXObejct-object.

	GXOBJECT_HANDLE hGxObject = g_pExecutive->CreateGXObject(szFileName,
															 ModelProcCh,
															 NULL,
															 GXOBJECT_CREATE_TYPE_NOT_OPTIMIZE);
	


#ifdef _DEBUG

	if (NULL == hGxObject)
	{	
		return FALSE;
	}

#endif	

	// Insert handle of GxObject to the handle list.

	m_lstGXObject.AddTail(hGxObject);


	char szDirectory[1024];
	memset(szDirectory, 0, 1024);

	QueryDirectoryString(szDirectory, szFileName);	

	CreateFileNameListFromChrFile(hGxObject,
							     szFileName,
							     szDirectory);

	// Set Current rendered object.

	SetCurrentRenderedGxObject(hGxObject);

	// And add file name to the chr file list.

	m_strChrFileName = szFileName;

	// Set Ch file flag

	m_bIsChFile = TRUE;

	++m_iTotalGxObject;
	m_iCurrentGxObject = m_iTotalGxObject -1;	


	// Set last informations.

	m_iCurrentModNo = 0;	
	m_iCurrentAnmNo = 0;
	m_iCurrentMtlNo = 0;


	SetCurrentMtlFile(m_iCurrentMtlNo);
	SetCurrentAnmFile(m_iCurrentAnmNo);
	SetCurrentModFile(m_iCurrentModNo);

	return TRUE;
}


//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::AddChxFile
// Description :
//   Add Chx file.
//   Chx File conatins lots of model files and lots of animation files.
//   First, Clear model list and animation list.
//   Next, To contain chx file's information to the lists.
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::AddChxFile(char* szFileName)
{	
	// Clear legacy resource.	 (It includes to stop animation )

	// Sound objects are indepent from Model, animation and material files...
	// So back-up its data before clear informations of ToolInfo Object.

	int iLastWavNo = -1;

	if (0 < m_iTotalWavNo)
	{
		iLastWavNo = m_iCurrentWavNo;
	}	

	ClearInformation();

	SetCurrentWavFile(iLastWavNo);


	// Create GXObejct-object.

	GXOBJECT_HANDLE hGxObject = g_pExecutive->CreateGXObject(szFileName,
															 ModelProcCh,
															 NULL,
															 GXOBJECT_CREATE_TYPE_NOT_OPTIMIZE);

#ifdef _DEBUG

	if (NULL == hGxObject)
	{	
		assert(!"Failed Create g_hGxObject");
		return FALSE;
	}

#endif		

	// Insert handle of GxObject to the handle list.

	m_lstGXObject.AddTail(hGxObject);

	char szDirectory[1024];
	memset(szDirectory, 0, 1024);

	QueryDirectoryString(szDirectory, szFileName);

	CreateFileNameListFromChxFile(hGxObject,
							     szFileName,
							     szDirectory);

	// And add file name to the chr file list.	

	m_strChrFileName = szFileName;

	// Set Ch File Flag.

	m_bIsChFile = TRUE;

	// And Set Current GxObject

	SetCurrentRenderedGxObject(hGxObject);

	++m_iTotalGxObject;
	m_iCurrentGxObject = m_iTotalGxObject -1;	


	// Set last informations.

	m_iCurrentModNo = 0;	
	m_iCurrentAnmNo = 0;
	m_iCurrentMtlNo = 0;

	SetCurrentMtlFile(m_iCurrentMtlNo);
	SetCurrentAnmFile(m_iCurrentAnmNo);
	SetCurrentModFile(m_iCurrentModNo);

	return TRUE;	
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::CreateHandleListFromChrFile
// Description :
//		Above function is called by AddChrFile method.
//		Make model handle of Chr file and 
//		bind animation file with model file.
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::CreateFileNameListFromChrFile(GXOBJECT_HANDLE hGxObject, char* szChrFileName, char* szDirectory)
{	
	BOOL bResult = TRUE;

	FILE* fp = fopen(szChrFileName, "rb");

	if (NULL == fp)
	{
		bResult = FALSE;
	}
	else
	{
		BOOL bFileEnd = FALSE;		
		char szOneLine[1024];
		char szLeftToken[256];
		char szRightToken[256];
		CString strCompleteFileName;

		while (FALSE == bFileEnd)
		{
			memset(szOneLine, 0, 1024);
			memset(szLeftToken, 0, 256);
			memset(szRightToken, 0, 256);
			
			fgets(szOneLine, 1024, fp);

			if (0 != feof(fp))
			{
				bFileEnd = TRUE;
				continue;
			}

			sscanf(szOneLine, "%s %s", szLeftToken, szRightToken);

			// Mod File Load

			if (0 == strcmp("*MOD_FILE_NAME", szLeftToken))
			{			
				// Insert file name to model file name list.

				strCompleteFileName = szDirectory;				
				strCompleteFileName += szRightToken;				
				
				m_lstModFileName.AddTail(strCompleteFileName);

				BOOL bReadAnimation = FALSE;
				BOOL bReadMaterial = FALSE;
				
				while (FALSE == bReadAnimation || FALSE == bReadMaterial)
				{
					fgets(szOneLine, 1024, fp);

					if (0 != feof(fp))
					{
						bFileEnd = TRUE;
						bResult = FALSE;
						
						break;
					}

					sscanf(szOneLine, "%s", szLeftToken);

					if (0 == strcmp("*MOTION_NUM", szLeftToken) && FALSE == bReadAnimation)
					{
						/// Anm File Load. 

						int iAnimationCount;
						sscanf(szOneLine, "%s %d", szLeftToken, &iAnimationCount);

						int i = 0;

						for (i = 0; i < iAnimationCount && FALSE == bFileEnd; i++)
						{
							fgets(szOneLine, 1024, fp);


							if (0 != feof(fp))
							{
								bFileEnd = TRUE;
								bResult = FALSE;
								continue;
							}


							sscanf(szOneLine, "%s", szLeftToken);
							strCompleteFileName = szDirectory ;
							strCompleteFileName += szLeftToken;
							
							// Insert Animation file name to the Animation file list.
						

							m_lstAnmFileName.AddTail(strCompleteFileName);
						}

						bReadAnimation = TRUE;
					}


					// Read Material file information.

					if (0 == strcmp("*MATERIAL_NUM", szLeftToken) && FALSE == bReadMaterial)
					{
						/// Mtl File Load.

						int iMaterialCount;
						sscanf(szOneLine, "%s %d", szLeftToken, &iMaterialCount);


						int i = 0;
						
						for (i = 0; i < iMaterialCount && FALSE == bFileEnd; i++)
						{
							fgets(szOneLine, 1024, fp);

							if (0 != feof(fp))
							{
								bFileEnd = TRUE;
								bResult = FALSE;
								continue;
							}

							sscanf(szOneLine, "%s", szLeftToken);
							strCompleteFileName = szDirectory ;
							strCompleteFileName += szLeftToken;
							
							// Insert Material file name to the Animation file list.
							
							m_lstMtlFileName.AddTail(strCompleteFileName);
						}

						bReadMaterial = TRUE;						

					}

				}
			}			
		}

		bResult = TRUE;
	}


	// Restore used memory.

	fclose(fp);

	return bResult;
}



//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::CreateHandleListFromChxFile
// Description :
//		Above function is called by AddChrFile method.
//		Make model handle of Chr file and 
//		bind animation file with model file.
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::CreateFileNameListFromChxFile(GXOBJECT_HANDLE hGxObject, char* szChrFileName, char* szDirectory)
{	
	BOOL bResult = TRUE;
	BOOL bAnmFileComplete = FALSE;
	BOOL bMtlFileComplete = FALSE;

	FILE* fp = fopen(szChrFileName, "rb");

	if (NULL == fp)
	{
		bResult = FALSE;
	}
	else
	{
		BOOL bFileEnd = FALSE;
		char szOneLine[1024];
		char szLeftToken[256];
		char szRightToken[256];
		CString strCompleteFileName;

		while (FALSE == bFileEnd)
		{
			memset(szOneLine, 0, 1024);
			memset(szLeftToken, 0, 256);
			memset(szRightToken, 0, 256);
			
			fgets(szOneLine, 1024, fp);

			if (0 != feof(fp))
			{
				bFileEnd = TRUE;
				continue;
			}

			sscanf(szOneLine, "%s %s", szLeftToken, szRightToken);		

			if (0 == strcmp("*MOD_FILE_NAME", szLeftToken))
			{			
				// Insert file name to model file name list.

				strCompleteFileName = szDirectory;				
				strCompleteFileName += szRightToken;				
				
				m_lstModFileName.AddTail(strCompleteFileName);


				// Read animation file information.
				
				fgets(szOneLine, 1024, fp);

#ifdef _DEBUG

				if (0 != feof(fp))
				{
					bFileEnd = TRUE;
					bResult = FALSE;
					continue;
				}
			
#endif			
			


				if (FALSE == bAnmFileComplete)
				{

					int iAnimationCount;
					sscanf(szOneLine, "%s %d", szLeftToken, &iAnimationCount);

					int i = 0;

					for (i = 0; i < iAnimationCount && FALSE == bFileEnd; i++)
					{
						fgets(szOneLine, 1024, fp);

#ifdef _DEBUG
						if (0 != feof(fp))
						{
							bFileEnd = TRUE;
							bResult = FALSE;
							continue;
						}
#endif

						sscanf(szOneLine, "%s", szLeftToken);
						strCompleteFileName = szDirectory ;
						strCompleteFileName += szLeftToken;
						
						// Insert Animation file name to the Animation file list.

						m_lstAnmFileName.AddTail(strCompleteFileName);

					}

					bAnmFileComplete = TRUE;
				}


				// Read animation file information.
				
				fgets(szOneLine, 1024, fp);

#ifdef _DEBUG

				if (0 != feof(fp))
				{
					bFileEnd = TRUE;
					bResult = FALSE;
					continue;
				}
			
#endif			


				// Material load.

				if (FALSE == bMtlFileComplete)
				{
					int iMaterialCount;
					sscanf(szOneLine, "%s %d", szLeftToken, &iMaterialCount);

					int i = 0;

					for (i = 0; i < iMaterialCount; i++)
					{
						fgets(szOneLine, 1024, fp);

						if (0 != feof(fp))
						{
							bFileEnd = TRUE;
							bResult = FALSE;
							continue;
						}


						sscanf(szOneLine, "%s", szLeftToken);
						strCompleteFileName = szDirectory ;
						strCompleteFileName += szLeftToken;
						
						// Insert Material file name to the Animation file list.

						m_lstMtlFileName.AddTail(strCompleteFileName);

					}

					bMtlFileComplete = TRUE;
				}
			}			
		}

		bResult = TRUE;
	}


	// Restore used memory.

	fclose(fp);

	return bResult;
}


//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::SetCurrentModFile
// Description : if argument iCount is Zero that describes all GxObject is rendered.
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::SetCurrentModFile(int iCount)
{	
	if (0 > iCount)
	{
		return FALSE;
	}
	
	if (0 == m_lstModFileName.GetCount())
	{
		m_gxhCurrentRendered = NULL;
		return FALSE;
	}

	if (FALSE == m_bIsChFile)
	{
		POSITION pos = m_lstGXObject.FindIndex(iCount);
		GXOBJECT_HANDLE gxh = m_lstGXObject.GetAt(pos);
		
		m_iCurrentModNo = iCount;

		if (TRUE != m_bIsAllSelect)
		{
			SetCurrentRenderedGxObject(gxh);
		}
		else
		{
			SetCurrentRenderedGxObject(NULL);
		}

		// Update user interface

		DWORD dwMotion = g_pExecutive->GXOGetMotionNum(gxh, 0);

		m_gxhCurrentRendered = gxh;

		if (0 < dwMotion)
		{
			MOTION_DESC motion;
			
			g_pExecutive->GXOGetMotionDesc(gxh,
										   &motion,
										   m_iCurrentAnmNo +1,
										   0);

			

			m_iScrollMin = motion.dwFirstFrame;
			m_iScrollMax = motion.dwLastFrame;
			m_iScrollPos = motion.dwFirstFrame;

			UpdateAnimationInfo(iCount);
		}
		else
		{
			m_iScrollMin = 0;
			m_iScrollMax = 0;
			m_iScrollPos = 0;

			UpdateAnimationInfo(iCount);
		}
	}
	else		// It is Chr or Chx file
	{		
		GXOBJECT_HANDLE gxh = m_lstGXObject.GetHead();
		m_iCurrentModNo = iCount;
		SetCurrentRenderedGxObject(NULL);

		// Update user interface
	
		DWORD dwMotion = g_pExecutive->GXOGetMotionNum(gxh, 0);
		m_gxhCurrentRendered = gxh;

		if (0 < dwMotion)
		{
			MOTION_DESC motion;

			g_pExecutive->GXOGetMotionDesc(gxh,
										   &motion,
										   m_iCurrentAnmNo +1,
										   0);

			m_iScrollMin = motion.dwFirstFrame;
			m_iScrollMax = motion.dwLastFrame;
			m_iScrollPos = motion.dwFirstFrame;

			UpdateAnimationInfo(iCount);
		}
	}	

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::SetCurrentAnmFile
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::SetCurrentAnmFile(int iCount)
{
	if (0 > iCount)
	{
		return FALSE;
	}

	if (0 == m_lstAnmFileName.GetCount())
	{
		return FALSE;
	}

	m_iCurrentAnmNo = iCount;

	// Setting current animation informatioin

	SetCurrentRenderedAnimation(iCount);
	
	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::StopAnimation
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::StopAnimation(VOID)
{
	m_bNowPlayed = FALSE;	

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::UpdateAnimationInfo
// Description : Update information of Engine Object from CModelViewToolInfo
//               Class' value.
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::UpdateAnimationInfo(INT iIndex)
{	
	if (iIndex >= m_lstGXObject.GetCount())
	{
		return FALSE;
	}	

	POSITION pos = m_lstGXObject.FindIndex(iIndex);
	GXOBJECT_HANDLE gxh = m_lstGXObject.GetAt(pos);

	// Setting Current frame of all GxObject is Zero.
	
	g_pExecutive->GXOSetCurrentFrame(gxh, 0);	

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::CreateChFile
// Description :  ... Above function have to retouch. becuase I have no time,
//				  So It is made non-speedy.                  
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::CreateChFile(char* szFileName, ENUM_CH_FILE_TYPE chType)
{
	BOOL bReturn = FALSE;	

	if (CHR_FILE == chType)
	{
		FILE* fp = fopen(szFileName, "wb");
		if (NULL == fp)
		{
			bReturn = FALSE;
			goto lb_return;				// ㅡ_-;; How annoying!. (To use exception l).
		}

		char szSaveFileName[512];
		
		CString strModFileName = m_lstModFileName.GetHead();	
		GetPureFileName(szSaveFileName, (LPSTR)(LPCTSTR)strModFileName, 512);


		fprintf(fp, "*MOD_FILE_NAME\t%s\n", szSaveFileName, 512);		
		fprintf(fp, "\t*MOTION_NUM\t%d\n", m_lstAnmFileName.GetCount());
		
		int i = 0;

		for (i = 0; i < m_lstAnmFileName.GetCount(); i++)
		{
			memset(szSaveFileName, 0, 512);

			POSITION posAnm = m_lstAnmFileName.FindIndex(i);
			CString strAnmFileName = m_lstAnmFileName.GetAt(posAnm);

			GetPureFileName(szSaveFileName, (LPSTR)(LPCTSTR)strAnmFileName, 512);

			fprintf(fp, "\t\t%s\n", szSaveFileName);
		}

		fprintf(fp, "\t*MATERIAL_NUM\t%d\n", m_lstMtlFileName.GetCount());

		for (i = 0; i < m_lstMtlFileName.GetCount(); i++)
		{
			memset(szSaveFileName, 0, 512);

			POSITION posMtl = m_lstMtlFileName.FindIndex(i);
			CString strMtlFileName = m_lstMtlFileName.GetAt(posMtl);

			GetPureFileName(szSaveFileName, (LPSTR)(LPCTSTR)strMtlFileName, 512);

			fprintf(fp, "\t\t%s\n", szSaveFileName);
		}

		fclose(fp);
	}
	else if (CHX_FILE == chType)
	{
		FILE* fp = fopen(szFileName, "wb");
		if (NULL == fp)
		{
			bReturn = FALSE;
			goto lb_return;				// ㅡ_-;;;
		}

		fprintf(fp, "*MOD_FILE_NUM\t%d\n\n", m_lstModFileName.GetCount());

		char szSaveFileName[512];
		
		
		int i = 0;

		for (i = 0; i < m_lstModFileName.GetCount(); i++)
		{
			memset(szSaveFileName, 0, 512);

			POSITION posMod = m_lstModFileName.FindIndex(i);
			CString strModFileName = m_lstModFileName.GetAt(posMod);
			
			GetPureFileName(szSaveFileName, (LPSTR)(LPCTSTR)strModFileName, 512);

			fprintf(fp, "*MOD_FILE_NAME\t%s\n", szSaveFileName);
			fprintf(fp, "\t*MOTION_NUM\t%d\n", m_lstAnmFileName.GetCount());

			int j = 0;
			for (j = 0; j < m_lstAnmFileName.GetCount(); j++)
			{
				memset(szSaveFileName, 0, 512);

				POSITION posAnm = m_lstAnmFileName.FindIndex(j);
				CString strAnmFileName = m_lstAnmFileName.GetAt(posAnm);

				GetPureFileName(szSaveFileName, (LPSTR)(LPCTSTR)strAnmFileName, 512);

				fprintf(fp, "\t\t%s\n", szSaveFileName);
			}

			fprintf(fp, "\t*MATERIAL_NUM\t%d\n", m_lstMtlFileName.GetCount());

			for (j = 0; j < m_lstMtlFileName.GetCount(); j++)
			{
				memset(szSaveFileName, 0, 512);

				POSITION posMtl = m_lstMtlFileName.FindIndex(j);
				CString strMtlFileName = m_lstMtlFileName.GetAt(posMtl);

				GetPureFileName(szSaveFileName, (LPSTR)(LPCTSTR)strMtlFileName, 512);

				fprintf(fp, "\t\t%s\n", szSaveFileName);
			}


			fprintf(fp, "\n");

		}

		fclose(fp);
	}
	else
	{
		bReturn = FALSE;
	}


lb_return:	
	return bReturn;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::AddMtlFile
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::AddMtlFile(char* szFileName)
{
	// Stop Animation

	StopAnimation();

	int i = 0;

	for (i = 0; i < m_lstGXObject.GetCount(); i++)
	{
		I3DModel* pModel = NULL;
		DWORD dwRefIndex = 0;

		POSITION pos = m_lstGXObject.FindIndex(i);
		GXOBJECT_HANDLE gxh = m_lstGXObject.GetAt(pos);

		g_pExecutive->GXOGetModel(gxh,
								  &pModel,
								  &dwRefIndex,
								  0);

		pModel->AddMaterial(szFileName, dwRefIndex, 0);

		// Restore used resource.

		pModel->Release();
	}
	
	// Update user interface

	++m_iCurrentMtlNo;
	++m_iTotalMtlNo;	

	// Adding material name to the materail file list.

	CString strFileName = szFileName;
	m_lstMtlFileName.AddTail(strFileName);



	SetCurrentMtlFile(m_iCurrentMtlNo);
	SetCurrentAnmFile(m_iCurrentAnmNo);

	// Above code includes updating user interface.

	SetCurrentModFile(m_iCurrentModNo);
	

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::AddMtlFile
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::RemoveMtlFile(int iCount)
{
	// Stop Animation.

	StopAnimation();

	if (iCount >= m_lstMtlFileName.GetCount())
	{
		return FALSE;
	}

	if (0 > iCount)
	{
		return FALSE;
	}

	POSITION pos = m_lstMtlFileName.FindIndex(iCount);
	m_lstMtlFileName.RemoveAt(pos);

	int i = 0;
	for (i = 0; i < m_lstGXObject.GetCount(); i++)
	{
		I3DModel* pModel = NULL;
		DWORD dwRefIndex = 0;

		pos = m_lstGXObject.FindIndex(i);

		GXOBJECT_HANDLE gxh = m_lstGXObject.GetAt(pos);

		g_pExecutive->GXOGetModel(gxh,		
							      &pModel,
							      &dwRefIndex,
							      0);
		
		pModel->DeleteAllMaterialPerReference(dwRefIndex);

		pModel->Release();
		pModel = NULL;
	}

	int j = 0;
	for (i = 0; i < m_lstGXObject.GetCount(); i++)
	{
		I3DModel* pModel = NULL;

		DWORD dwRefIndex = 0;

		pos = m_lstGXObject.FindIndex(i);

		GXOBJECT_HANDLE gxh = m_lstGXObject.GetAt(pos);
	
		g_pExecutive->GXOGetModel(gxh,
							  &pModel,
							  &dwRefIndex,
							  0);
		
		pModel->CreateMaterialList(MAX_MATERIAL_NUM, dwRefIndex);
		

		for (j = 0; j < m_lstMtlFileName.GetCount(); j++)
		{
			POSITION posMtl = m_lstMtlFileName.FindIndex(j);

			CString strMtlFileName = m_lstMtlFileName.GetAt(posMtl);

			pModel->AddMaterial((LPSTR)(LPCTSTR)strMtlFileName,
							  dwRefIndex,
							  0);	
		}

		pModel->Release();
		pModel = NULL;
	}

	--m_iTotalMtlNo;
	--m_iCurrentMtlNo;

	if (0 > m_iCurrentMtlNo && 0 < m_iTotalMtlNo)
	{
		m_iCurrentMtlNo = 0;
	}

	SetCurrentMtlFile(m_iCurrentMtlNo);
	SetCurrentModFile(m_iCurrentModNo);
	

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::AddMtlFile
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::SetCurrentMtlFile(int iCount)
{
	if (0 > iCount)
	{
		return FALSE;
	}

	if (0 == m_lstMtlFileName.GetCount())
	{
		return FALSE;
	}

	m_iCurrentMtlNo = iCount;

	// Setting current animation informatioin

	SetCurrentRenderedMaterial(iCount);
	
	return TRUE;	
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::LoadGridInformation
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::LoadGridInformation(VOID)
{
	// clear legacy information.

	ClearGridInformation();

	// Open Grid Informatio file.
	
	char szGridInfoFileName[1024];
	memset(szGridInfoFileName, 0, 1024);

	strcpy(szGridInfoFileName, "data/");	
	strcat(szGridInfoFileName, c_szGridInfoFileName);

	FILE* fp = fopen(szGridInfoFileName, "rb");

	if (NULL == fp)
	{
		return FALSE;
	}


	// Read information.

	char szOneLine[1024];
	memset(szOneLine, 0, 1024);

	fgets(szOneLine, 1024, fp);

	// Parse information.

	char szLeftToken[512];
	char szRightToken[512];

	memset(szLeftToken, 0, 512);
	memset(szRightToken, 0, 512);

	sscanf(szOneLine, "%s %s", szLeftToken, szRightToken);

	// Is information Judgement lawfulness?

	if (0 == strcmp(c_szGridFileIndicator, szLeftToken))
	{	
		strcpy(m_szGridName, m_szProgramPath);
		strcat(m_szGridName, "\\data\\");

		SetCurrentDirectory(m_szGridName);
		
		strcat(m_szGridName, szRightToken);		
	}
	else		// Wrong data.
	{
		fclose(fp);
		fp = NULL;
		return FALSE;
	}
	
	// Restore used memory.

	fclose(fp);
	fp = NULL;	

	return TRUE;
}


BOOL CModelViewToolInfo::CreateDefaultMap()
{
//	BOOL bResult = g_pExecutive->LoadMapScript(

	return TRUE;
}
//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::ClearGridInformation
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::ClearGridInformation(VOID)
{
	// Restore used memory.

	if (NULL == g_pExecutive)
	{
		return FALSE;
	}

	if (NULL != m_gxhGrid)
	{
		g_pExecutive->DeleteGXObject(m_gxhGrid);
		m_gxhGrid = NULL;
	}

	// Reset all member variables .

	m_bIsGrid = FALSE;
	m_bDisplayGrid = FALSE;

	memset(m_szGridName, 0, 1024);

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::CreateGridInformation
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::CreateGridInformation(VOID)
{
	// Create neccessary resource.

	if (NULL == g_pExecutive)
	{
		return FALSE;
	}

	if (0 == strcmp(m_szGridName, ""))
	{
		return FALSE;
	}

	m_gxhGrid = g_pExecutive->CreateGXObject(m_szGridName,
										     NULL,
											 NULL,
											 GXOBJECT_CREATE_TYPE_NOT_OPTIMIZE);

	if (NULL == m_gxhGrid)	// There is no grid model data.
	{
		ClearGridInformation();
		return FALSE;
	}

	// Set require flags.

	m_bIsGrid = TRUE;
	m_bDisplayGrid = FALSE;

	DisplayGrid(m_bDisplayGrid);

	return TRUE;
}


//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::DisplayGrid
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::DisplayGrid(BOOL bDisplay)
{
	// Is grid loaded?

	if (FALSE == m_bIsGrid)
	{
		return FALSE;
	}

	// Is Executive object created?

	if (NULL == g_pExecutive)
	{
		return FALSE;
	}


	if (TRUE == bDisplay)
	{
		m_bDisplayGrid = TRUE;
		g_pExecutive->EnableRender(m_gxhGrid);
	}
	else
	{
		m_bDisplayGrid = FALSE;
		g_pExecutive->DisableRender(m_gxhGrid);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::DisplayBoundingBox
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::DisplayBoundingBox(BOOL bDisplay)
{
	// Is Excutive object created?

	if (NULL == g_pExecutive)
	{
		return FALSE;
	}


	if (TRUE == bDisplay)
	{
		g_ToolInfo.m_bDisplayBoundingBox = TRUE;
		g_pExecutive->GetGeometry()->SetDrawDebugFlag(DEBUG_DRAW_MODEL_COL_MESH);
	}
	else
	{
		g_ToolInfo.m_bDisplayBoundingBox = FALSE;
		DWORD	dwFlag = g_pExecutive->GetGeometry()->GetDrawDebugFlag();
		dwFlag &= ~(DEBUG_DRAW_MODEL_COL_MESH);
		g_pExecutive->GetGeometry()->SetDrawDebugFlag(dwFlag);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::DisplayBoneBoundingBox
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::DisplayBoneBoundingBox(BOOL bDisplay)
{
	// Is Excutive object created?

	if (NULL == g_pExecutive)
	{
		return FALSE;
	}


	if (TRUE == bDisplay)
	{
		g_ToolInfo.m_bDisplayBoneBoundingBox = TRUE;
		g_pExecutive->GetGeometry()->SetDrawDebugFlag(DEBUG_DRAW_BONE_COL_MESH);

		int i = 0;
		for (i = 0; i < m_lstGXObject.GetCount(); i++)
		{
			POSITION pos = m_lstGXObject.FindIndex(i);
			GXOBJECT_HANDLE gxh = m_lstGXObject.GetAt(pos);
			DWORD dwFlag = g_pExecutive->GetRenderFlag(gxh);
			dwFlag |= RENDER_TYPE_NOT_DRAW;
			g_pExecutive->SetRenderFlag(gxh,dwFlag);

			
		}
	}
	else
	{
		g_ToolInfo.m_bDisplayBoneBoundingBox = FALSE;
		DWORD	dwFlag = g_pExecutive->GetGeometry()->GetDrawDebugFlag();
		dwFlag &= ~(DEBUG_DRAW_BONE_COL_MESH);
		g_pExecutive->GetGeometry()->SetDrawDebugFlag(dwFlag);
		int i = 0;
	
		for (i = 0; i < m_lstGXObject.GetCount(); i++)
		{
			POSITION pos = m_lstGXObject.FindIndex(i);
			GXOBJECT_HANDLE gxh = m_lstGXObject.GetAt(pos);
			DWORD dwFlag = g_pExecutive->GetRenderFlag(gxh);
			dwFlag &= ~(RENDER_TYPE_NOT_DRAW);
			g_pExecutive->SetRenderFlag(gxh,dwFlag);
		}
	}

	

	return TRUE;
}
//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::RenderCameraInfo
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::RenderCameraInfo(VOID)
{
	// Written by KBS. 

	VECTOR3 v3Angle;
	CAMERA_DESC	desc;
	char szStr[ 0xff ];

	g_pExecutive->GetGeometry()->GetCameraAngleRad(&v3Angle, 0);
	g_pExecutive->GetGeometry()->GetCameraDesc(&desc, 0);	
	

	sprintf(szStr, "Camera From  (%8.2f, %8.2f, %8.2f)\nCamera Angle (%8.2f, %8.2f, %8.2f)",
				   desc.v3From.x,
				   desc.v3From.y,
				   desc.v3From.z,
				   v3Angle.x,
				   v3Angle.y,
				   v3Angle.z);

	RECT rect;
	rect.top	= 10;
	rect.bottom = 30;
	rect.left	= 550;
	rect.right	= 700;
	
	g_pExecutive->GetGeometry()->RenderFont(g_ToolInfo.m_pFont,
											szStr,
											strlen(szStr),
											&rect, 
											0xffffffff,
											CHAR_CODE_TYPE_ASCII,
											0xff,
											0);

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::AddWavFile
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::AddWavFile(char* szFileName)
{
	if (NULL == g_pAudio)
	{
		return FALSE;
	}

	GXSOUND_HANDLE hGxSound = g_pAudio->CreateSoundObject(szFileName, NULL, NULL, 0);
	if (NULL == hGxSound)
	{
		return FALSE;
	}

	m_lstGXAudio.AddTail(hGxSound);	

	CString strAddFileName = szFileName;
	m_lstWavFileName.AddTail(strAddFileName);

	// Set flag relation with wav file name list.

	++m_iCurrentWavNo;
	++m_iTotalWavNo;

	SetCurrentWavFile(m_iCurrentWavNo);

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::RemoveWavFile
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::RemoveWavFile(int iCount)
{
	if (NULL == g_pAudio)
	{
		return FALSE;
	}

	StopPlaySound();

	if (0 > iCount)
	{
		return FALSE;
	}
	
	if (iCount >= m_lstWavFileName.GetCount())
	{
		return FALSE;
	}

	// Delete Sound handle.

	POSITION HandlePos = m_lstGXAudio.FindIndex(iCount);
	GXSOUND_HANDLE  hGxSound = m_lstGXAudio.GetAt(HandlePos);

	if (m_gxLastSoundHandle == hGxSound)
	{
		m_gxLastSoundHandle = NULL;
	}

	m_lstGXAudio.RemoveAt(HandlePos);
	g_pAudio->DeleteSoundObject(hGxSound);

	POSITION pos = m_lstWavFileName.FindIndex(iCount);	
	m_lstWavFileName.RemoveAt(pos);

	--m_iCurrentWavNo;
	--m_iTotalWavNo;

	if (0 > m_iCurrentWavNo && 0 < m_iTotalWavNo)
	{
		m_iCurrentWavNo = -1;
	}	

	SetCurrentWavFile(m_iCurrentWavNo);

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::AddWavFile
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::PlayWav(int iCount)
{
	if (NULL == g_pAudio)
	{
		return FALSE;
	}

	if (FALSE == m_bPlaySound)
	{
		return FALSE;
	}

	if (0 > iCount)
	{
		return FALSE;
	}

	if (m_lstWavFileName.GetCount() <= m_iCurrentWavNo)
	{
		return FALSE;
	}	

	POSITION pos = m_lstGXAudio.FindIndex(iCount);
	GXSOUND_HANDLE GxSound = m_lstGXAudio.GetAt(pos);

	if (NULL != m_gxLastSoundHandle)
	{
		g_pAudio->Stop(m_gxLastSoundHandle);
	}
			
	g_pAudio->Play(GxSound, 0);
	m_gxLastSoundHandle = GxSound;


	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::SetCurrentWavFile
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::SetCurrentWavFile(int iCount)
{
	if (NULL == g_pAudio)
	{
		return FALSE;
	}

	if (0 > iCount)
	{
		m_iCurrentWavNo = -1;
		return FALSE;
	}

	if (iCount >= m_lstWavFileName.GetCount())
	{
		m_iCurrentWavNo = -1;
		return FALSE;
	}

	m_iCurrentWavNo = iCount;		

	return TRUE;	
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::StopPlaySound
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::StopPlaySound(VOID)
{
	if (NULL == g_pAudio)
	{
		return FALSE;
	}

	m_bPlaySound = FALSE;	

	if (-1 != m_iCurrentWavNo)
	{
		POSITION pos = m_lstGXAudio.FindIndex(m_iCurrentWavNo);
		GXSOUND_HANDLE GxSound = m_lstGXAudio.GetAt(pos);

		g_pAudio->Stop(GxSound);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::DeleteAllAudio
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::DeleteAllAudio(VOID)
{
	if (NULL == g_pAudio)
	{
		return FALSE;
	}

	int i = 0;

	for (i = 0; i < m_lstGXAudio.GetCount(); i++)
	{
		POSITION pos = m_lstGXAudio.FindIndex(i);
		GXSOUND_HANDLE hGxSound = m_lstGXAudio.GetAt(pos);

		g_pAudio->DeleteSoundObject(hGxSound);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::ClearAudioInformation
// Description :
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::ClearAudioInformation(VOID)
{
	StopPlaySound();
	DeleteAllAudio();

	m_lstWavFileName.RemoveAll();	
	m_lstGXAudio.RemoveAll();

	m_iCurrentWavNo = -1;
	m_gxLastSoundHandle = NULL;

	return TRUE;

}


//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::OnLoadIllusionMesh
// Description :	잔상 관련용으로 추가한 함수들	2003.09.17
//					m_Illusion의 내용만 설정하고, 실제 실행되는건 ApplyIllusionMeshStatus()에서이다.
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::OnLoadIllusionMesh( GXOBJECT_HANDLE gxhTarget,int iModelIndex, char* szObjectName, char* szIllusionMeshName)
{
	if( m_Illusion.bIllusionMeshBlockSet)
	{
		// 이미 잔상이 올라와있나.?
		_asm int 3;
	}

	// m_Illusion의 내용을 체우고,
	m_Illusion.iAttachTargetModelIndex = iModelIndex;
	m_Illusion.bIllusionMeshBlockSet			=	TRUE;

	m_Illusion.gxhIllusionMesh			=	0;
	lstrcpy( m_Illusion.szIllusionMeshFileName, szIllusionMeshName);
	m_Illusion.dwIllusionLength			=	5;					// 잔상의 길이 기본값은 5?

	m_Illusion.gxhIllusionAttachTarget	=	gxhTarget;
	lstrcpy( m_Illusion.szObjectName, szObjectName);

	CreateIllusionMesh();

	return	FALSE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::ApplyIllusionMeshStatus
// Description :	잔상 관련용으로 추가한 함수들	2003.09.17
//					잔상이 있으면 파기 하고 m_Illusion의 내용을 비운다.
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::OnUnloadIllusionMesh()
{
	if( m_Illusion.gxhIllusionMesh)
	{
		this->DeleteIllusionMesh();
		memset( &m_Illusion, 0, sizeof(m_Illusion));
	}
	return	TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::ApplyIllusionMeshStatus
// Description :	잔상 관련용으로 추가한 함수들	2003.09.17
//					m_Illusion내에 주어진 인자로 오브젝트를 생성한다.
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::CreateIllusionMesh()
{
	char	szCurrentPath[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, szCurrentPath);

	char	szDataPath[MAX_PATH];
	QueryDirectoryString( szDataPath, m_Illusion.szIllusionMeshFileName);

	SetCurrentDirectory( szDataPath);

	m_Illusion.gxhIllusionMesh	=	g_pExecutive->CreateGXObject( m_Illusion.szIllusionMeshFileName, 0, 0, 0);
	if( m_Illusion.gxhIllusionMesh == NULL)
	{
		_asm int 3;
	}

	SetCurrentDirectory( szCurrentPath);

	// 잔상효과를 설정한 칼을 파이터에게 붙인다.
	//g_pExecutive->GXOAttach( m_Illusion.gxhIllusionMesh, m_Illusion.gxhIllusionAttachTarget, m_Illusion.szObjectName);
	g_pExecutive->GXOAttachWithModelIndex( m_Illusion.gxhIllusionMesh, m_Illusion.gxhIllusionAttachTarget,m_Illusion.iAttachTargetModelIndex, m_Illusion.szObjectName);

////////////	////////////	////////////	////////////	////////////	////////////	////////////	////////////	////////////	
	DWORD			dwRefIndex;
	I3DModel*		pModel;
	g_pExecutive->GXOGetModel( m_Illusion.gxhIllusionMesh, &pModel, &dwRefIndex, 0);

	DWORD	dwObjCount	=	pModel->GetObjectNum();
	DWORD	i;
	char*	szObjName;
	for( i = 0; i < dwObjCount; i++)
	{
		szObjName	=	pModel->GetObjectName(i);
		_asm nop
	}

	pModel->Release();


	// 잔상용 오브젝트의 이름은 "_ILLUSION_MESH_" 혹은 "_ILLUSION_MESH" 두개만.... '_' 이거 하나 차이이다.

	//	인자리스트 ::attach할 대상 gxobject, 대상gxobject의 pModel, pModel내의 오브젝트 이름, 실제 갖다 붙일 잔상 오브젝트.
	BOOL	bResult	=	g_pExecutive->GXOInitializeIllusionEffect(
		m_Illusion.gxhIllusionMesh,			// 잔상이 붙은 GXObject, 예를 들면 칼 따위..
		m_Illusion.dwIllusionLength,				// 잔상이 따라다닐 프레임 수..크게 할수록 늘어지는 효과. 커질수록 느려진다.
		"_ILLUSION_MESH_",			// 잔상으로 사용할 오브젝트 이름. _ILLUSION_MESH_....어쩌고가 된다.
		m_Illusion.phMaterial,					// 잔상에 적용할 매터리얼 핸들.매터리얼을 따로 만들지 않는 경우 NULL
		0);							// 현재 사용하지 않는다.
	if( bResult == FALSE)
	{
		bResult	=	g_pExecutive->GXOInitializeIllusionEffect(
			m_Illusion.gxhIllusionMesh,			// 잔상이 붙은 GXObject, 예를 들면 칼 따위..
			m_Illusion.dwIllusionLength,				// 잔상이 따라다닐 프레임 수..크게 할수록 늘어지는 효과. 커질수록 느려진다.
			"_ILLUSION_MESH",			// 잔상으로 사용할 오브젝트 이름. _ILLUSION_MESH_....어쩌고가 된다.
			m_Illusion.phMaterial,					// 잔상에 적용할 매터리얼 핸들.매터리얼을 따로 만들지 않는 경우 NULL
			0);				 			// 현재 사용하지 않는다.
	}

	if( bResult == FALSE)
	{
		// 잔상용 오브젝트를 포함하고 있지 않다고 가정한다.
		MessageBox( 0, "잔상용 오브젝트 (_ILLUSION_MESH, _ILLUSION_MESH_)를 찾을 수 없습니다.", "Error", MB_OK);
	}


	g_pExecutive->GXOBeginIllusionEffect(m_Illusion.gxhIllusionMesh);

	// 혹시 보이지 마라가 설정이 되어 있다면,
//	if( !bShow)
//		g_pExecutive->GXOEndIllusionEffect( m_gxhIllusionMesh);

////////////	////////////	////////////	////////////	////////////	////////////	////////////	////////////	
	// 잔상 시작..
//	SetIllusionMeshStatus( 5, 0, TRUE);
	return	FALSE;
}
//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::ApplyIllusionMeshStatus
// Description :	잔상 관련용으로 추가한 함수들	2003.09.17
//					잔상 오브젝트가 있을 경우 날린다. m_Illusion 블럭은 건드리지 않는다.
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::DeleteIllusionMesh()
{
	// 어태치 된것을 뗀다.
	if( m_Illusion.gxhIllusionMesh)
	{
		g_pExecutive->GXOEndIllusionEffect( m_Illusion.gxhIllusionMesh);
		g_pExecutive->GXODetach( m_Illusion.gxhIllusionAttachTarget, m_Illusion.gxhIllusionMesh);
		g_pExecutive->DeleteGXObject( m_Illusion.gxhIllusionMesh);
		m_Illusion.gxhIllusionMesh	=	0;
	}

	// 메쉬를 언로드 한다.
	return	TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::ApplyIllusionMeshStatus
// Description :	잔상 관련용으로 추가한 함수들	2003.09.17
//					기존의 잔상 오브젝트를 파기하고 새로 생성한다.
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::ApplyIllusionMeshStatus( DWORD dwFrameCount, void* pMtlHandle, BOOL bShow)
{
	DeleteIllusionMesh();


	m_Illusion.dwIllusionLength	=	dwFrameCount;
	m_Illusion.phMaterial		=	pMtlHandle;

	CreateIllusionMesh();

	if( bShow == FALSE)
	{
		g_pExecutive->GXOEndIllusionEffect( m_Illusion.gxhIllusionMesh);
	}


	return	FALSE;
}


//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::SetBackgroundColor
// Description :	
//----------------------------------------------------------------------------
void CModelViewToolInfo::SetBackgroundColor(DWORD R, DWORD G, DWORD B)
{

}


//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::OnLButtonDown
// Description :	
//----------------------------------------------------------------------------
void CModelViewToolInfo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	POINT		pt;
	
	VECTOR3		v3Pos;
	float		fDistResult = 9000000.0f;
	DWORD		dwModelIndexResult;
	DWORD		dwObjIndexResult;
	
	GXOBJECT_HANDLE	gxoResult = NULL;
	

	float			fDist;
	DWORD			dwModelIndex;
	DWORD			dwObjIndex;
	

	pt.x = point.x;
	pt.y = point.y;

	if (nFlags & MK_CONTROL)
	{

		char* pszObjName = NULL;
		BOOL	bResult = FALSE;


		int i = 0;
		for (i = 0; i < m_lstGXObject.GetCount(); i++)
		{
			POSITION pos = m_lstGXObject.FindIndex(i);
			GXOBJECT_HANDLE gxh = m_lstGXObject.GetAt(pos);
			if (g_pExecutive->IsCollisionWithScreenCoord(gxh,&v3Pos,&dwModelIndex,&dwObjIndex,&fDist,&pt,0,PICK_TYPE_PER_BONE_OBJECT))
			{
				if (fDist < fDistResult)
				{
					fDistResult = fDist;
					dwModelIndexResult = dwModelIndex;
					dwObjIndexResult = dwObjIndex;
					gxoResult = gxh;
				}
				bResult = TRUE;

			}
		}
		if (bResult)
		{
			I3DModel*	pModel;
			DWORD		dwRefIndex;
			if (g_pExecutive->GXOGetModel(gxoResult,&pModel,&dwRefIndex,dwModelIndex)== S_OK)
			{
				pszObjName = pModel->GetObjectName(dwObjIndex);
				pModel->Release();
				SetDlgItemText(g_pDlgBar->m_hWnd,IDC_STATIC_PICK_OBJNAME,pszObjName);
			}
		}
	}
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::CreateFrontOfCameraObject
// Desc : 
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::CreateFrontOfCameraObject(char* pszFrontOfCameraFileName)
{
	//
	// Argument checking.
	//

	if (NULL == pszFrontOfCameraFileName)
	{
		return FALSE;
	}

	//
	// If there is existing already, destroy legacy Front of camera object.
	//

	if (NULL != m_gxhFrontOfCamera)
	{
		RemoveFrontOfCameraObject();
	}

	//
	// And Create new front of camera object.
	//

	m_gxhFrontOfCamera = g_pExecutive->CreateGXObject(pszFrontOfCameraFileName,
													  NULL,
													  NULL,
													  GXOBJECT_CREATE_TYPE_DEFAULT_PROC);
	//
	// Attatch it to Camera.
	//

	g_pExecutive->GXOAttachCameraFront(m_gxhFrontOfCamera, m_fFrontOfCameraDistance);
	g_pExecutive->SetAlphaFlag(m_gxhFrontOfCamera, m_byFrontOfCameraAlpah);


	//
	// Save require informations.
	//
	
	lstrcpy(m_szFrontOfCameraName, pszFrontOfCameraFileName);

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::RemoveFrontOfCameraObject
// Desc : 
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::RemoveFrontOfCameraObject(VOID)
{
	//
	// If there is front of camera object, then remove it.
	//

	if (NULL != m_gxhFrontOfCamera)
	{
		g_pExecutive->GXODetachCameraFront(m_gxhFrontOfCamera);
		g_pExecutive->DeleteGXObject(m_gxhFrontOfCamera);		
	}

	// Front of Camera object properties.

	m_byFrontOfCameraAlpah		= 40;		// default values.	
	m_gxhFrontOfCamera			= NULL;
	m_fFrontOfCameraDistance	= 25.0f;	
	
	lstrcpy(m_szFrontOfCameraName, "None");
	
	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::SetFrontOfCameraAlpha
// Desc : 
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::SetFrontOfCameraAlpha(BYTE byAlpha)
{
	//
	// If there is no front of camera object, don't do anything.
	//

	if (NULL == m_gxhFrontOfCamera)
	{
		return FALSE;
	}

	//
	// Set alpha level.
	//

	m_byFrontOfCameraAlpah = byAlpha;
	g_pExecutive->SetAlphaFlag(m_gxhFrontOfCamera, byAlpha);

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::GetFrontOfCameraAlpha
// Desc : 
//----------------------------------------------------------------------------
BYTE CModelViewToolInfo::GetFrontOfCameraAlpha(VOID)
{
	//
	// Return alpha level.
	//

	return m_byFrontOfCameraAlpah;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::SetFrontOfCameraDistance
// Desc : 
//----------------------------------------------------------------------------
BOOL CModelViewToolInfo::SetFrontOfCameraDistance(float fDistance)
{
	if (NULL != m_gxhFrontOfCamera)
	{
		g_pExecutive->GXODetachCameraFront(m_gxhFrontOfCamera);
		g_pExecutive->GXOAttachCameraFront(m_gxhFrontOfCamera, fDistance);
	}

	m_fFrontOfCameraDistance = fDistance;

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewToolInfo::GetFrontOfCameraDistance
// Desc : 
//----------------------------------------------------------------------------
float CModelViewToolInfo::GetFrontOfCameraDistance(VOID)
{
	return m_fFrontOfCameraDistance;
}


//----------------------------------------------------------------------------
