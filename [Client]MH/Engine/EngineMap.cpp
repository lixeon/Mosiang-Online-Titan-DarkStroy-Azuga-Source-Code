// EngineMap.cpp: implementation of the EngineMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EngineMap.h"
#include "../MainGame.h"
#include "MHFile.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEngineMap::CEngineMap()
{
	m_bMapBuild = FALSE;

	m_ghMap = NULL;
}

CEngineMap::~CEngineMap()
{
	//Release();
}

BOOL CEngineMap::Init(char* szMapFile)
{
//	return g_pExecutive->LoadMapScript(szMapFile,NULL,LOAD_MAP_FLAG_DEFAULT_PROC_AUTOANIMATION);

	m_pExecutive = g_pExecutive;
	
	BOOL	bBuildTree	=	FALSE;	

	char	buf[1024];
	memset(buf, 0, 1024);

	CMHFile file;
	file.Init(szMapFile,"ET",MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT);

	LOGFILE("%s file inited",szMapFile);

	while (file.GetString(buf)!=EOF)
	{
		if( !lstrcmp( buf, PID_GX_METADATA))
		{
			while( lstrcmp( buf, "}"))
			{
				file.GetString(buf);
				if( !lstrcmp( buf, PID_BOUNDINGBOX_MAX))
				{
					m_WorldBox.Max.x = file.GetFloat();
					m_WorldBox.Max.y = file.GetFloat();
					m_WorldBox.Max.z = file.GetFloat();					
				}
				else if( !lstrcmp( buf, PID_BOUNDINGBOX_MIN))
				{					
					m_WorldBox.Min.x = file.GetFloat();
					m_WorldBox.Min.y = file.GetFloat();
					m_WorldBox.Min.z = file.GetFloat();
				}
			}
		}
		if (!lstrcmp(buf,PID_GX_MAP))
		{
			while (lstrcmp(buf,"}"))
			{
				file.GetString(buf);
				if (!lstrcmp(buf,PID_STATIC_MODEL))
				{
					file.GetString(buf);
					LOGFILE("%s PID_STATIC_MODEL",buf);
					ImportStaticModel(buf);
					bBuildTree	=	TRUE;
				}
				if (!lstrcmp(buf,PID_HEIGHT_FIELD))
				{
					file.GetString(buf);
					LOGFILE("%s PID_HEIGHT_FIELD",buf);
					ImportHeightField(buf,0);
					bBuildTree	=	TRUE;
				}


			}
			if (bBuildTree)
			{
				LOGFILE("bBuildTree");
				BuildMap(&m_WorldBox);
			}			
		}

		if (!lstrcmp(buf,PID_GX_OBJECT))
		{
			DWORD	dwGXObjectsNum;
			dwGXObjectsNum = file.GetDword();

			file.GetString(buf);		// {
			for (DWORD i=0; i<dwGXObjectsNum; i++)
			{
				DWORD		dwFlag;
				DWORD		dwID;
				VECTOR3		v3Scale,v3Pos,v3Axis;
				float		fRad;

				// | 파일명 | 고유ID | 스케일 | 위치 | 축벡터 | 방향각 | 플래그 
				file.GetString(buf);		// 파일명
				LOGFILE("%s gxobject",buf);

				dwID = file.GetDword();		// 고유ID
				v3Scale.x = file.GetFloat();
				v3Scale.y = file.GetFloat();
				v3Scale.z = file.GetFloat();

				v3Pos.x = file.GetFloat();
				v3Pos.y = file.GetFloat();
				v3Pos.z = file.GetFloat();

				v3Axis.x = file.GetFloat();
				v3Axis.y = file.GetFloat();
				v3Axis.z = file.GetFloat();
				fRad = file.GetFloat();

				LOGFILE("hex start");
				file.GetHex(&dwFlag);
				LOGFILE("hex end");
				
				GXOBJECT_HANDLE gxo = m_pExecutive->CreateGXObject(buf,NULL,NULL,dwFlag);
				if(gxo == NULL)
				{					
					char temp[1024];
					sprintf(temp,"Failed to load MapObject (%s)",buf);
					LOGFILE(temp);
					continue;
				}

				//////////////////////////////////////////////////////////////////////////
				// yh edit
				DWORD motionNum = m_pExecutive->GXOGetMotionNum(gxo,0);
				if(motionNum > 0)
				{
					m_pExecutive->DeleteGXObject(gxo);
					gxo = m_pExecutive->CreateGXObject(buf,NULL,NULL,dwFlag|GXOBJECT_CREATE_TYPE_DEFAULT_PROC);
				}
				//////////////////////////////////////////////////////////////////////////

				if (gxo)
				{
					m_pExecutive->SetID(gxo,dwID);

					m_pExecutive->GXOSetScale(gxo,&v3Scale);
					m_pExecutive->GXOSetPosition(gxo,&v3Pos,FALSE);
					m_pExecutive->GXOSetDirection(gxo,&v3Axis,fRad);

					/*
					char*	pFileName;
					pFileName = (char*)m_pExecutive->GetData(gxo);
					pFileName = new char[lstrlen(buf)+1];
					lstrcpy(pFileName,buf);
					m_pExecutive->SetData(gxo,pFileName);
					*/

					// YHEdit
					m_pExecutive->DisableSendShadow(gxo);
					m_pExecutive->DisableUpdateShading(gxo);
					m_pExecutive->DisablePick(gxo);


					m_GXObjectList.AddTail(gxo);
				}
			}
			file.GetString(buf);		// }
		}
		if (!lstrcmp(buf,PID_GX_LIGHT))
		{
			DWORD	dwGXLightsNum;
			dwGXLightsNum = file.GetDword();

			// YH Light 없에 버림
			dwGXLightsNum = 0;

			ASSERTMSG(dwGXLightsNum < 300,"LIGHT 최대 갯수가 300개를 넘었습니다.");


		
			file.GetString(buf);		// {
			for (DWORD i=0; i<dwGXLightsNum; i++)
			{
				
				LIGHT_DESC	lightDesc;
				memset(&lightDesc,0,sizeof(LIGHT_DESC));
				DWORD		dwFlag;

				// | RGBA | 위치 |범위(CM) | 플래그 
				file.GetHex(&lightDesc.dwDiffuse);
				lightDesc.v3Point.x = file.GetFloat();
				lightDesc.v3Point.y = file.GetFloat();
				lightDesc.v3Point.z = file.GetFloat();
				lightDesc.fRs = file.GetFloat();

				file.GetHex(&dwFlag);
				dwFlag |= GXLIGHT_TYPE_STATIC;

				GXLIGHT_HANDLE	gxl = m_pExecutive->CreateGXLight(&lightDesc,NULL,NULL,0xffffffff,NULL,dwFlag);


				if(gxl)
				{
					m_GXLightList.AddTail(gxl);
				}
			}
			file.GetString(buf);		// }
		}
		if (!lstrcmp(buf,PID_GX_TRIGGER))
		{
			DWORD	dwGXTriggerNum;
			dwGXTriggerNum = file.GetDword();
		
			file.GetString(buf);		// {
			for (DWORD i=0; i<dwGXTriggerNum; i++)
			{
				EVENT_TRIGGER_DESC	triggerDesc;
				memset(&triggerDesc,0,sizeof(triggerDesc));
				DWORD		dwFlag;
				DWORD		dwID;

					
				// 고유아이디 | RGBA | 위치 | 스케일 | 회전 | 플래그 
				dwID = file.GetDword();
				file.GetHex(&triggerDesc.dwColor);
				triggerDesc.v3Pos.x = file.GetFloat();
				triggerDesc.v3Pos.y = file.GetFloat();
				triggerDesc.v3Pos.z = file.GetFloat();

				triggerDesc.v3Scale.x = file.GetFloat();
				triggerDesc.v3Scale.y = file.GetFloat();
				triggerDesc.v3Scale.z = file.GetFloat();

				triggerDesc.v3Rot.x = file.GetFloat();
				triggerDesc.v3Rot.y = file.GetFloat();
				triggerDesc.v3Rot.z = file.GetFloat();

				file.GetHex(&dwFlag);

				GXTRIGGER_HANDLE	gxt = m_pExecutive->CreateGXEventTrigger(NULL,NULL,dwFlag);
				m_pExecutive->SetID(gxt,dwID);
				m_pExecutive->GXTSetPosition(gxt,&triggerDesc.v3Pos);
				m_pExecutive->GXTSetScale(gxt,&triggerDesc.v3Scale);
				m_pExecutive->GXTSetRotation(gxt,&triggerDesc.v3Rot);
			}
			file.GetString(buf);		// }
				
		}
		
	} 

	file.Release();

	return m_bMapBuild;
}

void CEngineMap::Release()
{
	if(g_pExecutive)
	{
		//g_pExecutive->DeleteAllGXMapObjectsWitLoadMapScript();

		GXOBJECT_HANDLE gxo;
		GXLIGHT_HANDLE gxl;
		while(gxo = m_GXObjectList.RemoveTail())
		{
			g_pExecutive->DeleteGXObject(gxo);
		}
		while(gxl = m_GXLightList.RemoveTail())
		{
			g_pExecutive->DeleteGXLight(gxl);
		}

		CleanHeightField();
		CleanStaticModel();
		if( m_ghMap )
		{
			g_pExecutive->DeleteGXMap(m_ghMap);	
			m_ghMap = NULL;
		}
	}
}

void CEngineMap::SetAmbientColor(DWORD Color)
{
	g_pExecutive->GetGeometry()->SetAmbientColor(0,Color);
}


BOOL CEngineMap::ImportHeightField(char* szFileName,DWORD dwIndexBufferNum)
{
	
	TEXTURE_TABLE*	pTexTable = NULL;
	BOOL			bResult = FALSE;
	
	CleanHeightField();

	CAMERA_DESC	cameraDesc;
	m_pExecutive->GetGeometry()->GetCameraDesc(&cameraDesc,0);
	m_pExecutive->GetGeometry()->SetCameraViewDistance(100.0f,51200.0f,0);
	
	bResult = m_pExecutive->GetGeometry()->CreateHeightField(&m_pHField,0);

	if (!m_pHField->ReadFile(szFileName,dwIndexBufferNum,NULL,0))
	{
		m_pHField->Release();
		m_pHField = NULL;
		memset(m_szHFieldName,0,sizeof(m_szHFieldName));
		return FALSE;
	}
	lstrcpy(m_szHFieldName,szFileName);


	m_pExecutive->GetGeometry()->SetCameraViewDistance(cameraDesc.fNear,cameraDesc.fFar,0);

	m_pHField->GetHFieldDesc(&m_hfDesc);
	
//	for (DWORD i=0; i<4; i++)
//		ResetDefaultCamera(i);

	m_pExecutive->GetRenderer()->SetLightMapFlag(0);

	return bResult;

}


BOOL CEngineMap::ImportStaticModel(char* szFileName)
{
	I3DStaticModel*	pStaticModel = NULL;
	
	if (0xffffffff == m_pExecutive->GetGeometry()->CreateStaticModel(&pStaticModel,4000,500,0))
		return FALSE;


	if (!pStaticModel->ReadFile(szFileName,NULL,STATIC_MODEL_LOAD_ENABLE_SHADE))
	{
		pStaticModel->Release();
		return FALSE;
	}

	if (m_ghMap)
	{
		m_pExecutive->DeleteGXMap(m_ghMap);
		m_ghMap = NULL;
	}

	CleanStaticModel();

	m_pStaticModel = pStaticModel;
//	m_pStaticModel->BeginCreateMesh(0);

	return TRUE;
}

void CEngineMap::CleanHeightField()
{	
	if (m_pHField)
	{
		m_pHField->Release();
		m_pHField = NULL;
	}

	memset(&m_hfDesc,0,sizeof(m_hfDesc));
	
	m_bMapBuild = FALSE;
}
void CEngineMap::CleanStaticModel()
{
	if (m_pStaticModel)
	{
		m_pStaticModel->Release();
		m_pStaticModel = NULL;
	}
	m_bMapBuild = FALSE;
}

BOOL CEngineMap::BuildMap(MAABB* pWorldBox)
{
	if (m_bMapBuild)
	{
		m_pExecutive->RebuildMap(pWorldBox->Max.y, pWorldBox->Min.y);
		m_WorldBox	=	*pWorldBox;
		return TRUE;

	}
	if( (pWorldBox->Max.y - pWorldBox->Min.y) <= 0.0f)
	{
		pWorldBox->Max.y	=	pWorldBox->Min.y	+	100.0f;
	}
	m_WorldBox.Max.y	=	pWorldBox->Max.y;
	m_WorldBox.Min.y	=	pWorldBox->Min.y;

	m_ghMap = m_pExecutive->CreateGXMap(NULL,0,NULL);
	
	m_pExecutive->BeginBuildMap(0);

	if (m_pStaticModel)
		m_pExecutive->InsertStaticModelTOGXMap(m_pStaticModel);
	
	if (m_pHField)
		m_pExecutive->InsertHFieldToGXMap(m_pHField);

	m_pExecutive->EndBuildMap(pWorldBox->Max.y,pWorldBox->Min.z);

	m_pExecutive->GetWorldBoundingBox( &m_WorldBox);

	m_bMapBuild = TRUE;
	return TRUE;
}
